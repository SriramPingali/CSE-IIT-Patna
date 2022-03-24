# Deduction Theorem

# Declaring Boolean functions
def AND(a, b):
	return a and b

def OR(a, b):
	return a or b

def IMPLIES(a, b):
	return OR(not a, b)

# Mapping symbols to corresponding boolean operations
OP_DICT = {'^':AND, 'v':OR, '=>':IMPLIES}

# evaluating the value of an expression
def evaluater(expression, values):
	index = 0
	left,right = "",""
	operation = ""
	l = len(expression)

	if(l == 1):
		return(values[expression[0]])
	elif(l == 2):
		return(str((int(values[expression[1]]) + 1) % 2))

	while index < l:
		if expression[index] == '^' or expression[index] == 'v' or expression[index] == '=':
			operation = expression[index]
			break
		left += expression[index]
		index += 1
	index += 1

	if expression[index] == '>':
		operation += expression[index]
		index += 1
	while index < l:
		right += expression[index]
		index += 1

	if left[0] == '~':
		left_value = values[left[1]]
	else :
		left_value = values[left[0]]

	if right[0] == '~':
		right_value = values[right[1]]
	else :
		right_value = values[right[0]]

	res = OP_DICT[operation](int(left_value),int(right_value))
	return str(res);


# simplifying a query
def query_simplifier(query, values):
	stack = []
	
	for i, ch in enumerate(query):
		if(ch == ")"):
			expression = []
			
			while(stack[-1] != "("):
				expression.append(stack.pop())
			
			stack.append(evaluater(expression[::-1], values))
		else:
			stack.append(ch)
	
	if(stack[-1] == "1"):
		return True
	
	return False


# split the left and right hand sides of an implication
def lhs_rhs_splitter(query, values):
	stack = []
	index = 0

	for i, c in enumerate(query):
		if(c==")"):
			stack.pop()
			if(len(stack) == 0):
				index = i
				break
		elif(c == "("):
			stack.append(c)

	if("=>" in query):
		lhs = query[:i+1]
		rhs = query[i+3:]
		
		if (query_simplifier(lhs, values)):
			if (not query_simplifier(rhs, values)):
				return False

# driver function/main function
if __name__ == "__main__":
	input_expr = str(input("Enter a Query: "))
	variable = set([x for x in input_expr if x not in ['=', '>', '~', ')', '(', '^', 'V']])
	
	flag = 0
	
	for i in range(2**len(variable)):
		values = {}
		values['0'] = '0'
		values['1'] = '1'

		temp = format(i, '0'+str(len(variable))+'b')
		for j, c in enumerate(variable):
			values[c] = temp[j]
		
		if(lhs_rhs_splitter(input_expr, values) == False):
			flag = 1
	
	if(flag):
		print(input_expr+" is NOT a theorem")
	else:
		print(input_expr+" is a theorem")
