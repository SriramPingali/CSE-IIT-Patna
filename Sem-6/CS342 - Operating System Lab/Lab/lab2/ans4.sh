#!/bin/bash
echo -n "Enter Password: "
read password

# check if the password has atleast 7 characters
len=${#password}
if (( $len < 7 ))
then
    echo "Invalid Password"
    exit 1
fi

# check if there are numbers in the password
isnum=0
for (( i=0; i<$len; i++ ))
do
    for ch in {0..9}
    do
        if [[ ${password:i:1} == $ch ]]
        then
            isnum=1
        fi
    done
done

# if no numbers present, password is invalid
if [[ $isnum != 1 ]]
then
    echo "Invalid Password"
    exit 1
fi

# check if there are alphabets in the password
isalpha=0
for (( i=0; i<$len; i++ ))
do
    for ch in {a..z}
    do
        if [[ ${password:i:1} == $ch ]]
        then
            isalpha=1
        fi
    done

    for ch in {A..Z}
    do
        if [[ ${password:i:1} == $ch ]]
        then
            isalpha=1
        fi
    done
done

# if no alphabets present, password is invalid
if [[ $isalpha != 1 ]]
then
    echo "Invalid Password"
    exit 1
fi

declare -a sym
sym="/()<>?"

# check if there are special symbols in the password
isspl=0
for (( i=0; i<$len; i++ ))
do
    if [[ ${password:$i:1} == "/" || ${password:$i:1} == "(" || ${password:$i:1} == ")" || ${password:$i:1} == "<" || ${password:$i:1} == ">" || ${password:$i:1} == "?" ]]
    then
		isspl=1
	fi
done

# if no special characters present, password is invalid
if [[ $isspl != 1 ]]
then
    echo "Invalid Password"
    exit 1
fi

# if all conditions hold true, password is valid
echo "Valid Password"