#!/bin/sh
# count number of lines in file specified by $1 argument
lines=0
while read file; do
	lines=$(($lines+1))
done <$1
echo "$1 has $lines lines"

# if number of lines is less than $2 argument, delete file
if [ $lines -lt $2 ]
then
	rm $1
	echo "Removed $1"
fi