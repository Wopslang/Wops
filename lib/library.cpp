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
#include "../src/type/array.h"
#include "../src/type/variable.h"

// Standard I/O Functions

/**
* @brief out([any s]): Standard output
* @name out
* @param [any s]
* @return none
*/
extern "C" ArrayWithCode out(Array s) {
    // EMPTY VARIABLE
    Array null(Variable("_", "", INT));
    for (Variable e: s.container) {
        if (e.GetValue() == "") continue;
        if (e._t == STRING)
            std::cout << e.trim(e.GetValue());
        else 
            std::cout << e.GetValue();
    }
    return {null, OK};
}

/**
* @brief string in(): Standard input
* @name in
* @param none
* @return string s
* Terminates inputting when space or new line character is entered
*/
extern "C" ArrayWithCode in(Array _) { 
    std::string s; std::cin >> s;
    Array ret(Variable("_", "\""+s+"\"", STRING));
    return {ret, OK};
}

// Type Function
/**
* @brief int toint(any s): Change datatype to integer
* @name toint
* @param any s
* @return int r
*/
extern "C" ArrayWithCode toint(Array s) { 
    Array ret;
    if (s.container.size() != 1) return {ret, ERROR};

    Variable e = s.container[0];
    if (e._t == STRING) {
        if (e.GetValue()[0] != '"'
            || e.GetValue()[e.value.length()-1] != '"'
            || e.value.length() < 2
            || std::regex_match(e.trim(e.value), std::regex("/[+-]?\\d+"))) {
                return {ret, ERROR};
        }
        ret.container.push_back(Variable("_", std::to_string(std::stoi(e.trim(e.value))), INT));
        return {ret, OK};
    }
    ret.container.push_back(Variable("_", std::to_string(std::stoi(e.value)), INT));
    return {ret, OK};
}

/**
* @brief string tostring(any s): Change datatype to string 
* @name tostring
* @param any s
* @return string r
*/
extern "C" ArrayWithCode tostring(Array s) { 
    Array ret;
    if (s.container.size() != 1) return {ret, ERROR};

    Variable e = s.container[0];
    ret.container.push_back(Variable("_", "\""+e.GetValue()+"\"", STRING));
    return {ret, OK};
}

// Mathematics Functions
// These functions will be with v0.15. Check the announcement for detail.

/**
 * @brief int phi(int a): Euler's totient of a
 * @name phi
 * @param int a
 * @return int a
 * @remark This is just a demo code. It'll be with v0.15. 
 */
extern "C" ArrayWithCode phi(Array a) {
    Array err(Variable("_", "", INT));
    if (a.container.size() != 1) return {err, ERROR};

    Variable e = a.container[0];
    if (e._t != INT) return {err, ERROR};

    Int n = stoi(e.GetValue());
    if (n < 0) return {err, ERROR};

    Int ret = n;
    for (Int i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            ret -= (ret / i);
            while (!(n % i)) n /= i;
        }
    }

    ret = (n > 1 ? ret - (ret / n) : ret);

    Array res(Variable("_", std::to_string(ret), INT));
    return {res, OK};
}
