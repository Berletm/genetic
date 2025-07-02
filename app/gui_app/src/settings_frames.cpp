#include "settings_frames.hpp"


namespace genetic_gui
{
    AlgoSettingsFrame::AlgoSettingsFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style): 
    wxFrame(parent, id, title, pos, size, style)
    {        
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
        
        wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
        main_sizer->AddSpacer(10);
        
        wxBoxSizer* delta_sigma_sizer = new wxBoxSizer(wxHORIZONTAL);
        
        wxStaticText* delta_label = new wxStaticText(panel, wxID_ANY, "Delta:");
        delta_ctrl = new wxTextCtrl(panel, wxID_ANY, "1.0", wxDefaultPosition, wxSize(80, 25));
        wxBoxSizer* delta_sizer = new wxBoxSizer(wxVERTICAL);
        delta_sizer->Add(delta_label, 0, wxALL | wxALIGN_LEFT, 2);
        delta_sizer->Add(delta_ctrl, 0, wxALL | wxALIGN_LEFT, 2);
        
        wxStaticText* sigma_label = new wxStaticText(panel, wxID_ANY, "Sigma:");
        sigma_ctrl = new wxTextCtrl(panel, wxID_ANY, "1.0", wxDefaultPosition, wxSize(80, 25));
        wxBoxSizer* sigma_sizer = new wxBoxSizer(wxVERTICAL);
        sigma_sizer->Add(sigma_label, 0, wxALL | wxALIGN_LEFT, 2);
        sigma_sizer->Add(sigma_ctrl, 0, wxALL | wxALIGN_LEFT, 2);
        
        delta_sigma_sizer->Add(delta_sizer, 0, wxALL, 15);
        delta_sigma_sizer->Add(sigma_sizer, 0, wxALL, 15);
        
        wxStaticText* scalingLabel = new wxStaticText(panel, wxID_ANY, "Max Scaling:");
        m_scalingCtrl = new wxTextCtrl(panel, wxID_ANY, "100.0", wxDefaultPosition, wxSize(80, 25));
        
        m_verboseCtrl = new wxCheckBox(panel, wxID_ANY, "Verbose");
        
        wxButton* apply_btn = new wxButton(panel, wxID_APPLY, "Apply");

        main_sizer->Add(delta_sigma_sizer, 0, wxALL | wxCENTER, 10);
        main_sizer->Add(scalingLabel, 0, wxALL | wxCENTER, 5);
        main_sizer->Add(m_scalingCtrl, 0, wxALL | wxCENTER, 5);
        main_sizer->Add(m_verboseCtrl, 0, wxALL | wxCENTER, 10);
        main_sizer->AddSpacer(15);
        main_sizer->Add(apply_btn, 0, wxALL | wxCENTER, 10);
        main_sizer->AddSpacer(10);
        
        panel->SetSizer(main_sizer);
        
        wxBoxSizer* frame_sizer = new wxBoxSizer(wxVERTICAL);
        frame_sizer->Add(panel, 1, wxEXPAND | wxALL, 0);
        
        frame_sizer->Fit(this);
        SetMinSize(GetSize());

        this->Bind(wxEVT_CLOSE_WINDOW, &AlgoSettingsFrame::OnClose, this);
        apply_btn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AlgoSettingsFrame::OnApply, this);
    }

    RendSettingsFrame::RendSettingsFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, style)
    {
        wxPanel* panel = new wxPanel(this);
        
        show_legend_check = new wxCheckBox(panel, wxID_ANY, "Show legend");
        multisampling_check = new wxCheckBox(panel, wxID_ANY, "Multisampling");
        
        wxStaticText* resolutionLabel = new wxStaticText(panel, wxID_ANY, "Resolution:");
        resolution_spin = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString, 
                                        wxDefaultPosition, wxDefaultSize,
                                        wxSP_ARROW_KEYS, 100, 1500, 500);
        
        wxStaticText* fpsLabel = new wxStaticText(panel, wxID_ANY, "FPS:");
        fps_spin = new wxSpinCtrl(panel, wxID_ANY, wxEmptyString,
                                wxDefaultPosition, wxDefaultSize,
                                wxSP_ARROW_KEYS, 50, 240, 100);
        
        apply_btn = new wxButton(panel, wxID_OK, "Apply");
        
        wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
        
        wxStaticBoxSizer* display_sizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Display");
        display_sizer->Add(show_legend_check, 0, wxALL, 5);
        
        wxStaticBoxSizer* quality_sizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Quality");
        quality_sizer->Add(multisampling_check, 0, wxALL, 5);
        
        wxFlexGridSizer* grid_sizer = new wxFlexGridSizer(2, 2, 5, 10);
        grid_sizer->Add(resolutionLabel, 0, wxALIGN_CENTER_VERTICAL);
        grid_sizer->Add(resolution_spin, 0, wxEXPAND);
        grid_sizer->Add(fpsLabel, 0, wxALIGN_CENTER_VERTICAL);
        grid_sizer->Add(fps_spin, 0, wxEXPAND);
        grid_sizer->AddGrowableCol(1);
        
        quality_sizer->Add(grid_sizer, 0, wxALL | wxEXPAND, 5);
        
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(apply_btn, 0, wxALL, 5);
        
        main_sizer->Add(display_sizer, 0, wxALL | wxEXPAND, 10);
        main_sizer->Add(quality_sizer, 0, wxALL | wxEXPAND, 10);
        main_sizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        
        panel->SetSizer(main_sizer);
        
        main_sizer->Fit(this);
        SetMinSize(GetSize());
        
        this->Bind(wxEVT_CLOSE_WINDOW, &RendSettingsFrame::OnClose, this);
    }

}
