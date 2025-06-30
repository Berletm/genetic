#include "genetic_app.hpp"
#include "genetic_frame.hpp"

namespace genetic_gui
{
    bool GeneticApp::OnInit()
    {
        GeneticFrame* mainFrame = new GeneticFrame();
        mainFrame->Show(true);
        mainFrame->Center();

        mainFrame->SetStatusText("Welcome to Genetic GUI!");
        
        return true;
    }
}
