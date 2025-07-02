#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <generator.hpp>
#include <wx/wx.h>

namespace genetic_gui
{
    class GeneticController
    {
    private:
        std::vector<wxWindow*> observers;

        genetic::Generation current_generation;
        double mean_fitness;
        double delta_fitness;
        int current_epoch;

        bool is_running;
        
    public:
        inline void AddObserver(wxWindow* observer) {observers.push_back(observer);}
        inline void NotifyObservers() {for (auto obs : observers) {if (obs) obs->Refresh();}}

        GeneticController();

        void InitAlgo();

        inline bool IsRunning() {return this->is_running;}

        bool MakeStep();
    };
}

#endif // CONTROLLER_HPP
