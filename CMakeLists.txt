cmake_minimum_required(VERSION 3.18)

project(
  Wopslang 
  VERSION 0.1
  DESCRIPTION "Wops programming language"
  LANGUAGES CXX)

add_executable(test src/runtime/AST-test.cpp src/import_/eexec_.cpp)
target_compile_options(test PUBLIC -std=c++17 -ldl)
