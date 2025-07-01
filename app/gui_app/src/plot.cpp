#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/colour.h>
#include <wx/wx.h>
#include <wx/sysopt.h>
#include "plot.hpp"
#include "shared.hpp"
#include <evolution.hpp>
#include <generator.hpp>

namespace genetic_gui
{
    // Plot
    Plot::Plot(wxWindow *parent, GeneticController *ctr)
    : wxGLCanvas(
        parent, wxID_ANY, 
        nullptr,  
        wxDefaultPosition, 
        wxDefaultSize, 
        wxFULL_REPAINT_ON_RESIZE), context(this), controller(ctr)
    {
        wxGLAttributes attrs;
        attrs.PlatformDefaults().EndList();
        SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    }

    void Plot::RenderGrid()
    {
        float rangeX = x_max - x_min;
        float rangeY = y_max - y_min;

        float gridStepX = rangeX / 20.0f;
        float gridStepY = rangeY / 20.0f;
        float boldStepX = gridStepX * 5.0f;
        float boldStepY = gridStepY * 5.0f;

        glColor4f(0.85f, 0.85f, 0.85f, 0.5f);
        glLineWidth(1.0f);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xAAAA);

        glBegin(GL_LINES);
        for (float x = x_min; x <= x_max; x += gridStepX)
        {
            glVertex2f(x, y_min);
            glVertex2f(x, y_max);
        }
        for (float y = y_min; y <= y_max; y += gridStepY)
        {
            glVertex2f(x_min, y);
            glVertex2f(x_max, y);
        }
        glEnd();

        glDisable(GL_LINE_STIPPLE);

        glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
        glLineWidth(1.5f);

        glBegin(GL_LINES);
        for (float x = x_min; x <= x_max; x += boldStepX)
        {
            glVertex2f(x, y_min);
            glVertex2f(x, y_max);
        }
        for (float y = y_min; y <= y_max; y += boldStepY)
        {
            glVertex2f(x_min, y);
            glVertex2f(x_max, y);
        }
        glEnd();

