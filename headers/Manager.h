#ifndef FIRST_PROJECT_DA_MANAGER_H
#define FIRST_PROJECT_DA_MANAGER_H

#include "Graph.h"
#include "City.h"
#include "Reservoir.h"
#include "Station.h"
#include <list>
#include <tuple>

#include <unordered_map>
using namespace std;

class Manager {
    private:
        Graph network;
        unordered_map<string, Reservoir *> reservoirs;
        unordered_map<string, Station *> stations;
        unordered_map<string, City *> cities;
		unordered_map<string, Element *> allElements;

    public:
        // Loaders
        void loadReservoirs();
        void loadStations();
        void loadCities();
        void loadPipes();

		// Getters
		Graph getNetwork();
		unordered_map<string, Reservoir *> getReservoirs();
		unordered_map<string, Station *> getStations();
        unordered_map<string, City *> getCities();
		unordered_map<string, Element *> getAllElements();
        // Basic Service Metrics
        double CalculateMaxFlow();
        void balanceNetwork();
        // Reliability
        void removeReservoir(Reservoir* reservoir);
    
    private:
        double FarthestAugmentingPath(list<Edge*>& biggestPath, Vertex*& last);
        void EdmondsKarp(Element* source, Element* target, const unordered_set<Vertex*>& affected);
        double EdmondsBFS(Vertex* source, Vertex* target, const unordered_set<Vertex*>& affected);
        tuple<double, double, double> AnalyzeBalance();
};

inline Graph Manager::getNetwork() {
	return network;
}

inline unordered_map<string, Reservoir *> Manager::getReservoirs() {
	return reservoirs;
}

inline unordered_map<string, Station *> Manager::getStations() {
	return stations;
}

inline unordered_map<string, City *> Manager::getCities() {
	return cities;
}

inline unordered_map<string, Element *> Manager::getAllElements() {
	return allElements;
}


#endif //FIRST_PROJECT_DA_MANAGER_H
