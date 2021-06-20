/*
 * lib/library,cpp
 * Wopslang Library DLL Sourcefile
 *
 * 2021, Wops Team
 * */

#include <iostream>
#include <string>
#include "../src/type/variable.h"

// Standard I/O Functions

/**
* @brief out(any s): Standard output
* @name out
* @param (any s)
* @return none
*/
extern "C" VariableWithCode out(Variable s) {
    // EMPTY VARIABLE
    Variable null = Variable("_", "", INT);

    std::cout << s.GetValue();
    return {null, OK};
}

/**
* @brief string in(): Standard input
* @name in
* @param none
* @return string s
* Terminates inputting when space or new line character is entered
*/
extern "C" VariableWithCode in(Variable _) {
    std::string s; std::cin >> s;
    Variable ret = Variable("_", s, STRING);
    return {ret, OK};
}

// Mathematics Functions

/**
 * @brief int phi(int a): Euler's totient of a
 * @name phi
 * @param int a
 * @return int a
 */
extern "C" VariableWithCode phi(Variable a) {
    Variable err("_", "", INT);
    if (a._t != INT) return {err, ERROR};

    Int n = stoi(a.GetValue());
    if (n < 0) return {err, ERROR};

    Int ret = n;
    for (Int i = 2; i*i <= n; i++) {
        if (n % i == 0) {
            ret -= (ret / i);
            while (!(n % i)) n /= i;
        }
    }

    ret = (n > 1 ? ret - (ret / n) : ret);

    Variable res("_", std::to_string(ret), INT);
    return {res, OK};
}
