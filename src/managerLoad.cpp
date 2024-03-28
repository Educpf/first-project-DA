#include "../headers/Manager.h"
#include <fstream>
#include <sstream>




void Manager::loadReservoirs() {
    std::ifstream file("../dataset/Reservoir.csv");

    if(!file.is_open()){
        cout << "Error opening file Reservoir" << endl;
    } else {
        string line;
        getline(file,line);

        while(getline(file,line)){
            string name, municipality, id, code, maxDelivery;
            stringstream ss(line);

            getline(ss, name,',');
            getline(ss, municipality,',');
            getline(ss, id,',');
            getline(ss, code,',');
            getline(ss, maxDelivery,',');

            auto reservoir = new Reservoir(stoi(id),code, name, municipality, stoi(maxDelivery));

            this->reservoirs.insert({code, reservoir});
            network.addVertex(reservoir);
        }
    }
}

void Manager::loadStations(){
    std::ifstream file("../dataset/Stations.csv");

    if(!file.is_open()){
        cout << "Error opening file Stations" << endl;
    } else {
        string line;
        getline(file,line);

        while(getline(file,line)){
            string id, code;
            stringstream ss(line);

            getline(ss, id,',');
            getline(ss, code,',');

            auto station = new Station(stoi(id),code);

            this->stations.insert({code, station});
            network.addVertex(station);
        }
    }
}

void Manager::loadCities() {
    std::ifstream file("../dataset/Cities.csv");

    if(!file.is_open()){
        cout << "Error opening file Cities" << endl;
    } else {
        string line;
        getline(file,line);

        while(getline(file,line)){
            string name, id, code, demand, population;
            stringstream ss(line);

            getline(ss, name,',');
            getline(ss, id,',');
            getline(ss, code,',');
            getline(ss, demand,',');
            getline(ss, population,',');

            auto city = new City(stoi(id),code, name, stoi(demand), stoi(population));

            this->cities.insert({code, city});
            network.addVertex(city);
        }
    }
}


void Manager::loadPipes(){
    std::ifstream file("../dataset/Pipes.csv");

    if(!file.is_open()){
        cout << "Error opening file Pipes" << endl;
    } else {
        string line;
        getline(file,line);

        while(getline(file,line)){
            string source, destination, capacity, direction;
            stringstream ss(line);

            getline(ss, source,',');
            getline(ss, destination,',');
            getline(ss, capacity,',');
            getline(ss, direction,',');

            if(direction == "0") network.addBidirectionalEdge(source,destination, stoi(capacity));
            else network.addEdge(source,destination,stoi(capacity));
        }
    }
}