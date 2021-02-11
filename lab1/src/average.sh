#!/bin/bash
echo $# parameters
sum=0
for params in "$@"
do
sum=$(($params+$sum))
done
average=$(($sum/$#))
echo $average


