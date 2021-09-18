# Makefile

test :
	g++ -o test src/runtime/AST-test.cpp src/import_/eexec_.cpp -std=c++11 -ldl

clean:
	rm test
