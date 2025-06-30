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

    float x_scale_factor = 1.0f; 

    void RenderGrid();
    void RenderAxes();
    void RenderData();
    void Render(wxPaintEvent &evt);

protected:
    inline void SetXScaleFactor(float scale) { x_scale_factor = scale; }
    inline float GetXScaleFactor() const { return x_scale_factor; }
public:
    Plot(wxWindow *parent);

    DECLARE_EVENT_TABLE()
};

class FitnessPlot: public Plot
{
private:
    void RenderData();
public:
    FitnessPlot(wxWindow *parent);
};

}
#endif // PLOT_HPP
