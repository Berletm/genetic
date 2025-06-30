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
    DECLARE_EVENT_TABLE()
    public:
        GeneticFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    protected:
        // event handlers
        void OnClose(wxCloseEvent& event);
        void New(wxCommandEvent& event);
        void Save(wxCommandEvent& event);
        void Load(wxCommandEvent& event);
        void RendSettings(wxCommandEvent& event);
        void AlgoSettings(wxCommandEvent& event);
        void Prev(wxCommandEvent& event);
        void Next(wxCommandEvent& event);
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
    };
}

#endif // GENETIC_FRAME_HPP
