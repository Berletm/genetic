#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
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
        render_settings.gl_attribs[0] == 0 ? nullptr : render_settings.gl_attribs,  
        wxDefaultPosition, 
        wxDefaultSize, 
        wxFULL_REPAINT_ON_RESIZE), context(this), controller(ctr)
    {
        SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    }

    void Plot::ApplyRenderSettings()
    {
        SetCurrent(context);
        if (render_settings.gl_attribs[0] == WX_GL_SAMPLES) glEnable(GL_MULTISAMPLE);
        else glDisable(GL_MULTISAMPLE);

        this->Refresh();
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

        glVertex2f(x_min, 0.0f);
        glVertex2f(x_max, 0.0f);

        glVertex2f(0.0f, y_min);
        glVertex2f(0.0f, y_max);

        glEnd();
        glLineWidth(1.0f);

        float arrow_size_x = (x_max - x_min) * 0.015f;
        float arrow_size_y = (y_max - y_min) * 0.02f;

        glBegin(GL_TRIANGLES);

        float y_axis = 0.0f;
        float x_arrow = x_max;
        glVertex2f(x_arrow, y_axis);
        glVertex2f(x_arrow - arrow_size_x, y_axis + arrow_size_y / 2);
        glVertex2f(x_arrow - arrow_size_x, y_axis - arrow_size_y / 2);

        float x_axis = 0.0f;
        float y_arrow = y_max;
        glVertex2f(x_axis, y_arrow);
        glVertex2f(x_axis - arrow_size_x / 2, y_arrow - arrow_size_y);
        glVertex2f(x_axis + arrow_size_x / 2, y_arrow - arrow_size_y);

        glEnd();
    }

    void Plot::PrepareForRendering() 
    {
        x_min = render_settings.x_min;
        x_max = render_settings.x_max;
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
        
        gluOrtho2D(x_min, x_max, y_min, y_max);
        
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
    AlgoPlot::AlgoPlot(wxWindow *parent, GeneticController *ctr) 
        : Plot(parent, ctr)
    {
        static bool glut_initialized = false;
        if (!glut_initialized) 
        {
            int argc = 0;
            char* argv[] = { nullptr };
            glutInit(&argc, argv);
            glut_initialized = true;
        }
    }

    void AlgoPlot::RenderLegend()
    {
       if (!render_settings.show_legend) return;

        float legend_width = (x_max - x_min) * 0.25f; 
        float legend_height = (y_max - y_min) * 0.2f;
        float padding = (x_max - x_min) * 0.02f;

        float legend_x = x_max - legend_width - padding;
        float legend_y = y_max - legend_height - padding;

        glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
        glBegin(GL_QUADS);
        glVertex2f(legend_x, legend_y);
        glVertex2f(legend_x + legend_width, legend_y);
        glVertex2f(legend_x + legend_width, legend_y + legend_height);
        glVertex2f(legend_x, legend_y + legend_height);
        glEnd();

        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(legend_x, legend_y);
        glVertex2f(legend_x + legend_width, legend_y);
        glVertex2f(legend_x + legend_width, legend_y + legend_height);
        glVertex2f(legend_x, legend_y + legend_height);
        glEnd();

        float line_spacing = legend_height * 0.4f; 
        float line_length = legend_width * 0.3f;   
        float text_offset = legend_width * 0.05f; 
        float line_y_offset = legend_height * 0.75f;

        float item1_y = legend_y + line_y_offset;
        
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(legend_x + text_offset, item1_y);
        glVertex2f(legend_x + text_offset + line_length, item1_y);
        glEnd();

        float item2_y = legend_y + line_y_offset - line_spacing;
        
        glColor3f(0.0f, 0.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(legend_x + text_offset, item2_y);
        glVertex2f(legend_x + text_offset + line_length, item2_y);
        glEnd();

        glLineWidth(1.0f);
        RenderLegendText(legend_x, legend_y, legend_width, legend_height, text_offset, line_length, line_spacing, line_y_offset);
    }

    void AlgoPlot::RenderLegendText(float legend_x, float legend_y, float legend_width, float legend_height,
                                 float text_offset, float line_length, float line_spacing, float line_y_offset)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        
        float text_x_world = legend_x + text_offset + line_length + text_offset * 0.5f;
        float text_y1_world = legend_y + line_y_offset;
        float text_y2_world = legend_y + line_y_offset - line_spacing;
        
        glRasterPos2f(text_x_world, text_y1_world);
        const char* text1 = "Polynomial";
        for (const char* c = text1; *c != '\0'; c++) 
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
        
        glRasterPos2f(text_x_world, text_y2_world);
        const char* text2 = "Individual";
        for (const char* c = text2; *c != '\0'; c++) 
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    }

    void AlgoPlot::RenderData()
    {
        if (!controller) return;
        if (!controller->IsRunning()) return;
        
        glLineWidth(1.5f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);

        double dx = (x_max - x_min) / render_settings.resolution;

        for (double x = x_min; x <= x_max; x += dx) 
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
        RenderLegend();
    }

    FitnessPlot::FitnessPlot(wxWindow *parent, GeneticController* ctr)
    : Plot(parent, ctr)
    {
        SetXScaleFactor(0.5);
    }

    void FitnessPlot::PrepareForRendering() 
    {
        if (!controller || mean_fitness_history.empty()) 
        {
            x_min = 0.0;
            x_max = 10.0;
            y_min = -1.0;
            y_max = 1.0;
            x_center = 5.0;
            x_range = 10.0;
            return;
        }

        size_t last_gen = mean_fitness_history.size() - 1;
        
        double new_target_x_max = std::max(static_cast<double>(last_gen), 10.0);
        
        auto [min_it, max_it] = std::minmax_element(mean_fitness_history.begin(), mean_fitness_history.end());
        double y_data_range = *max_it - *min_it;
        
        if (y_data_range < 1e-10) {
            y_data_range = 1.0;
        }
        
        double new_target_y_min = *min_it - y_data_range * 0.1;
        double new_target_y_max = *max_it + y_data_range * 0.1;
        
        const double expansion_speed = 0.1;
        
        if (new_target_x_max > x_max || x_max == 10.0) 
        {
            x_max = x_max * (1.0 - expansion_speed) + new_target_x_max * expansion_speed;
        }
        
        y_min = y_min * (1.0 - expansion_speed) + new_target_y_min * expansion_speed;
        y_max = y_max * (1.0 - expansion_speed) + new_target_y_max * expansion_speed;

        x_min = 0.0;
        
        x_center = (x_min + x_max) / 2.0;
        x_range = (x_max - x_min);
    }

    void FitnessPlot::RenderData()
    {
        if (!controller || mean_fitness_history.empty()) return;

        glLineWidth(2.0f);
        glColor3f(0.0f, 0.5f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (size_t i = 0; i < mean_fitness_history.size(); ++i) 
        {
            glVertex2f(static_cast<float>(i), mean_fitness_history[i]);
        }
        glEnd();

        glPointSize(5.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        if (!mean_fitness_history.empty()) 
        {
            size_t last = mean_fitness_history.size() - 1;
            glVertex2f(static_cast<float>(last), mean_fitness_history[last]);
        }
        glEnd();
    }
   
    void FitnessPlot::RenderAxes()
    {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glLineWidth(3.0f);
        
        float axis_offset_x = (x_max - x_min) * 0.02f;
        float axis_offset_y = (y_max - y_min) * 0.05f;
        
        float axis_x_start = x_min + axis_offset_x;
        float axis_x_end = x_max - axis_offset_x;
        float axis_y_start = y_min + axis_offset_y;
        float axis_y_end = y_max - axis_offset_y;
        
        glBegin(GL_LINES);
        glVertex2f(axis_x_start, axis_y_start);
        glVertex2f(axis_x_end, axis_y_start);
        
        glVertex2f(axis_x_start, axis_y_start);
        glVertex2f(axis_x_start, axis_y_end);
        glEnd();
        
        double arrow_size_x = (x_max - x_min) * 0.03f;
        double arrow_size_y = (y_max - y_min) * 0.05f;
        
        arrow_size_x = std::max(arrow_size_x, (x_max - x_min) * 0.02f);
        arrow_size_y = std::max(arrow_size_y, (y_max - y_min) * 0.03f);
        
        glBegin(GL_TRIANGLES);
        
        glVertex2f(axis_x_end, axis_y_start);
        glVertex2f(axis_x_end - arrow_size_x, axis_y_start + arrow_size_y * 0.5f);
        glVertex2f(axis_x_end - arrow_size_x, axis_y_start - arrow_size_y * 0.5f);
        
        glVertex2f(axis_x_start, axis_y_end);
        glVertex2f(axis_x_start - arrow_size_x * 0.5f, axis_y_end - arrow_size_y);
        glVertex2f(axis_x_start + arrow_size_x * 0.5f, axis_y_end - arrow_size_y);
        
        glEnd();
        
        glLineWidth(1.0f);
    }

}