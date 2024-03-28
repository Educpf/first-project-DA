#ifndef FIRST_PROJECT_DA_RESERVOIR_H
#define FIRST_PROJECT_DA_RESERVOIR_H

#include <string>
#include "Element.h"

class Reservoir : public Element {
    private:
        std::string name;
        std::string municipality;
        int maxDelivery = 0;
    public:
        Reservoir(int id, std::string code, std::string name, std::string municipality, int maxDelivery);
        std::string getName() const;
        std::string getMunicipality() const;
        int getMaxDelivery() const;
};


Reservoir::Reservoir(int id, std::string code, std::string name, std::string municipality, int maxDelivery)
    : Element(id, code), name(name), municipality(municipality), maxDelivery(maxDelivery) {}

std::string Reservoir::getName() const {
    return name;
}

std::string Reservoir::getMunicipality() const {
    return municipality;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery;
}


#endif //FIRST_PROJECT_DA_RESERVOIR_H
