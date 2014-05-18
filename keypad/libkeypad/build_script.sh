#!/bin/bash
cd libkeypad
echo "Cleaning up keypad library"
make clean
echo "Building keypad library"
make
cd ..
echo "Cleaning keypad application"
make clean
echo "Building keypad application"
make
echo "Keypad library and application built!"