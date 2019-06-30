#!/bin/bash

if [ $# -eq 1 ]
  then
    NAME=$1
else
    NAME="test"
fi

CPPNAME="${NAME}.cpp"
EXENAME="${NAME}.out"

g++ -I../include $CPPNAME -o $EXENAME && ./$EXENAME
