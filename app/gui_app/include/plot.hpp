#ifndef PLOT_HPP
#define PLOT_HPP

#include <wx/glcanvas.h>
#include <functional>
#include "genetic_controller.hpp"
#include "shared.hpp"

namespace genetic_gui
{
class Plot: public wxGLCanvas
{
protected:
    wxGLContext context;
    GeneticController *controller;
    
    float x_scale_factor = 1.0f; 

    double x_min = render_settings.x_min, x_max = render_settings.x_max;
    double y_min = render_settings.y_min, y_max = render_settings.y_max;

    double x_center = (x_min + x_max) / 2.0;
    double x_range = (x_max - x_min) * x_scale_factor;

    void RenderText(const std::string& text, float x, float y, bool center_x, bool center_y);

    virtual void PrepareForRendering();
    virtual void RenderGrid();
    virtual void RenderAxes();
    virtual void RenderData() = 0;
    void Render(wxPaintEvent &evt);

    inline void  SetXScaleFactor(float scale) { x_scale_factor = scale; }
    inline float GetXScaleFactor() const { return x_scale_factor; }
public:
    inline void SetController(GeneticController* ctrl) { controller = ctrl; }
    inline GeneticController* GetController() const { return controller; }

    void ApplyRenderSettings();

    Plot(wxWindow *parent, GeneticController *ctrl);

    DECLARE_EVENT_TABLE()
};

class AlgoPlot: public Plot
{
protected:
    void RenderData() override;
    void RenderLegend();
    void RenderText(float x, float y, const char* text);
    void RenderLegendText(float legend_x, float legend_y, float legend_width, float legend_height, float text_offset, float line_length, float line_spacing, float line_y_offset);
public:
    AlgoPlot(wxWindow *parent, GeneticController* ctrl);
};

class FitnessPlot: public Plot
{
protected:
    double target_x_max = 1000.0;
    double target_y_min = -1.0;
    double target_y_max = 100.0;
    const double expansion_speed = 0.5;

    void PrepareForRendering() override;
    void RenderData() override;
    void RenderAxes() override;    
public:
    FitnessPlot(wxWindow *parent, GeneticController* ctr);
};

}
#endif // PLOT_HPP
