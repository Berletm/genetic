#ifndef FILE_FRAMES_HPP
#define FILE_FRAMES_HPP

#include <wx-3.2/wx/frame.h>

namespace genetic_gui
{
    class NewFrame : public wxFrame
    {
    private:
    public:
        NewFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    };
        

}

#endif // FILE_FRAMES_HPP
