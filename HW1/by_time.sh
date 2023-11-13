#!/bin/zsh

g++ -std=c++17 by_time.cpp -o by_time

echo "sim_time rho L L_q W_q" > output/time.txt

for i in {3..10}
do
  j=$(echo "$i" | bc)
  ./by_time $j
done