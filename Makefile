# Makefile

ifeq ($(OS), Windows_NT)
	OPTION=-std=c++11
else
	OPTION=-std=c++11 -ldl
endif

Wopslang :
	g++ -o Wopslang src/interpreter.cpp src/parser/parse.cpp src/import_/eexec_.cpp $(OPTION)

clean:
	rm Wopslang 
