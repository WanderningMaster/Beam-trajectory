#!/bin/bash
gcc `pkg-config gtk+-3.0 --cflags` main.c -lm -o main `pkg-config gtk+-3.0 --libs`
./main