#!/bin/bash

if [ $# -eq 1 ]
  then
    NAME=$1
else
    NAME="tests"
fi

CPPNAME="${NAME}.cpp"

g++ -Iinclude $CPPNAME -o $NAME && ./$NAME
