#!/bin/bash
set -e # stop the script on errors
g++ -fPIC -shared -o libhello.so hello.cpp
g++ -o main.out main.cpp -ldl
./main.out