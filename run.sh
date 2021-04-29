#!/bin/bash
gcc `pkg-config gtk+-3.0 --cflags` main.c -lm -ldl -o main `pkg-config gtk+-3.0 --libs`
./main