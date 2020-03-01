#!/bin/bash
count=0
summa=0
for param in "$@"
do
count=$(($count+1))
summa=$(($summa+$param))
done
echo "quality $count"
b=$(($summa/$count))
echo srednee "$b"