#ifndef FIRST_PROJECT_DA_ELEMENT_H
#define FIRST_PROJECT_DA_ELEMENT_H

#include <string>

class Element {
    private:
        int id;
        std::string code;
    public:
        Element(int id, std::string code);
        int getId();
        std::string getCode();
};

Element::Element(int id, std::string code)
    : id(id), code(code) {}

std::string Element::getCode() {
    return code;
}

int Element::getId() {
    return id;
}


#endif //FIRST_PROJECT_DA_ELEMENT_H
