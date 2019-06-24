#!/bin/bash

g++ my_rend.cpp -o my_rend && ./my_rend > out.ppm

eog out.ppm
