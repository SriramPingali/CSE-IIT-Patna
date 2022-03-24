import numpy as np

# Sigmoid function
def sigmoid(x):
    return 1/(1+np.exp(-x))

# Threshold function
def threshold(output_output):
    return (output_output >= 0.5).astype(int)

# Possible inputs for XOR gate
features = np.array([[0,0],[0,1],[1,0],[1,1]])

print('''Number of input layer neurons = 2
Number of hidden layer neurons = 2
Number of output layer neurons = 1
''')
# Hard coded weights and biases
hidden_weights = np.array([
    [5.8, 3.6],
    [5.9, 3.6]
])
hidden_bias = np.array([[-2.4, -5.5]])

output_weights = np.array([
    [ 7.4],
    [-8.1]
])
output_bias = np.array([[-3.3]])

# Running the model on all inputs
hidden_activate = np.dot(features,hidden_weights)
hidden_activate += hidden_bias
hidden_output = sigmoid(hidden_activate)

output_activate = np.dot(hidden_output,output_weights)
output_activate += output_bias
output_output = sigmoid(output_activate)

predicted_xors = threshold(output_output)

# printing the hyper parameters
print('hidden_weights:')
print(hidden_weights)
print('\nhidden_bias:')
print(hidden_bias)
print('\noutput_weights:')
print(output_weights)
print('\noutput_bias:')
print(output_bias)

print('\nTruth Table of XOR gate neural network:')
print(np.hstack((features,predicted_xors)))