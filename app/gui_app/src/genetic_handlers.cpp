#include "genetic_frame.hpp"
#include "file_frames.hpp"
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
        
    }

    void GeneticFrame::AlgoSettings(wxCommandEvent& event)
    {

    }

    void GeneticFrame::Next(wxCommandEvent& event)
    {

    }
    
    void GeneticFrame::Prev(wxCommandEvent& event)
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

    void NewFrame::Compute(wxCommandEvent& event)
    {
        auto poly_temp = poly_ctrl->GetValue().ToStdString();

        poly = ParsePoly(poly_temp);
        
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
}
