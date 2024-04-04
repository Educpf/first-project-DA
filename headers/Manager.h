#ifndef FIRST_PROJECT_DA_MANAGER_H
#define FIRST_PROJECT_DA_MANAGER_H

#include "Graph.h"
#include "City.h"
#include "Reservoir.h"
#include "Station.h"
#include <list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

class Manager {
    private:
        Graph network;
        std::unordered_map<std::string, Reservoir *> reservoirs;
        std::unordered_map<std::string, Station *> stations;
        std::unordered_map<std::string, City *> cities;
		std::unordered_map<std::string, Element *> allElements;

    public:
        // Loaders
        void loadReservoirs();
        void loadStations();
        void loadCities();
        void loadPipes();

		// Getters
		Graph getNetwork();
		std::unordered_map<std::string, Reservoir *> getReservoirs();
		std::unordered_map<std::string, Station *> getStations();
        std::unordered_map<std::string, City *> getCities();
		std::unordered_map<std::string, Element *> getAllElements();
        // Basic Service Metrics
        double CalculateMaxFlow();
        void balanceNetwork();
        // Reliability
        void removeReservoir(Reservoir* reservoir);
    
    private:
        double FarthestAugmentingPath(std::list<Edge*>& biggestPath, Vertex*& last);
        void EdmondsKarp(Element* source, Element* target, const std::unordered_set<Vertex*>& affected);
        double EdmondsBFS(Vertex* source, Vertex* target, const std::unordered_set<Vertex*>& affected);
        std::tuple<double, double, double> AnalyzeBalance();
};

inline Graph Manager::getNetwork() {
	return network;
}

inline std::unordered_map<std::string, Reservoir *> Manager::getReservoirs() {
	return reservoirs;
}

inline std::unordered_map<std::string, Station *> Manager::getStations() {
	return stations;
}

inline std::unordered_map<std::string, City *> Manager::getCities() {
	return cities;
}

inline std::unordered_map<std::string, Element *> Manager::getAllElements() {
	return allElements;
}

#endif //FIRST_PROJECT_DA_MANAGER_H
