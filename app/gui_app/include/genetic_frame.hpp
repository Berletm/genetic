#ifndef GENETIC_FRAME_HPP
#define GENETIC_FRAME_HPP

#include <wx-3.2/wx/frame.h>
#include <wx-3.2/wx/wx.h>
#include "plot.hpp"

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

        wxBoxSizer  *plot_sizer;
        wxPanel     *plot_panel;
        Plot        *plot;


        void OnExit();
    };
}

#endif // GENETIC_FRAME_HPP
