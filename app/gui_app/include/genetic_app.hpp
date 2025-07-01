#ifndef GENETIC_APP
#define GENETIC_APP

#include <wx-3.2/wx/wx.h>

namespace genetic_gui
{
    class GeneticApp: public wxApp
    {
    public:
        virtual bool OnInit();
        virtual int OnExit();
    };
}

#endif // GENETIC_APP
