#!/bin/sh
set -x
gcc -shared -fPIC -o libtest.so test.c
gcc -o main -L. main.c -ltest
