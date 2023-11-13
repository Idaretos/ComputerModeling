#!/bin/zsh

g++ -std=c++17 HW1.cpp -o HW1

echo "rho,L,L_q,W_q" > output/data.txt

for i in {0..95}
do
  j=$(echo "scale=2; $i*0.01" | bc)
  ./HW1 $j
done