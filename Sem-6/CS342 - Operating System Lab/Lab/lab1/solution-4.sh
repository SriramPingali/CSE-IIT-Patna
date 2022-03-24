#!/bin/bash
# print arguments provided
printf "arg1:\t$1\n"
printf "arg2:\t$2\n"
printf "arg3:\t$3\n"
echo "========="

# variable to count files renamed
count=1

# function to find file extension
findExtension(){
    file=$1   # get filename
    dot="."
    cnt=0     # position of dot
    length=${#file}
    # find position of dot through iteration
    for ((i=0;i<$length;i++))
    do
        if [[ "${file:i:1}" == "$dot" ]]; then
            cnt=$i                    
        fi
    done

    cnt=$(($cnt+1))
    # store the extension using dot position
    ex="${file:cnt:length}"
    # return the extension
    eval "$2=$ex"
}

# iterate through each file in $1 folder and rename if filename contains substring $2
for entry in "$1"/*
do
    printf "path:\t$entry\n"
    baseName=$(basename $entry)
    printf "file:\t$baseName\n"

    # search for substring $2 in filename
    # if present, rename that file to $3_$count.txt
    str=$baseName
    sub=$2
    strlen=${#str}
    sublen=${#sub}
    for i in $(seq 0 $strlen)
    do
        if [[ "${str:i:$sublen}" == "$sub" ]]
        then
            extens=""
            findExtension $baseName extens
            printf ">>>>> $str has been renamed to $3_$count.$extens <<<<<\n"
            mv $entry "$1/$3_$count.$extens"
            count=$(($count+1))
        fi
    done

    echo "---------"
done