#ifndef GENETIC_FRAME_HPP
#define GENETIC_FRAME_HPP

#include <wx-3.2/wx/frame.h>
#include <wx-3.2/wx/wx.h>
#include "plot.hpp"
#include "genetic_controller.hpp"

#define WIDTH  640
#define HEIGHT 480

namespace genetic_gui
{
    class GeneticFrame: public wxFrame
    {
    public:
        GeneticFrame();
    private:
        wxStatusBar *statusbar;
        wxMenuBar   *menubar;
        wxMenu      *file;
        wxMenu      *settings;

        GeneticController controller;

        wxBoxSizer  *algoplot_sizer;
        wxPanel     *alogplot_panel;
        Plot        *algoplot;

        wxBoxSizer  *fitnessplot_sizer;
        wxPanel     *fitnessplot_panel;
        Plot        *fitnessplot;

        wxTimer timer;

        void OnTimer(wxTimerEvent&);

        void OnExit();
    };
}

#endif // GENETIC_FRAME_HPP
