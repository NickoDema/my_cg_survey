#!/bin/bash

if [ $# -eq 1 ]
  then
    NAME=$1
else
    NAME="my_rend"
fi

CPPNAME="${NAME}.cpp"

g++ -Iinclude $CPPNAME -o $NAME && ./$NAME > out.ppm

eog out.ppm
