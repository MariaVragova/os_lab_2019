#!/bin/bash
#echo $# parameters
FILE=$1
parameters=0
sum=0
#for params in "$@";
while read LINE;
do
sum=$(($LINE+$sum))
parameters=$(($parameters+1))
echo parameter [$parameters] - "$LINE";
#echo $params
#sum=$(($params+$sum))
#echo $sum;
#done < numbers.txt
done < $FILE
echo $parameters- parameters
average=$(($sum/$parameters))
echo $average - average


