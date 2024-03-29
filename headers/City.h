#ifndef FIRST_PROJECT_DA_CITY_H
#define FIRST_PROJECT_DA_CITY_H

#include "Element.h"
#include <string>

class City : public Element {
    private:
        std::string name;
        int demand;
        int population;
    public:
        City(int id, std::string code, std::string name, int demand, int population);
        int getDemand() const;
        int getPopulation() const;
        std::string getName() const;
};

inline int City::getDemand() const {
    return demand;
}

inline int City::getPopulation() const {
    return population;
}

inline std::string City::getName() const {
    return name;
}

inline City::City(int id, std::string code, std::string name, int demand, int population)
: Element(id, code), name(name), demand(demand), population(population) {}

#endif //FIRST_PROJECT_DA_CITY_H
