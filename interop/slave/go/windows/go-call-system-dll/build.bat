#!/bin/bash

gcc -fPIC -shared -o example.dll example.c
go build main.go
