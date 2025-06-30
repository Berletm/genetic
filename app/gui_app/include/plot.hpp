#ifndef PLOT_HPP
#define PLOT_HPP

#include <wx/glcanvas.h>
#include <functional>
#include "shared.hpp"

namespace genetic_gui
{
class Plot: public wxGLCanvas
{
protected:
    wxGLContext context;

    float x_scale_factor = 1.0f; 

    void RenderGrid();
    void RenderAxes();
    virtual void RenderData() = 0;
    void Render(wxPaintEvent &evt);

    inline void SetXScaleFactor(float scale) { x_scale_factor = scale; }
    inline float GetXScaleFactor() const { return x_scale_factor; }
public:
    Plot(wxWindow *parent);

    DECLARE_EVENT_TABLE()
};

class AlgoPlot: public Plot
{
private:
    genetic::Generation init_generation;

protected:

public:
    AlgoPlot(wxWindow *parent);

    void InitAlgo();
};

class FitnessPlot: public Plot
{
protected:
    std::vector<double> fitness_history;

    void RenderData();
public:
    FitnessPlot(wxWindow *parent);
};

}
#endif // PLOT_HPP
