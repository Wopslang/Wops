# Makefile

test.out :
	g++ -o test.out src/runtime/AST-test.cpp src/import_/eexec_.cpp -std=c++11

clean:
	rm test.out