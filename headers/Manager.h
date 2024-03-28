#ifndef FIRST_PROJECT_DA_MANAGER_H
#define FIRST_PROJECT_DA_MANAGER_H

#include "Graph.h"
#include "City.h"
#include "Reservoir.h"
#include "Station.h"

#include <unordered_map>
using namespace std;

class Manager {
    private:
        Graph network;
        unordered_map<string, Reservoir *> reservoirs;
        unordered_map<string, Station *> stations;
        unordered_map<string, City *> cities;

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
