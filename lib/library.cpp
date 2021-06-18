/*
 * lib/library,cpp
 * Wopslang Library DLL Sourcefile
 *
 * 2021, Wops Team
 * */

#include <iostream>
#include <string>
#include "../src/type/array.h"
#include "../src/type/variable.h"

// 표준 I/O 함수

/**
* @brief out([any s]): 표준 출력
* 함수 정보
* 이름: out
* 매개변수: [any s]
* 반환형: none
*/
extern "C" ArrayWithCode out(Array s) { 
    // EMPTY VARIABLE
    Array null(Variable("_", "", INT));
    
    for (Variable e: s.container) {
        std::cout << e.GetValue();
    }
    return {null, OK};
}

/**
* @brief string in(): 표준 입력
* 이름: in
* 매개변수: none
* 반환형: string s
* 띄어쓰기 혹은 줄바꿈 시 입력이 종료됩니다
*/
extern "C" ArrayWithCode in(Array _) { 
    std::string s; std::cin >> s;
    Array ret(Variable("_", s, STRING));
    return {ret, OK};
}

// 수학 함수

/**
 * @brief int phi(int a): a에 대한 오일러 파이값
 * 이름: phi
 * 매개변수: int a
 * 반환형: int a
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