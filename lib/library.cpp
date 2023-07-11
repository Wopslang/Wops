/*
 * lib/library,cpp
 * Wopslang Library DLL Sourcefile
 *
 * Announcement
 * ---
 * We'll put only I/O functions in Ver 0.1 alpha.
 * Built-in function development is on the way. Stay tuned.
 * 
 * 2021, Wops Team
 * */

#include <iostream>
#include <string>
#include <algorithm>
#include <regex>
#include "../src/type/object.h"
#include "../src/type/variable.h"

// Standard I/O Functions

/**
* @brief out([any s]): Standard output
* @name out
* @param [any s]
* @return none
*/
extern "C" Object out(Object s) {
    for (Object e: s.GetContainer()) {
        if (e.GetBase().GetValue() == "") continue;
        if (e.dim) 
            return {"_", {}, {}, {}, 0, -1, TOO_HIGH_DIMENSION};
        if (e.GetBase()._t == STRING)
            std::cout << e.GetBase().trim(e.GetBase().GetValue());
        else 
            std::cout << e.GetBase().GetValue();
    }
    return {"_", {}, {}, {}, 0, -1, OK};
}

/**
* @brief string in(): Standard input
* @name in
* @param none
* @return string s
* Terminates inputting when space or new line character is entered
*/
extern "C" Object in(Object _) { 
    std::string s; std::cin >> s;
    return {"_", {}, {}, Variable("_", "\""+s+"\"", STRING), 0, -1, OK};
}

// Type Function
/**
* @brief int toint(any s): Change datatype to integer
* @name toint
* @param any s
* @return int r
*/
extern "C" Object toint(Object s) { 
    if (s.dim != 1 || s.size[0] != 1) return {"_", {}, {}, {}, 0, -1, TOO_HIGH_DIMENSION};

    Object cont = s.GetContainer()[0];
    if (cont.dim) return {"_", {}, {}, {}, 0, -1, TOO_HIGH_DIMENSION};

    Variable e = cont.GetBase();
    if (e._t == STRING) {
        if (e.GetValue()[0] != '"'
            || e.GetValue()[e.value.length()-1] != '"'
            || e.value.length() < 2
            || std::regex_match(e.trim(e.value), std::regex("/[+-]?\\d+"))) {
                return {"_", {}, {}, {}, 0, -1, OBJECT_NOT_MATCHING_DATA};
        }
        return {"_", {}, {}, Variable("_", std::to_string(std::stoi(e.trim(e.value)))), 0, -1, OK};
    }
    return {"_", {}, {}, Variable("_", std::to_string(std::stoi(e.value)), INT), 0, -1, OK};
}

/**
* @brief string tostring(any s): Change datatype to string 
* @name tostring
* @param any s
* @return string r
*/
extern "C" Object tostring(Object s) { 
    if (s.dim != 1 || s.size[0] != 1) return {"_", {}, {}, {}, 0, -1, TOO_HIGH_DIMENSION};

    Object cont = s.GetContainer()[0];
    if (cont.dim) return {"_", {}, {}, {}, 0, -1, TOO_HIGH_DIMENSION};

    Variable e = cont.GetBase();
    return {"_", {}, {}, Variable("_", "\""+e.GetValue()+"\"", STRING), 0, -1, OK};
}