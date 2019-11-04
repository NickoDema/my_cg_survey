#!/bin/bash

if [ $# -eq 1 ]
  then
    NAME=$1
else
    NAME="rend"
fi

eog pics/${NAME}_result.ppm
