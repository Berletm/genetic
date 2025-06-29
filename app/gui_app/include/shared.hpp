#ifndef SHARED_HPP
#define SHARED_HPP

#include <utils.hpp>
#include <wx-3.2/wx/glcanvas.h>

#define X_MAX 5
#define X_MIN -X_MAX
#define Y_MAX 5
#define Y_MIN -Y_MAX

namespace genetic_gui 
{
    struct RenderingSettings
    {
        double x_max = X_MAX, x_min = X_MIN;
        double y_max = Y_MAX, y_min = Y_MIN;
        bool   show_legend = true;
        int    resolution = 500;
        int    gl_attribs[3] = {WX_GL_SAMPLES, 4, 0};
    };

    extern RenderingSettings   settings;
    extern genetic::Polynomial poly;
    extern genetic::Individ    step;
}

#endif // SHARED_HPP 
