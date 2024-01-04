#!/bin/bash
set -e # stop the script on errors
g++ -fPIC -shared -o libhello.so hello.cpp -std=c++17
g++ -o main.out main.cpp -ldl -std=c++17
./main.out