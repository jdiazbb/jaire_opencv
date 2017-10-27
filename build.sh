#!/bin/sh
g++ -o search search.cpp `pkg-config opencv --cflags --libs`
g++ -o check_on_off check_on_off.cpp `pkg-config opencv --cflags --libs`
