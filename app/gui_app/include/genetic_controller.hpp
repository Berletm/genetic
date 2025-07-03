#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <generator.hpp>
#include <wx/wx.h>
#include "shared.hpp"

namespace genetic_gui
{
    class GeneticController
    {
    private:
        std::vector<wxWindow*> observers;

        genetic::Generation current_generation;
        double mean_fitness;
        double delta_fitness;

        bool is_running;        
    public:
        std::vector<HistoryState> undo_history;
        std::vector<HistoryState> redo_history;
        int current_epoch;

        inline void AddObserver(wxWindow* observer) {observers.push_back(observer);}
        inline void NotifyObservers() {for (auto obs : observers) {if (obs) obs->Refresh();}}
        inline void ClearObservers()  {observers.clear();}
        inline bool CanRedo() const   {return !redo_history.empty();}

        GeneticController();

        void InitAlgo();

        inline bool IsRunning() {return this->is_running;}

        bool MakeStep();

        bool MakeStepBack();

        bool RedoStep();
    };
}

#endif // CONTROLLER_HPP
