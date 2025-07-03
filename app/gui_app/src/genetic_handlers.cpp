#include "genetic_frame.hpp"
#include "file_frames.hpp"
#include "settings_frames.hpp"
#include "save_load.hpp"
#include <evolution.hpp>
#include <sstream>
#include <format>
#include <string>
#include <fstream>

namespace genetic_gui
{
    void GeneticFrame::OnClose(wxCloseEvent& event)
    {
        if (frames.algoFrame) 
        {
            frames.algoFrame->Destroy();
            frames.algoFrame = nullptr;
        }
        
        if (frames.rendFrame) 
        {
            frames.rendFrame->Destroy();
            frames.rendFrame = nullptr;
        }

        if (frames.newFrame)
        {
            frames.newFrame->Destroy();
            frames.newFrame = nullptr;
        }
        
        timer.Stop();
        
        wxTheApp->ProcessPendingEvents();
        
        wxTheApp->ExitMainLoop();
        
        this->Destroy(); 
    }

    void GeneticFrame::New(wxCommandEvent& event)
    {
        frames.newFrame->Show();
        frames.newFrame->SetFocus();
    }

    void GeneticFrame::Save(wxCommandEvent& event)
    {
        wxFileDialog save_file_dialog(
            this,                       
            _("Save current session."),        
            "",                         
            "session.json",                         
            "Json files (*.json)|*.json", 
            wxFD_SAVE | wxFD_OVERWRITE_PROMPT
        );

        int res = save_file_dialog.ShowModal();

        if (res == wxID_CANCEL) return;

        if (res == wxID_OK)
        {
            std::string filepath = save_file_dialog.GetPath().ToStdString();

            if (!filepath.ends_with(".json")) 
            {
                filepath += ".json";
            }

            json j;

            j["poly"]            = poly;
            j["best_individ"]    = step;
            j["controller"]      = controller;
            j["algo_settings"]   = algo_settings;
            j["render_settings"] = render_settings;

            std::ofstream(filepath) << j.dump(4);
        }
    }

    void GeneticFrame::Load(wxCommandEvent& event)
    {
        wxFileDialog load_file_dialog(
            this,                       
            _("Load saved session."),        
            "",                         
            "",                         
            "Json files (*.json)|*.json", 
            wxFD_OPEN | wxFD_FILE_MUST_EXIST
        );

        int res = load_file_dialog.ShowModal();

        if (res == wxID_CANCEL) return;
        
        if (res == wxID_OK)
        {
            std::string filepath = load_file_dialog.GetPath().ToStdString();

            std::ifstream in(filepath);

            if (!in.is_open()) 
            {
                statusbar->SetStatusText("Failed to load file.");
                return;
            }

            json j;
            in >> j;

            poly            = j.at("poly").get<genetic::Polynomial>();
            step            = j.at("best_individ").get<genetic::Individ>();
            controller      = j.at("controller").get<genetic_gui::GeneticController>();
            algo_settings   = j.at("algo_settings").get<genetic_gui::AlgoSettings>();
            render_settings = j.at("render_settings").get<genetic_gui::RenderingSettings>();

            best_individ_history.clear();
            mean_fitness_history.clear();

            best_individ_history.push_back(step);
            mean_fitness_history.push_back(step.fitness);

            statusbar->SetStatusText("Successfully loaded session!");
            
            wxMessageDialog dialog(this, 
                "Do you want to continue algorithm?", 
                "Choose option", 
                wxYES_NO | wxICON_QUESTION);

            dialog.SetYesNoLabels("Continue", "No, just show me result");
            
            int result = dialog.ShowModal();

            if (result == wxID_YES)
            {
                frames.loadFrame->Show();
                frames.loadFrame->SetFocus();

                RefreshPlots();
                controller.is_running = true;
                timer.Stop();

                return;
            }
            else if (result == wxID_NO)
            {
                RefreshPlots();
                controller.is_running = true;
                timer.Stop();
                return;
            }
        }
    }

