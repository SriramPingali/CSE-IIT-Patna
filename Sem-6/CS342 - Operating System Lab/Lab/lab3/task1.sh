#!/bin/bash

if [ $# -eq 0 ]
then
	printf "Task1: Error: No Arguments provided\n"
    exit 1
fi

N=$1
if [ $N -lt 1 ]
then
    echo "Error: N must be positive\n"
    exit 1
fi

# driver code
echo "N = $N"

# store search element
si=$((N+2))
S=${!si}

# place n1, n2, n3, ..., nN into array
declare -a n
for ((argi=2; argi<2+$N; argi++))
do
    n[$argi-2]=${!argi};
done

echo "n has ${#n[*]} items: ${n[*]}"

# Find S in array n by linear search
echo "You are searching for $S"
flag=0
for ((i=0; i<=$N; i++))
do
    if [[ ${n[$i]} -eq $S ]]
    then
        echo "Search found searched item S=$S at index=$((i+1))"
        echo ""
        flag=1
        exit 0
    fi
done

# show this message if S is not found
if [ !$flag ]
then
    echo "Item not found"
    echo ""
fi