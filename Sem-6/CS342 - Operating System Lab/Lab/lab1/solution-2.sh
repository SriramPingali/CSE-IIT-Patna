#!/bin/sh
# store the numbers
start=$1
end=$2

# if start number is greater than end, give error. Otherwise, print numbers
if [ $start -gt $end ]
then
    echo "Error: First argument must be lesser than or equal to second argument"
else
    while [ $start -le $end ]
    do
        printf "$start "
        start=$(($start+1))
    done
    echo ""
fi