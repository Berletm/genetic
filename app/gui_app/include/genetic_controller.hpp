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
        double delta_fitness = std::numeric_limits<double>::infinity();
        int current_epoch = 0;
        
    public:
        inline void AddObserver(wxWindow* observer) {observers.push_back(observer);}
        inline void NotifyObservers() {for (auto obs : observers) obs->Refresh();}

        GeneticController();

        void InitAlgo();

        bool MakeStep();
    };
}

#endif // CONTROLLER_HPP
