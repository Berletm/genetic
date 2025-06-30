#include "genetic_frame.hpp"

namespace genetic_gui
{
    void GeneticFrame::OnClose(wxCloseEvent& event)
    {
        frames.algoFrame->Close();
        frames.rendFrame->Close();
        this->Close();
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
}
