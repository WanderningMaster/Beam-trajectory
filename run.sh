#!/bin/bash
gcc `pkg-config gtk+-3.0 --cflags` main.c -o main `pkg-config gtk+-3.0 --libs`
./main