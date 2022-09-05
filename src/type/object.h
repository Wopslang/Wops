/*
 * src/type/object.h
 * Wopslang Object Template Headerfile
 *
 * 2021, Wops Team
 * */

#ifndef WOPS_OBJECT_H
#define WOPS_OBJECT_H
#include "variable.h"
#include <vector>

// enum OBJECT_TYPE { SINGLE, ARRAY, OTHER }
// Enumeration of object types
enum OBJECT_TYPE {
    SINGLE,
    ARRAY,
    OTHER,
};

// class Object
// Woplsang Object Class
class Object {
    private:
    std::vector<Variable> data;

    public:
    OBJECT_TYPE type;
    String token;

    Object(String objname = "_", std::vector<Variable> objdata = {}, OBJECT_TYPE objtype = OTHER) {
        token = objname;
        data = objdata;
        type = objtype;
    }
};

#endif
