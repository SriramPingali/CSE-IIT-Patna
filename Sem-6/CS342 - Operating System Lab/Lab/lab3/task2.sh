#!/bin/bash

if [ $# -eq 0 ]
then
  printf "Task2: Error: No Arguments provided\n"
  exit 1
fi

n=$1
if [[ $n -lt 0 ]]
then
  echo "Error: n must be positve!"
  exit 1
fi

# function to calculate factorial
fact() { 
  if (( $1 < 2 ))
  then
    echo 1
  else
    echo $(( $1 * $( fact $(( $1 - 1 )) ) ))
  fi
}

# function to check if a number is prime or not
isPrime() {
  if (( $1 < 2 ))
  then
    return 0
  fi

  num=$1
  var=$(($num/2))
  for (( i=$var; i>=2; i-- ))
  do
    if (( num%i ==0 ))
    then
      return 0
    fi
  done

  return 1
}

# function to calculate sum of prime numbers less than the given number
sumPrime() {
  if (( $1 < 2 ))
  then
    echo 0
  else
    isPrime $1
    flag=$?
    repeat=$(sumPrime $(($1-1)))
    if (( flag==1 ))
    then
      echo $(($1+$repeat))
    else
      echo $repeat
    fi
  fi
}

# driver code
fct=$(fact $n)
echo "Factorial of $n = $fct"

spr=$(sumPrime $n)
echo "Sum of Primes less than $n = $spr"

echo "Output = $(( fct * spr ))"