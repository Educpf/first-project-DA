#include "../headers/Manager.h"
#include <fstream>
#include <sstream>
using namespace std;

void Manager::loadReservoirs()
{
    std::ifstream file(useSmallSet ? "../dataset/small/Reservoirs_Madeira.csv" : "../dataset/Reservoir.csv");

    if(!file.is_open())
	{
        cout << "Error opening file Reservoir" << endl;
    }
	else
	{
        string line;
        getline(file,line);

        while(getline(file,line))
		{
            string name, municipality, id, code, maxDelivery;
            stringstream ss(line);

            getline(ss, name, ',');
            getline(ss, municipality, ',');
            getline(ss, id, ',');
            getline(ss, code, ',');
            getline(ss, maxDelivery, '\r');

            auto reservoir = new Reservoir(stoi(id),code, name, municipality, stoi(maxDelivery));

            reservoirs[code] = reservoir;
			allElements[code] = reservoir;
            network.addVertex(reservoir);
        }
    }
}

void Manager::loadStations()
{
    std::ifstream file(useSmallSet ? "../dataset/small/Stations_Madeira.csv" : "../dataset/Stations.csv");

    if(!file.is_open())
	{
        cout << "Error opening file Stations" << endl;
    }
	else
	{
        string line;
        getline(file,line);

        while(getline(file,line))
		{
            string id, code;
            stringstream ss(line);

            getline(ss, id, ',');
            getline(ss, code, '\r');

            auto station = new Station(stoi(id),code);

            stations[code] = station;
			allElements[code] = station;
            network.addVertex(station);
        }
    }
}

void Manager::loadCities()
{
    std::ifstream file(useSmallSet ? "../dataset/small/Cities_Madeira.csv" : "../dataset/Cities.csv");

    if(!file.is_open())
	{
        cout << "Error opening file Cities" << endl;
    }
	else
	{
        string line;
        getline(file,line);

        while(getline(file,line))
		{
            string name, id, code, demand, population;
            stringstream ss(line);

            getline(ss, name, ',');
            getline(ss, id, ',');
            getline(ss, code, ',');
            getline(ss, demand, ',');
            getline(ss, population, '\r');

            auto city = new City(stoi(id),code, name, stoi(demand), stoi(population));

            cities[code] = city;
			allElements[code] = city;
            network.addVertex(city);
        }
    }
}


void Manager::loadPipes()
{
    std::ifstream file(useSmallSet ? "../dataset/small/Pipes_Madeira.csv" : "../dataset/Pipes.csv");

    if(!file.is_open())
	{
        cout << "Error opening file Pipes" << endl;
    }
	else
	{
        string line;
        getline(file, line);

        while(getline(file, line))
		{
            string source, destination, capacity, direction;
            stringstream ss(line);

            getline(ss, source, ',');
            getline(ss, destination, ',');
            getline(ss, capacity, ',');
            getline(ss, direction, '\r');

            if (direction == "0") 
				network.addBidirectionalEdge(allElements[source], allElements[destination], stoi(capacity));
            else
				network.addEdge(allElements[source], allElements[destination], stoi(capacity));
        }
    }
}