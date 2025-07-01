#include "settings_frames.hpp"


namespace genetic_gui
{
    AlgoSettingsFrame::AlgoSettingsFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style): 
    wxFrame(parent, id, title, pos, size, style)
    {
       this->Bind(wxEVT_CLOSE_WINDOW, &AlgoSettingsFrame::OnClose, this);
        
        wxPanel* panel = new wxPanel(this);
        
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        
        wxBoxSizer* deltaSignmaSizer = new wxBoxSizer(wxHORIZONTAL);
        
        wxStaticText* deltaLabel = new wxStaticText(panel, wxID_ANY, "Delta:");
        m_deltaCtrl = new wxTextCtrl(panel, wxID_ANY, "1.0", wxDefaultPosition, wxSize(100, -1));
        wxBoxSizer* deltaSizer = new wxBoxSizer(wxVERTICAL);
        deltaSizer->Add(deltaLabel, 0, wxALL, 5);
        deltaSizer->Add(m_deltaCtrl, 0, wxALL, 5);
        
        wxStaticText* sigmaLabel = new wxStaticText(panel, wxID_ANY, "Sigma:");
        m_sigmaCtrl = new wxTextCtrl(panel, wxID_ANY, "1.0", wxDefaultPosition, wxSize(100, -1));
        wxBoxSizer* sigmaSizer = new wxBoxSizer(wxVERTICAL);
        sigmaSizer->Add(sigmaLabel, 0, wxALL, 5);
        sigmaSizer->Add(m_sigmaCtrl, 0, wxALL, 5);
        
        deltaSignmaSizer->Add(deltaSizer, 0, wxALL, 10);
        deltaSignmaSizer->Add(sigmaSizer, 0, wxALL, 10);
        
        wxStaticText* scalingLabel = new wxStaticText(panel, wxID_ANY, "Max Scaling:");
        m_scalingCtrl = new wxTextCtrl(panel, wxID_ANY, "100.0", wxDefaultPosition, wxSize(100, -1));
        
        m_verboseCtrl = new wxCheckBox(panel, wxID_ANY, "Verbose");
        
        mainSizer->Add(deltaSignmaSizer, 0, wxALL | wxCENTER, 10);
        mainSizer->Add(scalingLabel, 0, wxALL | wxCENTER, 5);
        mainSizer->Add(m_scalingCtrl, 0, wxALL | wxCENTER, 5);
        mainSizer->Add(m_verboseCtrl, 0, wxALL | wxCENTER, 10);
        
        panel->SetSizer(mainSizer);
        
        mainSizer->Fit(this);
        SetMinSize(GetSize());
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
        
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
        
        wxStaticBoxSizer* displaySizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Display");
        displaySizer->Add(show_legend_check, 0, wxALL, 5);
        
        wxStaticBoxSizer* qualitySizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Quality");
        qualitySizer->Add(multisampling_check, 0, wxALL, 5);
        
        wxFlexGridSizer* gridSizer = new wxFlexGridSizer(2, 2, 5, 10);
        gridSizer->Add(resolutionLabel, 0, wxALIGN_CENTER_VERTICAL);
        gridSizer->Add(resolution_spin, 0, wxEXPAND);
        gridSizer->Add(fpsLabel, 0, wxALIGN_CENTER_VERTICAL);
        gridSizer->Add(fps_spin, 0, wxEXPAND);
        gridSizer->AddGrowableCol(1);
        
        qualitySizer->Add(gridSizer, 0, wxALL | wxEXPAND, 5);
        
        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        buttonSizer->Add(apply_btn, 0, wxALL, 5);
        
        mainSizer->Add(displaySizer, 0, wxALL | wxEXPAND, 10);
        mainSizer->Add(qualitySizer, 0, wxALL | wxEXPAND, 10);
        mainSizer->Add(buttonSizer, 0, wxALL | wxALIGN_CENTER, 10);
        
        panel->SetSizer(mainSizer);
        
        mainSizer->Fit(this);
        SetMinSize(GetSize());
        
        this->Bind(wxEVT_CLOSE_WINDOW, &RendSettingsFrame::OnClose, this);
    }

}
