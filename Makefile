# Makefile

ifeq ($(OS), Windows_NT)
	OPTION=include/dlfcn.c -std=c++11
else
	OPTION=-std=c++11 -ldl
endif

Wopslang :
	g++ -fPIC -shared lib/library.cpp -o lib/library.so -std=c++11 | g++ -o Wopslang src/interpreter.cpp src/parser/parse.cpp src/import_/eexec_.cpp $(OPTION)

CI-TEST-Wopslang: # it will be used to run CI
	g++ -fPIC -shared lib/library.cpp -o test/lib/library.so -std=c++11 | g++ -o Wopslang src/interpreter.cpp src/parser/parse.cpp src/import_/eexec_.cpp $(OPTION)

clean:
	rm Wopslang 
