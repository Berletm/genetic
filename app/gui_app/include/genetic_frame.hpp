#ifndef GENETIC_FRAME_HPP
#define GENETIC_FRAME_HPP

#include <wx-3.2/wx/frame.h>
#include <wx-3.2/wx/wx.h>

namespace genetic_gui
{
    class GeneticFrame: public wxFrame
    {
    public:
        GeneticFrame();
    private:
        void OnExit();
    };
}

#endif