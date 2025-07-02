#include "genetic_frame.hpp"

#include <wx/artprov.h>
#include <wx/graphics.h>
#include <format>

namespace genetic_gui
{
    BEGIN_EVENT_TABLE(GeneticFrame, wxFrame)
        EVT_CLOSE(GeneticFrame::OnClose)
        EVT_MENU(ID_New, GeneticFrame::New)
        EVT_MENU(ID_Save, GeneticFrame::Save)
        EVT_MENU(ID_Load, GeneticFrame::Load)
        EVT_MENU(ID_RendSettings, GeneticFrame::RendSettings)
        EVT_MENU(ID_AlgoSettings, GeneticFrame::AlgoSettings)
        EVT_BUTTON(ID_Next, GeneticFrame::Next)
        EVT_BUTTON(ID_Next, GeneticFrame::Prev)
        EVT_BUTTON(ID_Stop, GeneticFrame::Stop)
    END_EVENT_TABLE()

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

    wxBitmap CreateStopBitmap(int width, int height, const wxColour& color) 
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
            
            int squareSize = std::min(width, height);
            
            int x = (width - squareSize) / 2;
            int y = (height - squareSize) / 2;
            
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(x, y);                    
            path.AddLineToPoint(x + squareSize, y);    
            path.AddLineToPoint(x + squareSize, y + squareSize); 
            path.AddLineToPoint(x, y + squareSize);
            path.CloseSubpath();
            
            gc->SetBrush(wxBrush(color));
            gc->SetPen(wxPen(color, 1));
            gc->FillPath(path);
            
            delete gc;
        }
        
        memDC.SelectObject(wxNullBitmap);
        return bmp;
    }

    GeneticFrame::GeneticFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):  wxFrame(parent, id, title, pos, size, style)
    {        
        const wxColour textColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);

        statusbar = this->CreateStatusBar(1, wxSTB_SIZEGRIP, wxID_ANY);

        wxBitmap next_bmp = CreateNextBitmap(12, 12, textColor);
        wxBitmap prev_bmp = CreatePrevBitmap(12, 12, textColor);
        wxBitmap stop_bmp = CreateStopBitmap(12, 12, textColor);

        wxBitmapButton *btn_next = new wxBitmapButton(
                statusbar, 
                ID_Next, 
                next_bmp,
                wxDefaultPosition,
                wxDefaultSize,
                wxBORDER_NONE);

        const int btnWidth = 20;
        const int btnHeight = statusbar->GetSize().GetHeight() - 2;
        const int btnX = WIDTH - btnWidth + 80;
        const int btnY = 0;

        btn_next->SetSize(btnX, btnY, btnWidth, btnHeight);

        const int stop_btnX = btnX - btnWidth;
        
        wxBitmapButton *btn_stop = new wxBitmapButton(
                statusbar, 
                ID_Stop, 
                stop_bmp,
                wxDefaultPosition,
                wxDefaultSize,
                wxBORDER_NONE);
        btn_stop->SetSize(stop_btnX, btnY, btnWidth, btnHeight);

        wxBitmapButton *btn_prev = new wxBitmapButton(
                statusbar, 
                ID_Prev, 
                prev_bmp,
                wxDefaultPosition,
                wxDefaultSize,
                wxBORDER_NONE);

        const int prev_btnX = stop_btnX - btnWidth;

        btn_prev->SetSize(prev_btnX, btnY, btnWidth, btnHeight);

        menubar   = new wxMenuBar(0);
        
        file     = new wxMenu();
        settings = new wxMenu();
        menubar->Append(file, _("File"));
        menubar->Append(settings, _("Settings"));
        
        wxMenuItem *newfile = new wxMenuItem(file, ID_New, _("New"), _("Start new session"), wxITEM_NORMAL); 
        wxMenuItem *save    = new wxMenuItem(file, ID_Save, _("Save"), _("Save current session"), wxITEM_NORMAL);
        wxMenuItem *load    = new wxMenuItem(file, ID_Load, _("Load"), _("Load saved session"), wxITEM_NORMAL);
        file->Append(newfile);
        file->Append(save);
        file->Append(load);

        wxMenuItem *algorithm_settings = new wxMenuItem(settings, ID_AlgoSettings, _("Algorithm"), _("Algorithm settings"), wxITEM_NORMAL);
        wxMenuItem *rendering_settings = new wxMenuItem(settings, ID_RendSettings, _("Rendering"), _("Rendering settings"), wxITEM_NORMAL);
        settings->Append(rendering_settings);
        settings->Append(algorithm_settings);

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

        this->SetMenuBar(menubar);
        this->SetSizer(main_sizer);
        this->Fit();
    }

    void GeneticFrame::OnTimer(wxTimerEvent&) 
    {
        if (best_individ_history.size() > 0 && mean_fitness_history.size() > 0 && algo_settings.verbose)
        {
            std::string str = std::format("Mean fitness: {:.2f} Best fitness: {:.2f}", 
                             mean_fitness_history.back(), 
                             best_individ_history.back().fitness);
            statusbar->SetStatusText(str);
        }
        else
        {
            statusbar->SetStatusText("Algorithm is working...");
        }

        if (!controller.MakeStep()) 
        {
            timer.Stop();
            std::string str = std::format("Algorithm successfully ended! Best fitness: {:.2f}", best_individ_history.back().fitness);
            statusbar->SetStatusText(str);
        }
    }

    void GeneticFrame::StartAlgo()
    {
        controller.InitAlgo();
        controller.AddObserver(algoplot);
        controller.AddObserver(fitnessplot);

        timer.Bind(wxEVT_TIMER, &GeneticFrame::OnTimer, this);
        timer.Start(1000 / render_settings.fps);
    }
}
