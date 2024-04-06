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
		bool useSmallSet;

    public:
		int totalNetworkFlow;
		std::unordered_map<std::string, int> maxFlows;
        std::unordered_map<std::string, int> deficitcities;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> rmPS;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> rmPipelines;

		Graph network;
        std::unordered_map<std::string, Reservoir *> reservoirs;
        std::unordered_map<std::string, Station *> stations;
        std::unordered_map<std::string, City *> cities;
		std::unordered_map<std::string, Element *> allElements;

		inline Manager(bool useSmallSet) : useSmallSet(useSmallSet) {};
		~Manager();

        // Loaders
        void loadReservoirs();
        void loadStations();
        void loadCities();
        void loadPipes();

        // Basic Service Metrics
        double CalculateMaxFlow();
        void maxFlowCities();
        void citiesInDeficit();
        void balanceNetwork();

        // Reliability
        void removeReservoir(Reservoir* reservoir);
        void maintenancePS();
        void maintenancePipes();

    private:
        double FarthestAugmentingPath(std::list<Edge*>& biggestPath, Vertex*& last);
        void EdmondsKarp(Element* source, Element* target, const std::unordered_set<Vertex*>& affected);
        double EdmondsBFS(Vertex* source, Vertex* target, const std::unordered_set<Vertex*>& affected);
        std::tuple<double, double, double> AnalyzeBalance();
};

inline Manager::~Manager()
{
	for (const auto& [code, e] : allElements)
		delete e;
}

#endif //FIRST_PROJECT_DA_MANAGER_H
