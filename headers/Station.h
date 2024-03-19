#ifndef FIRST_PROJECT_DA_STATION_H
#define FIRST_PROJECT_DA_STATION_H

#include "Element.h"

class Station : public Element {
    public:
        Station(int id, std::string code) : Element(id, code) {};
};

#endif //FIRST_PROJECT_DA_STATION_H
