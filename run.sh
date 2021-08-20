#!/bin/sh
mkdir -p build && cd build && cmake .. && make && ./sugi && cd ..