        glLineWidth(1.0f);
    }

    void Plot::RenderAxes()
    {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);

        if (render_settings.y_min <= 0 && render_settings.y_max >= 0)
        {
            glVertex2f(render_settings.x_min, 0.0f);
            glVertex2f(render_settings.x_max, 0.0f);
        }

        if (render_settings.x_min <= 0 && render_settings.x_max >= 0)
        {
            glVertex2f(0.0f, render_settings.y_min);
            glVertex2f(0.0f, render_settings.y_max);
        }

        glEnd();
        glLineWidth(1.0f);

        float arrow_size_x = (render_settings.x_max - render_settings.x_min) * 0.015f;
        float arrow_size_y = (render_settings.y_max - render_settings.y_min) * 0.02f;

        glBegin(GL_TRIANGLES);

        if (render_settings.y_min <= 0 && render_settings.y_max >= 0)
        {
            float y = 0.0f;
            float x = render_settings.x_max * x_scale_factor;
            glVertex2f(x, y);
            glVertex2f(x - arrow_size_x, y + arrow_size_y / 2);
            glVertex2f(x - arrow_size_x, y - arrow_size_y / 2);
        }

        if (render_settings.x_min <= 0 && render_settings.x_max >= 0)
        {
            float x = 0.0f;
            float y = render_settings.y_max;
            glVertex2f(x, y);
            glVertex2f(x - arrow_size_x / 2, y - arrow_size_y);
            glVertex2f(x + arrow_size_x / 2, y - arrow_size_y);
        }

        glEnd();
    }

    void Plot::UpdatePlotParameters(double center, double range, double ymin, double ymax) 
    {
        x_center = center;
        x_range = range;
        y_min = ymin;
        y_max = ymax;
    }

    void Plot::PrepareForRendering() 
    {
        x_center = (x_min + x_max) / 2.0;
        x_range = (x_max - x_min) * x_scale_factor;
        y_min = render_settings.y_min;
        y_max = render_settings.y_max;
    }

    void Plot::Render(wxPaintEvent &evt)
    {
        wxPaintDC dc(this);
        wxGLCanvas::SetCurrent(context);
        wxSize win_size = GetSize();
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        wxColour bg_col = *wxWHITE;
        glClearColor(bg_col.Red() / 255.f, bg_col.Green() / 255.f, bg_col.Blue() / 255.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double scale = GetContentScaleFactor();

        glViewport(0, 0, win_size.x * scale, win_size.y * scale);

        PrepareForRendering();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        gluOrtho2D(x_center - x_range/2, x_center + x_range/2, 
                y_min, y_max);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        RenderGrid();
        RenderAxes();
        RenderData();

        glFlush();
        SwapBuffers();
    }

    BEGIN_EVENT_TABLE(Plot, wxGLCanvas)
        EVT_PAINT(Plot::Render)
    END_EVENT_TABLE()

    // AlgoPlot
    AlgoPlot::AlgoPlot(wxWindow *parent, GeneticController* ctr) : Plot(parent, ctr)
    {

    }

    void AlgoPlot::RenderData()
    {
        if (!controller) return;
        
        glLineWidth(1.5f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);

        double dx = (render_settings.x_max - render_settings.x_min) / render_settings.resolution;

        for (double x = render_settings.x_min; x <= render_settings.x_max; x += dx) 
        {
            glVertex2f(x, poly.eval(x));
        }
        glEnd();
        
        if (!best_individ_history.empty()) 
        {
            glColor3f(0.0f, 0.0f, 1.0f);
            glBegin(GL_LINE_STRIP);

            for (const auto& interval: best_individ_history.back())
            {
                glVertex2f(interval.interval.first, interval.height);
                glVertex2f(interval.interval.second, interval.height);
            }

            glEnd();
            glLineWidth(1.0f);
        }
    }

    FitnessPlot::FitnessPlot(wxWindow *parent, GeneticController* ctr)
    : Plot(parent, ctr)
    {
        SetXScaleFactor(0.5);
        x_min = 0;
        x_max = 10.0;
        y_min = -1.0;
        y_max = 1.0;
    }

    void FitnessPlot::PrepareForRendering() 
    {
        if (!controller || mean_fitness_history.empty()) {
            Plot::PrepareForRendering();
            return;
        }

        size_t last_gen = mean_fitness_history.size() - 1;
        
        double new_target_x_max = std::max(static_cast<double>(last_gen), 10.0);
        
        auto [min_it, max_it] = std::minmax_element(mean_fitness_history.begin(), mean_fitness_history.end());
        double y_range = *max_it - *min_it;
        
        if (y_range < 1e-10) {
            y_range = 1.0;
            *min_it = *max_it - 0.5;
        }
        
        double new_target_y_min = *min_it - y_range * 0.2;
        double new_target_y_max = *max_it + y_range * 0.2;
        
        if (new_target_y_max - new_target_y_min < 0.1) 
        {
            new_target_y_min = -0.05;
            new_target_y_max = 0.05;
        }

        const double expansion_speed = 0.2;
        x_max = x_max * (1.0 - expansion_speed) + new_target_x_max * expansion_speed;
        y_min = y_min * (1.0 - expansion_speed) + new_target_y_min * expansion_speed;
        y_max = y_max * (1.0 - expansion_speed) + new_target_y_max * expansion_speed;

        x_min = 0;
        x_center = x_max / 2.0;
        x_range = x_max * 1.05;

        wxSize size = GetSize();
        float aspect = float(size.x)/size.y;
        float data_aspect = (x_max - x_min)/(y_max - y_min);
        
        if(data_aspect > aspect) {
            float new_height = (x_max - x_min)/aspect;
            float y_center = (y_min + y_max)/2;
            y_min = y_center - new_height/2;
            y_max = y_center + new_height/2;
        }
        else 
        {
            float new_width = (y_max - y_min)*aspect;
            float x_center = (x_min + x_max)/2;
            x_min = x_center - new_width/2;
            x_max = x_center + new_width/2;
            x_range = x_max - x_min;
        }

        
    }

    void FitnessPlot::RenderData()
    {
        if (!controller || mean_fitness_history.empty()) return;

        glLineWidth(2.0f);
        glColor3f(0.0f, 0.5f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < mean_fitness_history.size(); ++i) {
            glVertex2f(static_cast<float>(i), mean_fitness_history[i]);
        }
        glEnd();

        glPointSize(5.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        if (!mean_fitness_history.empty()) {
            size_t last = mean_fitness_history.size() - 1;
            glVertex2f(static_cast<float>(last), mean_fitness_history[last]);
        }
        glEnd();
    }

    void FitnessPlot::RenderAxes()
    {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glLineWidth(2.0f);
        
        float x_axis_y = y_min * 1.15;
        
        glBegin(GL_LINES);
        glVertex2f(x_min, x_axis_y);
        glVertex2f(x_max, x_axis_y);
        
        glVertex2f(0.0f, y_min);
        glVertex2f(0.0f, y_max);
        glEnd();
        
        float arrow_size_x = (x_max - x_min) * 0.01f;
        float arrow_size_y = (y_max - y_min) * 0.02f;
        
        glBegin(GL_TRIANGLES);
        glVertex2f(x_max, x_axis_y);
        glVertex2f(x_max - arrow_size_x, x_axis_y + arrow_size_y);
        glVertex2f(x_max - arrow_size_x, x_axis_y - arrow_size_y);
        
        glVertex2f(0.0f, y_max);
        glVertex2f(-arrow_size_x, y_max - arrow_size_y);
        glVertex2f(arrow_size_x, y_max - arrow_size_y);
        glEnd();
        
        glLineWidth(1.0f);
    }

}
