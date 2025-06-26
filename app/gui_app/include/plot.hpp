#ifndef PLOT_HPP
#define PLOT_HPP

#include <wx/glcanvas.h>

namespace genetic_gui
{
class Plot: public wxGLCanvas
{
private:
    wxGLContext context;

    void RenderGrid();
    void RenderAxes();
    void RenderData();
    void Render(wxPaintEvent &evt);
public:
    Plot(wxWindow *parent);

    DECLARE_EVENT_TABLE()
};
}
#endif
