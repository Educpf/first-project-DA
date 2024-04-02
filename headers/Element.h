#ifndef FIRST_PROJECT_DA_ELEMENT_H
#define FIRST_PROJECT_DA_ELEMENT_H

#include <string>

class Element {
    private:
        int id;
        std::string code;
    public:
        Element(int id, std::string code);
        int getId() const;
        virtual std::string getCode() const;
};

inline Element::Element(int id, std::string code)
    : id(id), code(code) {}

inline std::string Element::getCode() const {
    return code;
}

inline int Element::getId() const {
    return id;
}

#endif //FIRST_PROJECT_DA_ELEMENT_H
