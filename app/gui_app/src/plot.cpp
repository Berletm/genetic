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
#include <sstream>
#include <iomanip>

namespace genetic_gui
{
    double GetNiceStep(double range)
    {
        double rough_step = range / 10.0;
        double exponent = std::floor(std::log10(rough_step));
        double factor = rough_step / std::pow(10, exponent);

        if (factor < 1.9) return 1.0 * std::pow(10, exponent);
        else if (factor < 4.5) return 2.0 * std::pow(10, exponent);
        else if (factor < 9.0) return 5.0 * std::pow(10, exponent);
        else return 10.0 * std::pow(10, exponent);
    }

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

    void Plot::RenderText(const std::string& text, float x, float y, bool center_x, bool center_y) 
    {
        float text_width = 0;
        float text_height = 18;
        
        for (char c : text) {
            text_width += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, c);
        }
        
        float scale_x = (x_max - x_min) / GetSize().x;
        float scale_y = (y_max - y_min) / GetSize().y;
        
        text_width *= scale_x;
        text_height *= scale_y;
        
        if (center_x) x -= text_width / 2;
        if (center_y) y -= text_height / 2;
        
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
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

        float axis_y_pos = 0.0f;
        if (0.0f < y_min) axis_y_pos = y_min;
        else if (0.0f > y_max) axis_y_pos = y_max;
        
        glVertex2f(x_min, axis_y_pos);
        glVertex2f(x_max, axis_y_pos);

        float axis_x_pos = 0.0f;
        if (0.0f < x_min) axis_x_pos = x_min;
        else if (0.0f > x_max) axis_x_pos = x_max;

        glVertex2f(axis_x_pos, y_min);
        glVertex2f(axis_x_pos, y_max);

        glEnd();
        glLineWidth(1.0f);

        float arrow_size_x = (x_max - x_min) * 0.015f;
        float arrow_size_y = (y_max - y_min) * 0.02f;

        glBegin(GL_TRIANGLES);

        glVertex2f(x_max, axis_y_pos);
        glVertex2f(x_max - arrow_size_x, axis_y_pos + arrow_size_y / 2);
        glVertex2f(x_max - arrow_size_x, axis_y_pos - arrow_size_y / 2);

        glVertex2f(axis_x_pos, y_max);
        glVertex2f(axis_x_pos - arrow_size_x / 2, y_max - arrow_size_y);
        glVertex2f(axis_x_pos + arrow_size_x / 2, y_max - arrow_size_y);

        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);

        double step_x = GetNiceStep(x_max - x_min);
        int precision_x = 0;
        if (step_x < 1.0) {
            precision_x = std::ceil(-std::log10(step_x));
        }

        for (double x = std::ceil(x_min / step_x) * step_x; x <= x_max; x += step_x)
        {
            if (std::abs(x) < 1e-6) continue;

            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision_x) << x;
            RenderText(ss.str(), x, axis_y_pos - arrow_size_y * 1.5f, true, true); // Center X, bottom Y
        }

        double step_y = GetNiceStep(y_max - y_min);
        int precision_y = 0;
        if (step_y < 1.0) {
            precision_y = std::ceil(-std::log10(step_y));
        }

        for (double y = std::ceil(y_min / step_y) * step_y; y <= y_max; y += step_y)
        {
            if (std::abs(y) < 1e-6) continue; 

            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision_y) << y;
            RenderText(ss.str(), axis_x_pos - arrow_size_x * 2.0f, y, true, false); // Right-align X, center Y
        }
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
        
        glColor3f(0.0f, 0.0f, 0.0f);

        double step_x = GetNiceStep(x_max - x_min);
        int precision_x = 0;

        for (double x = std::ceil(axis_x_start / step_x) * step_x; x <= axis_x_end; x += step_x)
        {
            if (std::abs(x - axis_x_start) < 1e-6) continue;

            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision_x) << x;
            RenderText(ss.str(), x, axis_y_start + arrow_size_y * 0.8f, true, true); 
        }

        double step_y = GetNiceStep(y_max - y_min);
        int precision_y = 0;
        if (step_y < 1.0) {
            precision_y = std::ceil(-std::log10(step_y));
        } else if (step_y > 1.0 && std::fmod(step_y, 1.0) != 0.0) {
            precision_y = 1;
        }

        double start_y_label = std::ceil(axis_y_start / step_y) * step_y;
        if (std::abs(start_y_label - axis_y_start) < 1e-6) 
        { 
            start_y_label += step_y;
        }


        for (double y = start_y_label; y <= axis_y_end; y += step_y)
        {
            std::stringstream ss;
            ss << std::fixed << std::setprecision(precision_y) << y;

            RenderText(ss.str(), axis_x_start + arrow_size_x * 3.0f, y, true, false); 
        }
    }

}