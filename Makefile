# c++11
executor_test : executor_test.o
	clang++ -o executor_test executor_test.o -std=c++11

executor_test.o : runtime/executor_test.cpp
	clang++ -c -o executor_test.o runtime/executor_test.cpp -std=c++11

errorhandler_test : errorhandler_test.o
	clang++ -o errorhandler_test errorhandler_test.o -std=c++11

errorhandler_test.o : error/errorhandler_test.cpp
	clang++ -c -o errorhandler_test.o error/errorhandler_test.cpp -std=c++11

parser_test : parser_test.o
	clang++ -o parser_test parser_test.o -std=c++11

parser_test.o : parser/parser_test.cpp
	clang++ -c -o parser_test.o parser/parser_test.cpp -std=c++11
	
all: executor_test errorhandler_test parser_test

clean_executor_test:
	rm executor_test.o executor_test

clean_errorhandler_test:
	rm errorhandler_test.o errorhandler_test 

clean_parser_test:
	rm parser_test.o parser_test 
clean: clean_executor_test clean_errorhandler_test clean_parser_test
