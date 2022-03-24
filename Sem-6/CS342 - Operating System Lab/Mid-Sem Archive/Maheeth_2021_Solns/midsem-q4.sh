#!/bin/bash

#    Name: M Maheeth Reddy
#    Roll: 1801CS31
#
#    Operating Systems Lab - Midsem Assignent
#                Question 4
#    
#    Shell Script

two=2

# Inputs
F=$1	# Filename
K=$2	# Number


declare -A freq	# associative array to store frequency of each word in file F

# read file F, line by line. Update the frequency of each word in the line, in the "freq" array
while read -r line || [[ -n "$line" ]]; do
	for word in $line; do ((freq[$word]++)); done
done < $F


declare -A freq_gK	# associative array to store the words whose frequencies are greater than K

# iterate through "freq" array, store words whose frequency > K, in "freq_gK" array
# also print the frequencies of each word in file F
echo "Frequencies of words in $F:"
for iter in "${!freq[@]}"; do
    echo "$iter ${freq[$iter]}";

	if [[ "${freq[$iter]}" -gt "$K" ]]; then
		freq_gK[${#freq_gK[@]}]=$iter
   	fi
done


declare -A freqMask		# array to store frequency of MASKED in each txt file in current directory
declare -A filenames	# array to store name of each file in current directory

# replace words in txt files with MASKED as per given condition, using sed command
# and store frequency of MASKED in "freqMask" array, filename in "filenames" array
for file in `ls *.txt`; do
    for word in "${freq_gK[@]}"; do
    	sed -i "s/$word/MASKED/g" "$file"
    done

    freqMask[${#freqMask[@]}]=$(grep -o -i MASKED $file | wc -l)
    filenames[${#filenames[@]}]=$file
done

# sort the values in "freqMask" and "filenames" arrays in decreasing order of frequency of MASKED
for iter1 in "${!freqMask[@]}"; do
	for iter2 in "${!freqMask[@]}"; do
		if [[ ${freqMask[$iter1]} -gt ${freqMask[$iter2]} ]]; then
			tmpM=${freqMask[$iter1]}
			tmpF=${filenames[$iter1]}

			freqMask[$iter1]=${freqMask[$iter2]}
			filenames[$iter1]=${filenames[$iter2]}
			
			freqMask[$iter2]=$tmpM
			filenames[$iter2]=$tmpF
		fi
	done
done

echo ""
echo "Files with frequency of \"MASKED\" > $((K/two)) in current directory:"
# print filenames in which frequency of MASKED > K/2
for iter in "${!filenames[@]}"; do
    if [[ ${freqMask[$iter]} -gt $((K/two)) ]]; then
    	echo "${filenames[$iter]} : ${freqMask[$iter]}";
    fi
done