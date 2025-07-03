#ifndef FILE_FRAMES_HPP
#define FILE_FRAMES_HPP

#include <wx-3.2/wx/frame.h>
#include <wx-3.2/wx/textctrl.h>
#include <wx-3.2/wx/sizer.h>
#include <wx-3.2/wx/combobox.h>
#include <utils.hpp>

namespace genetic_gui
{
    class GeneticFrame;

    class NewFrame : public wxFrame
    {
    private:
        // event handlers
        void RefreshPlots();
        void OnCompute(wxCommandEvent& event);
        void OnNumberInput(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);

        void ParseTerm(const std::string& term, genetic::Polynomial& poly);
        double ParseCoefficient(const std::string& coeff_str);
        genetic::Polynomial ParsePoly(const std::string& poly_str);
    public:
        NewFrame(GeneticFrame* frame, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    private:
        GeneticFrame *geneticframe;

        wxBoxSizer *mainHorizontalSizer;
        wxBoxSizer *leftVerticalSizer;

        wxTextCtrl *generation_size_ctrl;
        wxTextCtrl *chromosome_size_ctrl;
        wxTextCtrl *poly_ctrl;
        wxTextCtrl *epoch_ctrl;
        wxTextCtrl *recomb_p_ctrl;
        wxTextCtrl *mutation_p_ctrl;
        wxTextCtrl *epsilon_ctrl;

        wxTextCtrl* interval_left_ctrl;
        wxTextCtrl* interval_right_ctrl;

        wxComboBox *selectionComboBox;
        wxComboBox *recombinationComboBox;
        wxComboBox *mutationComboBox;
    };
        

}

#endif // FILE_FRAMES_HPP
