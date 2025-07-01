#include "genetic_frame.hpp" 
#include "genetic_app.hpp"
#include "file_frames.hpp"
#include "settings_frames.hpp"
#include "shared.hpp"

namespace genetic_gui
{
    bool GeneticApp::OnInit()
    {
        frames.mainFrame = new GeneticFrame(nullptr, ID_Main, "Genetic", wxDefaultPosition, wxSize(WIDTH, HEIGHT), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
        frames.algoFrame = new AlgoSettingsFrame(nullptr, ID_AlgoSettings, "Algorithm", wxDefaultPosition, wxDefaultSize, 0);
        frames.rendFrame = new RendSettingsFrame(nullptr, ID_RendSettings, "Rendering", wxDefaultPosition, wxDefaultSize, 0);
        frames.newFrame  = new NewFrame(frames.mainFrame, nullptr, ID_New, "New", wxDefaultPosition, wxDefaultSize, 0);

        frames.mainFrame->Show(true);
        frames.mainFrame->Center();

        frames.mainFrame->SetStatusText("Welcome to Genetic GUI!");
        
        return true;
    }

    int GeneticApp::OnExit()
    {
        return wxApp::OnExit();
    }
}
