#include "plot.hpp"

#include <GL/gl.h>
#include <wx/colour.h>
#include <wx/wx.h>

namespace genetic_gui
{
    
BEGIN_EVENT_TABLE(Plot, wxGLCanvas)
    EVT_PAINT(Plot::Render)
END_EVENT_TABLE()

    Plot::Plot(wxWindow *parent): wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE), context(this)
    {
        SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    }

    void Plot::Render(wxPaintEvent &evt)
    {
        wxPaintDC dc(this);
        SetCurrent(context);
        wxSize win_size = GetSize();
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        wxColour bg_col = *wxWHITE;
        glClearColor(bg_col.Red() / 255.f, bg_col.Green() / 255.f, bg_col.Blue() / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, win_size.x, win_size.y);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, win_size.x, win_size.y, 0, 0, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glFlush();
        SwapBuffers();
    }
}
