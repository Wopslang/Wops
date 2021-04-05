# Inform: parser.cpp가 받는 파일 형식
parser.cpp는 어느정도의 최적화가 이루어 진 후 사용할 수 있는 코드입니다.  
그렇기 때문에 testfile.wops를 변경할 때 몇가지 주의하실 사항이 존재합니다.

- 앞 뒤에 공백을 넣지 말아주세요
- 가능한 문법은 lvalue 생성 or 수정, if, for, 일부 빌트인 함수 호출입니다.
- 또한 가능한 문법으로 코드를 적을 때에는 다음과 같은 형식으로 넣어주세요
    - int a = 30
    - out ( a )
    - for i in range ( 1 , 100 )
    - if ( i % 2 == 0 )
    - out ( i )
