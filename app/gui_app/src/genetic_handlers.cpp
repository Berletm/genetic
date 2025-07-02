#include "genetic_frame.hpp"
#include "file_frames.hpp"
#include "settings_frames.hpp"
#include <evolution.hpp>
#include <sstream>
#include <string>

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

    }

    void GeneticFrame::Load(wxCommandEvent& event)
    {

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

    }
    
    void GeneticFrame::Prev(wxCommandEvent& event)
    {

    }

    void GeneticFrame::Stop(wxCommandEvent& event)
    {
        
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
        std::istringstream iss(poly_str);
        std::string term;
        
        while (iss >> term) 
        {
            if (term == "+") continue;
            
            size_t x_pos = term.find('x');
            double coefficient = 1.0;
            double power = 0.0;
            
            if (x_pos != std::string::npos) 
            {
                std::string coeff_str = term.substr(0, x_pos);
                if (coeff_str.empty() || coeff_str == "+") 
                {
                    coefficient = 1.0;
                } 
                else if (coeff_str == "-") 
                {
                    coefficient = -1.0;
                } 
                else 
                {
                    coefficient = std::stod(coeff_str);
                }
                
                size_t caret_pos = term.find('^', x_pos);
                if (caret_pos != std::string::npos) 
                {
                    power = std::stod(term.substr(caret_pos + 1));
                } 
                else 
                {
                    power = 1.0;
                }
            } 
            else 
            {
                coefficient = std::stod(term);
                power = 0.0;
            }
            
            res.push_back({coefficient, power});
        }
        
        return res;
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
        
        algo_settings.selection_strategy = selection_map[genetic::SelectionMethod(selectionComboBox->GetSelection())];
        algo_settings.recombination_strategy = recombination_map[genetic::RecombinationMethod(recombinationComboBox->GetSelection())];
        algo_settings.mutation_strategy = mutation_map[genetic::MutationMethod(mutationComboBox->GetSelection())];

        geneticframe->StartAlgo();

        this->Hide();
    }


    void AlgoSettingsFrame::OnClose(wxCloseEvent& event)
    {
        this->Hide();
    }

    void AlgoSettingsFrame::OnApply(wxCommandEvent &event)
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

    void RendSettingsFrame::OnApply(wxCommandEvent &event)
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

        frames.mainFrame->algoplot->ApplyRenderSettings();
        frames.mainFrame->algoplot->Refresh();

        if (frames.mainFrame->timer.IsRunning())
        {
            frames.mainFrame->timer.Stop();

            int interval = (render_settings.fps > 0) ? (1000 / render_settings.fps) : 100;

            frames.mainFrame->timer.Start(interval);
        }

        this->Hide();
    }
}
