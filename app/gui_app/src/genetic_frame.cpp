#include "genetic_frame.hpp"

namespace genetic_gui
{
    GeneticFrame::GeneticFrame(): wxFrame(NULL, wxID_ANY, "Genetic", wxDefaultPosition, wxSize(WIDTH, HEIGHT), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
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

        wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    
        plot_sizer = new wxBoxSizer(wxVERTICAL);
        plot_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        plot_panel->SetMinSize(wxSize(WIDTH * 0.75f, HEIGHT * 0.75f));
        plot = new Plot(plot_panel);
        plot_sizer->Add(plot, 1, wxALL|wxEXPAND, 2);
        plot_panel->SetSizer(plot_sizer);
        main_sizer->Add(plot_panel, 2, wxALL|wxEXPAND, 2);

        fitnessplot_sizer = new wxBoxSizer(wxVERTICAL);
        fitnessplot_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        fitnessplot_panel->SetMinSize(wxSize(WIDTH * 0.25f, HEIGHT * 0.25f));
        fitnessplot = new FitnessPlot(fitnessplot_panel);
        fitnessplot_sizer->Add(fitnessplot, 1, wxALL|wxEXPAND, 2);
        fitnessplot_panel->SetSizer(fitnessplot_sizer);
        main_sizer->Add(fitnessplot_panel, 1, wxALL|wxEXPAND, 2);

        this->SetMenuBar(menubar);
        this->SetSizer(main_sizer);
        this->Fit();
    }
}
