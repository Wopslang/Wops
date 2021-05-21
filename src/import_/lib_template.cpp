/*
* This is template of external library.
* To contribute:
* 1. add header which needs.
* 2. code a function below the code (DO NOT MAKE NEW CODE!!!)
*
* This is just a template, and you shouldn't modify this.
*/

#include <iostream>
#include <string>

// function impl.
extern "C" int print(std::string s) { // function name will be a name for Wopslang
    std::cout << s;
    return 0; // if there isn't any return value, return 0 (int)
}
