#!/bin/bash

sum=0
count=0
for x in $(cat $1)
do
((count++))
sum=$((sum+x))
done

echo $count
echo $((sum/count))