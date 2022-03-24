#!/bin/bash

# constants
zero=0
one=1

# function to convert decimal number to binary
decToBin() {
    n=$1
    b=""
    while [ $n -gt 0 ]
    do
        t=$(($n%2))
        b=$t$b
        n=$(($n/2))
    done
    eval "$2=$b"
}

# function to calculate xor of 2 binary strings
XOR() {
    x1=$1
    x2=$2
    len=${#x1}

	# check the given numbers bit by bit
	# if bits are equal, then xor is 0. Otherwise xor is 1
    res=""
    for (( i=0;i<len;i++ ))
    do
        if [[ ${x1:i:1} == ${x2:i:1} ]]
        then
            res=$res$zero
        else
            res=$res$one
        fi
    done

    echo "XOR of $1 and $2 is $res"
    binToDec $res
}

# function to convert binary number to decimal
binToDec() {
    exp=1
    dec=0
    b=$1
    len=${#b}
    len=$(($len-1))

    for (( i=len; i>=0; i--))
    do
        if [[ ${b:i:1} -eq 1 ]]
        then
            dec=$(($dec+$exp))
        fi
        exp=$(($exp*2))
    done

    echo "$1 in decimal is $dec"
}

# store given numbers
n1=$1
n2=$2

# convert given numbers to corresponding binary equivalents
n1b=""
decToBin $n1 n1b
n2b=""
decToBin $n2 n2b

len1=${#n1b}
len2=${#n2b}

# pad zeroes to the binary formats, so that they will have same number of bits
if [[ $len1 -gt $len2 ]]
then
	temp=$(($len1-$len2))
	for (( i=0;i<temp;i++ ))
    do
		n2b=$zero$n2b
	done
else
	temp=$(($len2-$len1))
	for (( i=0;i<temp;i++ ))
    do
		n1b=$zero$n1b
	done
fi

# display result
echo "$n1 in binary: $n1b"
echo "$n2 in binary: $n2b"
XOR $n1b $n2b