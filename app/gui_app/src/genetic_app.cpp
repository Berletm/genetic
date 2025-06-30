#include "genetic_frame.hpp" 
#include "genetic_app.hpp"
#include "file_frames.hpp"
#include "settings_frames.hpp"
#include "shared.hpp"

namespace genetic_gui
{
    bool GeneticApp::OnInit()
    {
        frames.mainFrame = new GeneticFrame(NULL, wxID_ANY, "Genetic", wxDefaultPosition, wxSize(WIDTH, HEIGHT), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX));
        frames.algoFrame = new AlgoSettingsFrame();
        frames.rendFrame = new RendSettingsFrame();
        frames.newFrame  = new NewFrame(nullptr, ID_New, "New", wxDefaultPosition, wxDefaultSize, 0);

        frames.mainFrame->Show(true);
        frames.mainFrame->Center();

        frames.mainFrame->SetStatusText("Welcome to Genetic GUI!");
        
        return true;
    }
}
