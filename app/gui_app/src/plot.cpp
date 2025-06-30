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
    Plot::Plot(wxWindow *parent)
    : wxGLCanvas(
        parent, wxID_ANY, 
        settings.gl_attribs[0] == 0 ? nullptr : settings.gl_attribs,  
        wxDefaultPosition, 
        wxDefaultSize, 
        wxFULL_REPAINT_ON_RESIZE), context(this)
    {
        SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    }

    void Plot::RenderGrid()
    {
        float rangeX = settings.x_max - settings.x_min;
        float rangeY = settings.y_max - settings.y_min;

        float gridStepX = rangeX / 20.0f;
        float gridStepY = rangeY / 20.0f;
        float boldStepX = gridStepX * 5.0f;
        float boldStepY = gridStepY * 5.0f;

        glColor4f(0.85f, 0.85f, 0.85f, 0.5f);
        glLineWidth(1.0f);
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0xAAAA);

        glBegin(GL_LINES);
        for (float x = settings.x_min; x <= settings.x_max; x += gridStepX)
        {
            glVertex2f(x, settings.y_min);
            glVertex2f(x, settings.y_max);
        }
        for (float y = settings.y_min; y <= settings.y_max; y += gridStepY)
        {
            glVertex2f(settings.x_min, y);
            glVertex2f(settings.x_max, y);
        }
        glEnd();

        glDisable(GL_LINE_STIPPLE);

        glColor4f(0.6f, 0.6f, 0.6f, 1.0f);
        glLineWidth(1.5f);

        glBegin(GL_LINES);
        for (float x = settings.x_min; x <= settings.x_max; x += boldStepX)
        {
            glVertex2f(x, settings.y_min);
            glVertex2f(x, settings.y_max);
        }
        for (float y = settings.y_min; y <= settings.y_max; y += boldStepY)
        {
            glVertex2f(settings.x_min, y);
            glVertex2f(settings.x_max, y);
        }
        glEnd();

        glLineWidth(1.0f);
    }

    void Plot::RenderAxes()
    {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINES);

        if (settings.y_min <= 0 && settings.y_max >= 0)
        {
            glVertex2f(settings.x_min, 0.0f);
            glVertex2f(settings.x_max, 0.0f);
        }

        if (settings.x_min <= 0 && settings.x_max >= 0)
        {
            glVertex2f(0.0f, settings.y_min);
            glVertex2f(0.0f, settings.y_max);
        }

        glEnd();
        glLineWidth(1.0f);

        float arrow_size_x = (settings.x_max - settings.x_min) * 0.015f;
        float arrow_size_y = (settings.y_max - settings.y_min) * 0.02f;

        glBegin(GL_TRIANGLES);

        if (settings.y_min <= 0 && settings.y_max >= 0)
        {
            float y = 0.0f;
            float x = settings.x_max * x_scale_factor;
            glVertex2f(x, y);
            glVertex2f(x - arrow_size_x, y + arrow_size_y / 2);
            glVertex2f(x - arrow_size_x, y - arrow_size_y / 2);
        }

        if (settings.x_min <= 0 && settings.x_max >= 0)
        {
            float x = 0.0f;
            float y = settings.y_max;
            glVertex2f(x, y);
            glVertex2f(x - arrow_size_x / 2, y - arrow_size_y);
            glVertex2f(x + arrow_size_x / 2, y - arrow_size_y);
        }

        glEnd();
    }

    // void Plot::RenderData()
    // {
    //     double dx = (settings.x_max - settings.x_min) / settings.resolution; 
        
    //     glLineWidth(1.5f);
    //     glColor3f(1.0f, 0.0f, 0.0f);
    //     glBegin(GL_LINE_STRIP);

    //     for (double x = settings.x_min; x <= settings.x_max; x += dx)
    //     {
    //         glVertex2f(x, poly.eval(x));
    //     }

    //     glEnd();

    //     glColor3f(0.0f, 0.0f, 1.0f);
    //     glBegin(GL_LINE_STRIP);

    //     for (const auto& interval: step)
    //     {
    //         glVertex2f(interval.interval.first, interval.height);
    //         glVertex2f(interval.interval.second, interval.height);
    //     }

    //     glEnd();
    //     glLineWidth(1.0f);
    // }

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
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        double x_center = (settings.x_min + settings.x_max) / 2.0;
        double x_range = (settings.x_max - settings.x_min) * x_scale_factor;
        
        gluOrtho2D(x_center - x_range/2, x_center + x_range/2, 
                settings.y_min, settings.y_max);
        
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

    FitnessPlot::FitnessPlot(wxWindow *parent) : Plot(parent)
    {
        SetXScaleFactor(0.5);
    }

    
}
