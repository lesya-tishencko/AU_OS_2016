#!/bin/bash

make
qemu-system-x86_64 -kernel kernel -serial stdio
make clean
