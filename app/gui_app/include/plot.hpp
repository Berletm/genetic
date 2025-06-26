#ifndef PLOT_HPP
#define PLOT_HPP

#include <wx/glcanvas.h>
#include <functional>
#include "shared.hpp"

namespace genetic_gui
{
class Plot: public wxGLCanvas
{
private:
    wxGLContext context;

    double visible_left   = settings.x_min;
    double visible_right  = settings.x_max;
    double visible_top    = GetSize().y / 2;
    double visible_bottom = -GetSize().y / 2;

    void RenderGrid();
    void RenderAxes();
    void RenderData();
    void Render(wxPaintEvent &evt);
public:
    Plot(wxWindow *parent);

    DECLARE_EVENT_TABLE()
};
}
#endif // PLOT_HPP