    void GeneticFrame::RendSettings(wxCommandEvent& event)
    {
        frames.rendFrame->Show();
        frames.rendFrame->SetFocus();
    }

    void GeneticFrame::AlgoSettings(wxCommandEvent& event)
    {
        frames.algoFrame->Show();
        frames.algoFrame->SetFocus();
    }

    void GeneticFrame::Next(wxCommandEvent& event)
    {
        if (controller.IsRunning())
        {
            if (timer.IsRunning()) timer.Stop();

            if (controller.CanRedo())
            {
                controller.RedoStep();
                
                std::string str = std::format("Mean fitness: {:.2f} Best fitness: {:.2f}", 
                                mean_fitness_history.back(), 
                                best_individ_history.back().fitness);
                statusbar->SetStatusText(str);
            }
            else
            {
                if (mean_fitness_history.size() > 0 && std::isnan(mean_fitness_history.back()))
                {
                    std::string str = std::format("Error occured, nan fitness value. Stopping algorithm.");
                    statusbar->SetStatusText(str);
                    return;
                }
                
                if (best_individ_history.size() > 0 && mean_fitness_history.size() > 0 && algo_settings.verbose)
                {
                    std::string str = std::format("Mean fitness: {:.2f} Best fitness: {:.2f}", 
                                    mean_fitness_history.back(), 
                                    best_individ_history.back().fitness);
                    statusbar->SetStatusText(str);
                }
                else
                {
                    statusbar->SetStatusText("Algorithm is working...");
                }
                
                if (!controller.MakeStep()) 
                {
                    std::string str = std::format("Algorithm successfully ended! Best fitness: {:.2f}", best_individ_history.back().fitness);
                    statusbar->SetStatusText(str);
                }
            }
        }
        else 
        {
            std::string str = std::format("Error. Algorithm is not in progress.");
            statusbar->SetStatusText(str);
        }
    }
    
    void GeneticFrame::Prev(wxCommandEvent& event)
    {
        if (controller.IsRunning()) 
        {
            if (timer.IsRunning()) timer.Stop();

            if (controller.MakeStepBack())
            {
                std::string str = std::format("Mean fitness: {:.2f} Best fitness: {:.2f}", 
                                mean_fitness_history.back(), 
                                best_individ_history.back().fitness);
                statusbar->SetStatusText(str);
            }
            else
            {
                std::string str = std::format("Can not go any further.");
                statusbar->SetStatusText(str);
            }
        }
        else
        {
            std::string str = std::format("Error. Algorithm is not in progress.");
            statusbar->SetStatusText(str);
        }
    }

    void GeneticFrame::Stop(wxCommandEvent& event)
    {
        if (controller.IsRunning())
        {
            if (!timer.IsRunning()) timer.Start(1000 / render_settings.fps);
            else timer.Stop();
        }
        else
        {
            std::string str = std::format("Error. Algorithm is not in progress.");
            statusbar->SetStatusText(str);
        }
    }

    void GeneticFrame::RefreshPlots()
    {
        frames.mainFrame->timer.Stop();

        frames.mainFrame->fitnessplot->Destroy();
        frames.mainFrame->fitnessplot = new FitnessPlot(frames.mainFrame->fitnessplot_panel, &frames.mainFrame->controller);
        frames.mainFrame->fitnessplot_sizer->Add(frames.mainFrame->fitnessplot, 1, wxALL|wxEXPAND, 2);
        frames.mainFrame->fitnessplot_panel->Layout();

        frames.mainFrame->algoplot->Destroy();
        frames.mainFrame->algoplot = new AlgoPlot(frames.mainFrame->alogplot_panel, &frames.mainFrame->controller);
        frames.mainFrame->algoplot_sizer->Add(frames.mainFrame->algoplot, 1,  wxALL|wxEXPAND, 2);
        frames.mainFrame->alogplot_panel->Layout();

        frames.mainFrame->controller.ClearObservers();

        frames.mainFrame->controller.AddObserver(frames.mainFrame->algoplot);
        frames.mainFrame->controller.AddObserver(frames.mainFrame->fitnessplot);

        frames.mainFrame->timer.Start(1000 / render_settings.fps);
    }

