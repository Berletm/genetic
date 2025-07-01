#include "file_frames.hpp"
#include "genetic_frame.hpp"
#include <wx-3.2/wx/panel.h>
#include <wx-3.2/wx/wx.h>
#include <wx/valnum.h>

namespace genetic_gui
{
    NewFrame::NewFrame(GeneticFrame* frame, wxWindow* parent, wxWindowID id, const wxString& title, 
                 const wxPoint& pos, const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
    {  
        geneticframe = frame;
        this->SetSizeHints(wxDefaultSize, wxDefaultSize);
        wxPanel* panel = new wxPanel(this, wxID_ANY);

        mainHorizontalSizer = new wxBoxSizer(wxHORIZONTAL);

        leftVerticalSizer = new wxBoxSizer(wxVERTICAL);

        leftVerticalSizer->AddSpacer(20);

        // Polynomial
        wxStaticText* poly_label = new wxStaticText(panel, wxID_ANY, "Polynomial:");
        leftVerticalSizer->Add(poly_label, 0, wxLEFT | wxALIGN_LEFT, 15);

        poly_ctrl = new wxTextCtrl(panel, wxID_ANY, "x^2", wxDefaultPosition, wxSize(350, -1));
        leftVerticalSizer->Add(poly_ctrl, 0, wxLEFT | wxALIGN_LEFT, 15);

        leftVerticalSizer->AddSpacer(15);

        // Generation size
        wxStaticText* generation_label = new wxStaticText(panel, wxID_ANY, "Generation size:");
        leftVerticalSizer->Add(generation_label, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxIntegerValidator<unsigned long> generation_val;
        generation_val.SetMin(1);
        generation_val.SetMax(2000);

        generation_size_ctrl = new wxTextCtrl(panel, wxID_ANY, "300", wxDefaultPosition, wxSize(200, -1), 0, generation_val);
        leftVerticalSizer->Add(generation_size_ctrl, 0, wxLEFT | wxALIGN_LEFT, 15);

        leftVerticalSizer->AddSpacer(10);

        // Chromosome size
        wxStaticText* chromosome_label = new wxStaticText(panel, wxID_ANY, "Chromosome size:");
        leftVerticalSizer->Add(chromosome_label, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxIntegerValidator<unsigned long> chromosome_val;
        chromosome_val.SetMin(1);
        chromosome_val.SetMax(2000);

        chromosome_size_ctrl = new wxTextCtrl(panel, wxID_ANY, "100", wxDefaultPosition, wxSize(200, -1), 0, chromosome_val);
        leftVerticalSizer->Add(chromosome_size_ctrl, 0, wxLEFT | wxALIGN_LEFT, 15);

        leftVerticalSizer->AddSpacer(10);

        // Epoch number
        wxStaticText* n_epoch_label = new wxStaticText(panel, wxID_ANY, "Epoch number:");
        leftVerticalSizer->Add(n_epoch_label, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxIntegerValidator<unsigned long> epoch_val;
        epoch_val.SetMin(1);
        epoch_val.SetMax(15000);

        epoch_ctrl = new wxTextCtrl(panel, wxID_ANY, "1000", wxDefaultPosition, wxSize(200, -1), 0, epoch_val);
        leftVerticalSizer->Add(epoch_ctrl, 0, wxLEFT | wxALIGN_LEFT, 15);

        leftVerticalSizer->AddSpacer(10);

        // Probability controls
        wxBoxSizer* probSizer = new wxBoxSizer(wxHORIZONTAL);

        // Recombination probability
        wxBoxSizer* recombSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* recomb_p_label = new wxStaticText(panel, wxID_ANY, "P(r):");
        recombSizer->Add(recomb_p_label, 0, wxLEFT | wxALIGN_LEFT, 0);

        wxFloatingPointValidator<double> recomb_p_val(3);
        recomb_p_val.SetMin(0.05);
        recomb_p_val.SetMax(1.0);

        recomb_p_ctrl = new wxTextCtrl(panel, wxID_ANY, "0.7", wxDefaultPosition, wxSize(60, -1), 0, recomb_p_val);
        recombSizer->Add(recomb_p_ctrl, 0, wxLEFT | wxALIGN_LEFT, 0);
        probSizer->Add(recombSizer, 0, wxLEFT | wxALIGN_LEFT, 15);

        probSizer->AddSpacer(10);

        // Mutation probability
        wxBoxSizer* mutSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* mutation_p_label = new wxStaticText(panel, wxID_ANY, "P(m):");
        mutSizer->Add(mutation_p_label, 0, wxLEFT | wxALIGN_LEFT, 0);

        wxFloatingPointValidator<double> mutation_p_val(3);
        mutation_p_val.SetMin(0.05);
        mutation_p_val.SetMax(1.0);

        mutation_p_ctrl = new wxTextCtrl(panel, wxID_ANY, "0.1", wxDefaultPosition, wxSize(60, -1), 0, mutation_p_val);
        mutSizer->Add(mutation_p_ctrl, 0, wxLEFT | wxALIGN_LEFT, 0);
        probSizer->Add(mutSizer, 0, wxLEFT | wxALIGN_LEFT, 0);

        probSizer->AddSpacer(10);

        // Epsilon
        wxBoxSizer* epsilonSizer = new wxBoxSizer(wxVERTICAL);
        wxStaticText* epsilon_label = new wxStaticText(panel, wxID_ANY, "Epsilon:");
        epsilonSizer->Add(epsilon_label, 0, wxLEFT | wxALIGN_LEFT, 0);

        wxFloatingPointValidator<double> epsilon_val(3);
        epsilon_val.SetMin(0.001);
        epsilon_val.SetMax(1.0);

        epsilon_ctrl = new wxTextCtrl(panel, wxID_ANY, "0.001", wxDefaultPosition, wxSize(90, -1), 0, epsilon_val);
        epsilonSizer->Add(epsilon_ctrl, 0, wxLEFT | wxALIGN_LEFT, 0);
        probSizer->Add(epsilonSizer, 0, wxLEFT | wxALIGN_LEFT, 0);

        leftVerticalSizer->Add(probSizer, 0, wxLEFT, 0);

        leftVerticalSizer->AddSpacer(30);

        // Compute button
        wxButton *compute_btn = new wxButton(panel, wxID_ANY, "Compute");
        leftVerticalSizer->Add(compute_btn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 15);
        leftVerticalSizer->AddSpacer(20);

        mainHorizontalSizer->Add(leftVerticalSizer, 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* rightVerticalSizer = new wxBoxSizer(wxVERTICAL);
        
        rightVerticalSizer->AddSpacer(20 + poly_label->GetSize().GetHeight() - 25); 
        
        // Evolution
        wxStaticText* phasesLabel = new wxStaticText(panel, wxID_ANY, "Evolution phases");
        rightVerticalSizer->Add(phasesLabel, 0, wxLEFT | wxALIGN_LEFT, 15);
        rightVerticalSizer->AddSpacer(10);

        wxStaticText* selectionLabel = new wxStaticText(panel, wxID_ANY, "Selection:");
        rightVerticalSizer->Add(selectionLabel, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxArrayString selectionChoices;
        selectionChoices.Add("Roulette");
        selectionChoices.Add("Scaling (linear)");
        selectionChoices.Add("Scaling (sigma)");
        selectionChoices.Add("Scaling (softmax)");
        selectionChoices.Add("Scaling (exponential)");

        selectionComboBox = new wxComboBox(panel, wxID_ANY, "Roulette", 
                                        wxDefaultPosition, wxSize(150, -1), 
                                        selectionChoices, wxCB_READONLY);
        rightVerticalSizer->Add(selectionComboBox, 0, wxLEFT | wxALIGN_LEFT, 15);
        rightVerticalSizer->AddSpacer(15);

        wxStaticText* recombinationLabel = new wxStaticText(panel, wxID_ANY, "Recombination:");
        rightVerticalSizer->Add(recombinationLabel, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxArrayString recombinationChoices;
        recombinationChoices.Add("Single-point");
        recombinationChoices.Add("Arithmetic");

        recombinationComboBox = new wxComboBox(panel, wxID_ANY, "Single-point", 
                                            wxDefaultPosition, wxSize(150, -1), 
                                            recombinationChoices, wxCB_READONLY);
        rightVerticalSizer->Add(recombinationComboBox, 0, wxLEFT | wxALIGN_LEFT, 15);
        rightVerticalSizer->AddSpacer(15);

        wxStaticText* mutationLabel = new wxStaticText(panel, wxID_ANY, "Mutation:");
        rightVerticalSizer->Add(mutationLabel, 0, wxLEFT | wxALIGN_LEFT, 15);

        wxArrayString mutationChoices;
        mutationChoices.Add("Swap");
        mutationChoices.Add("Gauss");
        mutationChoices.Add("Perturbation");

        mutationComboBox = new wxComboBox(panel, wxID_ANY, "Swap", 
                                        wxDefaultPosition, wxSize(150, -1), 
                                        mutationChoices, wxCB_READONLY);
        rightVerticalSizer->Add(mutationComboBox, 0, wxLEFT | wxALIGN_LEFT, 15);

        mainHorizontalSizer->Add(rightVerticalSizer, 0, wxEXPAND | wxRIGHT | wxTOP, 15);

        generation_size_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        chromosome_size_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        epoch_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        recomb_p_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        mutation_p_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        epsilon_ctrl->Bind(wxEVT_TEXT, &NewFrame::OnNumberInput, this);
        compute_btn->Bind(wxEVT_BUTTON, &NewFrame::Compute, this);

        this->Bind(wxEVT_CLOSE_WINDOW, &NewFrame::OnClose, this);

        panel->SetSizer(mainHorizontalSizer);
        this->Layout();
    }
}