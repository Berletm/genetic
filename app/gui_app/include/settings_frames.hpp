#ifndef SETTINGS_FRAMES_HPP
#define SETTINGS_FRAMES_HPP

#include <wx-3.2/wx/frame.h>
#include <wx-3.2/wx/wx.h>
#include <wx-3.2/wx/spinctrl.h>

namespace genetic_gui
{

    class AlgoSettingsFrame : public wxFrame
    {
    private:
        void OnClose(wxCloseEvent &event);
        void OnApply(wxCommandEvent &event);
    private:
        wxTextCtrl *delta_ctrl;
        wxTextCtrl *sigma_ctrl;
        wxTextCtrl *m_scalingCtrl;
        wxCheckBox *m_verboseCtrl;

    public:
        AlgoSettingsFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style);
    };

    class RendSettingsFrame : public wxFrame
    {
    private:
        void OnClose(wxCloseEvent &event);

    private:
        wxCheckBox *show_legend_check;
        wxCheckBox *multisampling_check;
        wxSpinCtrl *resolution_spin;
        wxSpinCtrl *fps_spin;
        wxButton   *apply_btn;

    public:
        RendSettingsFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style);
    };

}

#endif // SETTINGS_FRAMES_HPP
