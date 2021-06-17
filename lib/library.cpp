/*
 * lib/library,cpp
 * Wopslang Library DLL Sourcefile
 *
 * 2021, Wops Team
 * */

#include <iostream>
#include <string>
#include "../src/type/variable.h"

// 표준 I/O 함수

/*
* 함수 정보
* 이름: out
* 매개변수: (any s)
* 반환형: none
*/
extern "C" VariableWithCode out(Variable s) { 
    // EMPTY VARIABLE
    Variable null = Variable("_", "", INT);
    
    std::cout << s.GetValue();
    return {null, OK};
}

/*
* 함수 정보
* 이름: in
* 매개변수: none
* 반환형: string s
* 띄어쓰기 혹은 줄바꿈 시 입력이 종료됩니다
*/
extern "C" VariableWithCode in(Variable _) { 
    std::string s; std::cin >> s;
    Variable ret = Variable("_", s, STRING);
    return {ret, OK};
}
