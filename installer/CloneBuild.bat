cd C:\
git clone https://github.com/Wopslang/Wops.git

cd C:\wops
g++ -fPIC -shared lib/library.cpp -o lib/library.so -std=c++11 | g++ -o Wopslang src/interpreter.cpp src/parser/parse.cpp include/dlfcn.c src/import_/eexec_.cpp -std=c++11