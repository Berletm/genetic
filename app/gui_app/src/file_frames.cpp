#include "file_frames.hpp"

namespace genetic_gui
{
    NewFrame::NewFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style): wxFrame(parent, id, title, pos, size, style)
    {
        this->SetSizeHints( wxDefaultSize, wxDefaultSize );
        this->Layout();
    }

}