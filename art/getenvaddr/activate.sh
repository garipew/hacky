#!/usr/bin/env bash

echo 0 | tee /proc/sys/kernel/randomize_va_space

make
