# Makefile

Wopslang :
	g++ -o Wopslang src/interpreter.cpp src/parser/parse.cpp src/import_/eexec_.cpp -std=c++11 -ldl

clean:
	rm Wopslang 
