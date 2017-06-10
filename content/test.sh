#!/bin/bash
set -e

clang++ -o ./std/$1/$1 ./std/$1/$1.cpp -O2 -std=c++11
clang++ -o ./brute/$1/$1 ./brute/$1/$1.cpp -O2 -std=c++11
clang++ -o ./generator/$1/gen ./generator/$1/gen.cpp -O3 -std=c++0x

rm -r tmp
mkdir tmp

while true; do
  ./generator/$1/gen < ./seed/$1.seed > tmp/$1.in
  ./brute/$1/$1 < ./tmp/$1.in > tmp/$1.brute.out
  ./std/$1/$1 < ./tmp/$1.in > tmp/$1.std.out

  diff tmp/$1.brute.out tmp/$1.std.out
  echo "PASS"
done
