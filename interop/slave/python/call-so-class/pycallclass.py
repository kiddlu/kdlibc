#!/usr/bin/env python

import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll("./libpycallclass.so")

print 'display()'
lib.display()

print 'display(100)'
lib.display_int(100)
