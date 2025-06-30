#include "genetic_frame.hpp"

#include <wx/artprov.h>
#include <wx/graphics.h>



namespace genetic_gui
{
    wxBitmap CreateNextBitmap(int width, int height, const wxColour& color) 
    {
        wxBitmap bmp(width, height, 32);
        bmp.UseAlpha();
        
        wxMemoryDC memDC(bmp);
        memDC.SetBackground(*wxTRANSPARENT_BRUSH);
        memDC.Clear();
        
        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        if (gc) 
        {
            gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
            
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(0, 0);
            path.AddLineToPoint(0, height);
            path.AddLineToPoint(width, height / 2);
            path.CloseSubpath();
            
            gc->SetBrush(wxBrush(color));
            gc->SetPen(wxPen(color, 1));
            gc->FillPath(path);
            
            delete gc;
        }
        
        memDC.SelectObject(wxNullBitmap);
        return bmp;
    }

    wxBitmap CreatePrevBitmap(int width, int height, const wxColour& color) 
    {
        wxBitmap bmp(width, height, 32);
        bmp.UseAlpha();
        
        wxMemoryDC memDC(bmp);
        memDC.SetBackground(*wxTRANSPARENT_BRUSH);
        memDC.Clear();
        
        wxGraphicsContext* gc = wxGraphicsContext::Create(memDC);
        if (gc) 
        {
            gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);
            
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(width, height);
            path.AddLineToPoint(width, 0);
            path.AddLineToPoint(0, height / 2);
            path.CloseSubpath();
            
            gc->SetBrush(wxBrush(color));
            gc->SetPen(wxPen(color, 1));
            gc->FillPath(path);
            
            delete gc;
        }
        
        memDC.SelectObject(wxNullBitmap);
        return bmp;
    }

    GeneticFrame::GeneticFrame(): wxFrame(NULL, wxID_ANY, "Genetic", wxDefaultPosition, wxSize(WIDTH, HEIGHT), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {        
        const wxColour textColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);

        statusbar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);

        wxBitmap next_bmp = CreateNextBitmap(12, 12, textColor);
        wxBitmap prev_bmp = CreatePrevBitmap(12, 12, textColor);

        wxBitmapButton *btn_next = new wxBitmapButton(
                statusbar, 
                wxID_ANY, 
                next_bmp,
                wxDefaultPosition,
                wxDefaultSize,
                wxBORDER_NONE);

        const int btnWidth = 20;
        const int btnHeight = statusbar->GetSize().GetHeight() - 2;
        const int btnX = WIDTH - btnWidth + 80;
        const int btnY = 0;

        btn_next->SetSize(btnX, btnY, btnWidth, btnHeight);

        wxBitmapButton *btn_prev = new wxBitmapButton(
                statusbar, 
                wxID_ANY, 
                prev_bmp,
                wxDefaultPosition,
                wxDefaultSize,
                wxBORDER_NONE);

        const int prev_btnX = btnX - btnWidth;

        btn_prev->SetSize(prev_btnX, btnY, btnWidth, btnHeight);

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

        controller.InitAlgo();

        wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    
        algoplot_sizer = new wxBoxSizer(wxVERTICAL);
        alogplot_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        alogplot_panel->SetMinSize(wxSize(WIDTH * 0.75f, HEIGHT * 0.75f));
        algoplot = new AlgoPlot(alogplot_panel, &controller);
        algoplot_sizer->Add(algoplot, 1, wxALL|wxEXPAND, 2);
        alogplot_panel->SetSizer(algoplot_sizer);
        main_sizer->Add(alogplot_panel, 2, wxALL|wxEXPAND, 2);

        fitnessplot_sizer = new wxBoxSizer(wxVERTICAL);
        fitnessplot_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        fitnessplot_panel->SetMinSize(wxSize(WIDTH * 0.25f, HEIGHT * 0.25f));
        fitnessplot = new FitnessPlot(fitnessplot_panel, &controller);
        fitnessplot_sizer->Add(fitnessplot, 1, wxALL|wxEXPAND, 2);
        fitnessplot_panel->SetSizer(fitnessplot_sizer);
        main_sizer->Add(fitnessplot_panel, 1, wxALL|wxEXPAND, 2);

        controller.AddObserver(algoplot);
        controller.AddObserver(fitnessplot);

        timer.Bind(wxEVT_TIMER, &GeneticFrame::OnTimer, this);
        timer.Start(100); // 10 FPS

        this->SetMenuBar(menubar);
        this->SetSizer(main_sizer);
        this->Fit();
    }

    void GeneticFrame::OnTimer(wxTimerEvent&) 
    {
        if (!controller.MakeStep()) 
        {
            timer.Stop();
            wxMessageBox("Algorithm finished!");
        }
    }
}
