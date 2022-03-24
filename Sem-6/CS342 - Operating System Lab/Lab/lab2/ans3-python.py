import sys

# function to find xor of given numbers in binary
def XOR(n1b, n2b):
	length=len(n1b)
	res=""

	for i in range(0, length):
		if n1b[i]==n2b[i]:
			res=res+"0"
		else:
			res=res+"1"
	
	# print the xor value
	print("XOR of",n1b,n2b,"is",res)
	binToDec(res)

def binToDec(num):
	exp = 1
	dec = 0
	length = len(num)-1

	# convert the binary format to decimal format
	for i in range(length,-1,-1):
		if num[i]=='1':
			dec=dec+exp
		exp=exp*2

	# print the result in decimal format
	print(num,"in decimal is",dec)

# function to convert decimal to binary format
def decToBin(dec):
	num=""
	while dec>0:	
		a=str(dec%2)
		num=a+num
		dec=dec//2
	return num

if __name__ == '__main__':

	# store the numbers
	n1=int(sys.argv[1])
	n2=int(sys.argv[2])

	# convert numbers to binary format
	n1b=decToBin(n1)
	n2b=decToBin(n2)

	len1=len(n1b)
	len2=len(n2b)

	# pad zeroes to the binary formats, so that they will have same number of bits
	if len1>len2:	
		temp=len1-len2
		for i in range(0,temp):
			n2b="0"+n2b
	else:
		temp=len2-len1
		for i in range(0,temp):
			n1b="0"+n1b
	
	print(n1,"in binary:",n1b)
	print(n2,"in binary:",n2b)

	#calculate xor value for first and second calculated binary formats
	XOR(n1b,n2b)