    void NewFrame::OnNumberInput(wxCommandEvent& event) 
    {
        wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
        if (ctrl) 
        {
            double value;
            if (!ctrl->GetValue().ToDouble(&value) || value <= 0) 
            {
                ctrl->SetBackgroundColour(wxColour(255, 200, 200));
            } 
            else 
            {
                ctrl->SetBackgroundColour(wxNullColour);
            }
            ctrl->Refresh();
        }
    }

    void NewFrame::OnClose(wxCloseEvent& event)
    {
        this->Hide();
    }

    genetic::Polynomial NewFrame::ParsePoly(const std::string& poly_str) 
    {
        genetic::Polynomial res;
        
        std::string cleaned = poly_str;
        cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ' '), cleaned.end());
        
        if (cleaned.empty()) {
            return res;
        }
        
        if (cleaned[0] != '+' && cleaned[0] != '-') 
        {
            cleaned = "+" + cleaned;
        }
        
        std::vector<std::string> terms;
        size_t start = 0;
        bool in_exponent = false;
        
        for (size_t i = 0; i < cleaned.length(); ++i) 
        {
            if (cleaned[i] == '^') {
                in_exponent = true;
                continue;
            }
            
            if (in_exponent && (cleaned[i] == '+' || cleaned[i] == '-')) 
            {
                in_exponent = false;
            }
            
            if (!in_exponent && (cleaned[i] == '+' || cleaned[i] == '-') && i > 0) 
            {
                terms.emplace_back(cleaned, start, i - start);;
                start = i;
            }
        }
        
        if (start < cleaned.length()) 
        {
            terms.push_back(cleaned.substr(start));
        }
        
        for (const std::string& term : terms) 
        {
            if (term.empty()) continue;
            
            try 
            {
                ParseTerm(term, res);
            } 
            catch (const std::exception& e) 
            {
                throw std::runtime_error("Error parsing term '" + term + "': " + e.what());
            }
        }
        
        return res;
    }

    void NewFrame::ParseTerm(const std::string& term, genetic::Polynomial& poly) 
    {
        if (term.empty()) return;
        
        double coefficient = 1.0;
        double power = 0.0;
        
        std::string work_term = term;
        bool negative = false;
        
        if (work_term[0] == '-') 
        {
            negative = true;
            work_term = std::string(work_term.begin() + 1, work_term.end());;
        } 
        else if (work_term[0] == '+') 
        {
            work_term = std::string(work_term.begin() + 1, work_term.end());;
        }
        
        if (work_term.empty()) {
            poly.push_back({negative ? -1.0 : 1.0, 0.0});
            return;
        }
        
        size_t x_pos = work_term.find('x');
        
        if (x_pos != std::string::npos) {
            std::string coeff_part = work_term.substr(0, x_pos);
            
            if (!coeff_part.empty() && coeff_part.back() == '*') {
                coeff_part.pop_back();
            }
            
            if (coeff_part.empty()) {
                coefficient = 1.0;
            } 
            else if (coeff_part == "-") {
                coefficient = -1.0;
            }
            else {
                coefficient = ParseCoefficient(coeff_part);
            }
            
            if (x_pos + 1 < work_term.length() && work_term[x_pos + 1] == '^') 
            {
                std::string power_str = work_term.substr(x_pos + 2);
                if (power_str.empty()) {
                    throw std::runtime_error("Missing exponent after '^'");
                }
                power = ParseCoefficient(power_str);
            } 
            else if (x_pos + 1 < work_term.length()) 
            {
                throw std::runtime_error("Unexpected character after 'x'");
            }
            else 
            {
                power = 1.0;
            }
        } 
        else 
        {
            coefficient = ParseCoefficient(work_term);
            power = 0.0;
        }
        
        if (negative) 
        {
            coefficient = -coefficient;
        }
        
        poly.push_back({coefficient, power});
    }

    double NewFrame::ParseCoefficient(const std::string& coeff_str) 
    {
        if (coeff_str.empty()) 
        {
            return 1.0;
        }
        
        try {
            std::string converted = coeff_str;
            std::replace(converted.begin(), converted.end(), ',', '.');
            
            size_t slash_pos = converted.find('/');
            if (slash_pos != std::string::npos) 
            {
                std::string numerator_str = converted.substr(0, slash_pos);
                std::string denominator_str = converted.substr(slash_pos + 1);
                
                if (numerator_str.empty() || denominator_str.empty()) {
                    throw std::runtime_error("Invalid fraction format");
                }
                
                double numerator = std::stod(numerator_str);
                double denominator = std::stod(denominator_str);
                
                if (denominator == 0.0) {
                    throw std::runtime_error("Division by zero in coefficient");
                }
                
                return numerator / denominator;
            } 
            else 
            {
                return std::stod(converted);
            }
        } catch (const std::exception& e) {
            throw std::runtime_error("Invalid coefficient '" + coeff_str + "': " + e.what());
        }
    }

    void NewFrame::OnCompute(wxCommandEvent& event)
    {
        auto poly_temp = poly_ctrl->GetValue().ToStdString();

        poly = ParsePoly(poly_temp);

        double left, right;
        interval_left_ctrl->GetValue().ToDouble(&left);
        interval_right_ctrl->GetValue().ToDouble(&right);
        if (left == right)
        {   
            wxMessageBox("Wrong interval!", "Error", wxOK | wxICON_ERROR);
            return;
        }
        algo_settings.interval = genetic::Interval{std::min(left, right), std::max(left, right)};

        double padding_factor = 1.1;

        render_settings.x_max = std::max(abs(left), abs(right)) * padding_factor;
        render_settings.x_min = -render_settings.x_max;

        render_settings.y_max = poly.eval(render_settings.x_max) * padding_factor;
        double temp = poly.eval(render_settings.x_min);
        render_settings.y_min = temp > 0 ? temp * -1 * padding_factor: temp * padding_factor;

        int generation;
        generation_size_ctrl->GetValue().ToInt(&generation);
        algo_settings.generation_size = generation;
        
        int chromosome;
        chromosome_size_ctrl->GetValue().ToInt(&chromosome);
        algo_settings.chromosome_size = chromosome;
        
        int epochs;
        epoch_ctrl->GetValue().ToInt(&epochs);
        algo_settings.n_epoch = epochs;
        
        double recombination_prob;
        recomb_p_ctrl->GetValue().ToDouble(&recombination_prob);
        algo_settings.recombination_p = recombination_prob;
        
        double mutation_prob;
        mutation_p_ctrl->GetValue().ToDouble(&mutation_prob);
        algo_settings.mutation_p = mutation_prob;
        
        double eps;
        epsilon_ctrl->GetValue().ToDouble(&eps);
        algo_settings.epsilon = eps;
        
        algo_settings.selection_id      = genetic::SelectionMethod(selectionComboBox->GetSelection());
        algo_settings.recombination_id  = genetic::RecombinationMethod(recombinationComboBox->GetSelection());
        algo_settings.mutation_id       = genetic::MutationMethod(mutationComboBox->GetSelection());

        algo_settings.selection_strategy     = selection_map[genetic::SelectionMethod(selectionComboBox->GetSelection())];
        algo_settings.recombination_strategy = recombination_map[genetic::RecombinationMethod(recombinationComboBox->GetSelection())];
        algo_settings.mutation_strategy      = mutation_map[genetic::MutationMethod(mutationComboBox->GetSelection())];

        frames.mainFrame->RefreshPlots();

        frames.mainFrame->StartAlgo();

        this->Hide();
    }

    void AlgoSettingsFrame::OnClose(wxCloseEvent& event)
    {
        this->Hide();
    }

    void AlgoSettingsFrame::OnApply(wxCommandEvent& event)
    {
        algo_settings.verbose = verbose_ctrl->GetValue();

        double scaling;
        scaling_ctrl->GetValue().ToDouble(&scaling);
        if (scaling > 0)
        {
            algo_settings.max_after_scaling = scaling;
        }
        else
        {
            wxMessageBox("Wrong scaling!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        selection_map[genetic::SelectionMethod::LinearScaling] = 
        [max_after_scaling = algo_settings.max_after_scaling](genetic::Generation& gen) {
            if(!gen.is_scaled) apply_scaling(genetic::ScalingType::linear, gen, max_after_scaling);
            return roulette_rule(gen);
        };

        double delta, sigma;

        delta_ctrl->GetValue().ToDouble(&delta);

        algo_settings.delta = delta;

        sigma_ctrl->GetValue().ToDouble(&sigma);

        algo_settings.sigma = sigma;

        this->Hide();
    }

    void RendSettingsFrame::OnClose(wxCloseEvent& event)
    {
        this->Hide();
    }

    void RendSettingsFrame::OnApply(wxCommandEvent& event)
    {
        int fps = fps_spin->GetValue();
        render_settings.fps = fps;
        
        int res = resolution_spin->GetValue();
        render_settings.resolution = res;

        if (multisampling_check->IsChecked()) 
        {
            render_settings.gl_attribs[0] = WX_GL_SAMPLES;
            render_settings.gl_attribs[1] = 4;
            render_settings.gl_attribs[2] = 0;
        }
        else 
        {
            render_settings.gl_attribs[0] = 0;
        }

        render_settings.show_legend = show_legend_check->IsChecked();

        frames.mainFrame->RefreshPlots();

        frames.mainFrame->algoplot->ApplyRenderSettings();
        frames.mainFrame->fitnessplot->ApplyRenderSettings();

        if (frames.mainFrame->timer.IsRunning())
        {
            frames.mainFrame->timer.Stop();

            int interval = (render_settings.fps > 0) ? (1000 / render_settings.fps) : 100;

            frames.mainFrame->timer.Start(interval);
        }

        this->Hide();
    }

    void LoadFrame::OnClose(wxCloseEvent& event)
    {
        this->Hide();
    }

    void LoadFrame::OnNumberInput(wxCommandEvent& event)
    {
        wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
        if (ctrl) 
        {
            double value;
            if (!ctrl->GetValue().ToDouble(&value) || value <= 0) 
            {
                ctrl->SetBackgroundColour(wxColour(255, 200, 200));
            } 
            else 
            {
                ctrl->SetBackgroundColour(wxNullColour);
            }
            ctrl->Refresh();
        }
    }

    void LoadFrame::OnCompute(wxCommandEvent& event)
    {
        int epochs;
        epoch_ctrl->GetValue().ToInt(&epochs);
        algo_settings.n_epoch = epochs;
        
        double recombination_prob;
        recomb_p_ctrl->GetValue().ToDouble(&recombination_prob);
        algo_settings.recombination_p = recombination_prob;
        
        double mutation_prob;
        mutation_p_ctrl->GetValue().ToDouble(&mutation_prob);
        algo_settings.mutation_p = mutation_prob;
        
        double eps;
        epsilon_ctrl->GetValue().ToDouble(&eps);
        algo_settings.epsilon = eps;
        
        algo_settings.selection_id      = genetic::SelectionMethod(selectionComboBox->GetSelection());
        algo_settings.recombination_id  = genetic::RecombinationMethod(recombinationComboBox->GetSelection());
        algo_settings.mutation_id       = genetic::MutationMethod(mutationComboBox->GetSelection());

        algo_settings.selection_strategy     = selection_map[genetic::SelectionMethod(selectionComboBox->GetSelection())];
        algo_settings.recombination_strategy = recombination_map[genetic::RecombinationMethod(recombinationComboBox->GetSelection())];
        algo_settings.mutation_strategy      = mutation_map[genetic::MutationMethod(mutationComboBox->GetSelection())];

        frames.mainFrame->RefreshPlots();

        frames.mainFrame->ContinueAlgo();

        this->Hide();
    }

}
