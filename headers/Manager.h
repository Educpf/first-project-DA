//
// Created by gabri on 19/03/2024.
//

#ifndef FIRST_PROJECT_DA_MANAGER_H
#define FIRST_PROJECT_DA_MANAGER_H

#include "Graph.h"
#include "City.h"
#include "Reservoir.h"
#include "Station.h"

class Manager {
    private:
        Graph<Element> network;
    public:
        // Loaders
        void loadReservoirs();
        void loadStations();
        void loadCities();
        void loadPipes();

        // Basic Service Metrics

        // Reliability
};


#endif //FIRST_PROJECT_DA_MANAGER_H
