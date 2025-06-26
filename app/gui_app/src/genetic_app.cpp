#include "genetic_app.hpp"
#include "genetic_frame.hpp"

namespace genetic_gui
{
    bool GeneticApp::OnInit()
    {
        GeneticFrame* main_frame = new GeneticFrame();
        main_frame->Show(true);

        return true;
    }
}