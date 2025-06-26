#include "genetic_frame.hpp"

namespace genetic_gui
{
    GeneticFrame::GeneticFrame(): wxFrame(NULL, wxID_ANY, "Genetic", wxDefaultPosition, wxSize(640, 480), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {        
        statusbar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);
        menubar   = new wxMenuBar(0);
        
        file     = new wxMenu();
        settings = new wxMenu();
        menubar->Append(file, _("File"));
        menubar->Append(settings, _("Settings"));
        
        wxMenuItem *newfile = new wxMenuItem(file, wxID_ANY, _("New"), _("Start new session"), wxITEM_NORMAL); 
        wxMenuItem *save    = new wxMenuItem(file, wxID_ANY, _("Save"), _("Save current session"), wxITEM_NORMAL);
        wxMenuItem *load    = new wxMenuItem(file, wxID_ANY, _("Load"), _("Load saved session"), wxITEM_NORMAL);
        file->Append(newfile);
        file->Append(save);
        file->Append(load);

        wxMenuItem *algorithm_settings = new wxMenuItem(settings, wxID_ANY, _("Algorithm"), _("Algorithm settings"), wxITEM_NORMAL);
        wxMenuItem *rendering_settings = new wxMenuItem(settings, wxID_ANY, _("Rendering"), _("Rendering settings"), wxITEM_NORMAL);
        settings->Append(rendering_settings);
        settings->Append(algorithm_settings);

        plot_sizer = new wxBoxSizer(wxVERTICAL);
        plot_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        plot = new Plot(plot_panel);
        plot_sizer->Add(plot, 1, wxALL|wxEXPAND, 5);
        plot_panel->SetSizer(plot_sizer);

        this->SetMenuBar(menubar);
    }
}
