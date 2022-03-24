#!/bin/sh
# $# tells the number of arguments passed
if [ $# -eq 0 ]
then
	printf "Error: No Arguments provided\n"
else
    given=$1
    units=0
    reverse=0

    # For negative numbers, reverse the modulus of the number and prepend that with minus sign
    if [ $given -lt 0 ]; then
        given=$(( -$given ))
    fi

    # code to reverse a positive number
    while [ $given -gt 0 ]; do
        units=$(( $given % 10 ))
        reverse=$(( $reverse * 10 + $units ))
        given=$(( $given / 10 ))
    done
    
    # print appropriate message
    if [ $1 -lt 0 ]; then
        echo "Reverse of $1 is -$reverse"
    else
        echo "Reverse of $1 is $reverse"
    fi
fi