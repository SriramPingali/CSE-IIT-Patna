# CS561 - ARTIFICIAL INTELLIGENCE LAB
# ASSIGNMENT-4: DECISION TREE
# 
# Group_Name: 1801cs31_1801cs32_1801cs33
# Date: 04-Oct-2021
# 
# Students:
#    | Name of Student |  Roll No. |   Batch  |
#    |-----------------|-----------|----------|
#    | M Maheeth Reddy | 1801CS31  |  B.Tech. |
#    | M Nitesh Reddy  | 1801CS32  |  B.Tech. |
#    | Nischal A	   | 1801CS33  |  B.Tech. |

# %%
# import libraries
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

from sklearn.model_selection import KFold
from sklearn.metrics import classification_report, f1_score, precision_score, recall_score, accuracy_score, confusion_matrix

from nltk import ngrams
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize, sent_tokenize

import string
import operator
from copy import deepcopy
from math import log2
from statistics import mean
from collections import Counter

import nltk
nltk.download('stopwords')
nltk.download('punkt')
nltk.download('averaged_perceptron_tagger')


# %%
def get_ngrams(data, n):
    tokens = [token for token in data.split(" ") if token != ""]
    return list(ngrams(tokens, n))

def get_postag(txt):
    stop_words = set(stopwords.words('english'))
    tokenized = sent_tokenize(txt)
    words_list = nltk.word_tokenize(tokenized[0]) 
    words_list = [w for w in words_list if not w in stop_words]  
    return nltk.pos_tag(words_list)

def build_data(path_to_data):
    data,uni,bi,tri,pos = [],[],[],[],[]
    file = open(path_to_data)

    for line in file:
        line = line.split(':')
        row = []
        
        _class, _question = line[0], line[1]
        row.append(_class)
        row.append(' '.join(_question.split(' ')[1:]).translate(str.maketrans('', '', string.punctuation)).rstrip())

        length = len(row[1].split(' '))
        row.append(length)

        unigram = get_ngrams(row[1], 1)
        row.append(unigram)
        uni.extend(unigram)

        bigram = get_ngrams(row[1], 2)
        row.append(bigram)
        bi.extend(bigram)
        
        trigram = get_ngrams(row[1], 3)
        row.append(trigram)
        tri.extend(trigram)

        postag = get_postag(row[1])
        row.append(postag)
        pos.extend(postag)

        # complete set of features for each text 
        data.append(row)

    return data, uni, bi, tri, pos

# load training data
data, uni, bi, tri, pos = build_data('./train_data.txt')
print('Loading Training data...')
print('Training Data:')
print(data[0:5])


# %%
def top_grams(grams, top_n):
    return Counter(grams).most_common(top_n)

unigram_counts = top_grams(uni, 500)
bigram_counts = top_grams(bi, 300)
trigram_counts = top_grams(tri, 200)
pos_counts = top_grams(pos, 500)

avg_length = mean([row[2] for row in data])
print('average length:',avg_length)

# Displaying the top features
print('Top features:\n')
print('Unigrams:\n')
print(unigram_counts[0:5])

print('Bigrams:\n')
print(bigram_counts[0:5])

print('Trigrams:\n')
print(trigram_counts[0:5])

#
print('Pos Counts:\n')
print(pos_counts[0:5])


# %%
def is_numeric(value):
    return isinstance(value, int) or isinstance(value, float)

header = ['Label', 'Text', 'Length', 'Unigram', 'Bigram', 'Trigram', 'POS']

class Question:
    def __init__(self, col, value):
        self.col = col # The column number in the header
        self.value = value # Actual value of the object

    # Matching attributes of current question with the current row
    def match(self, example):
        val = example[self.col]
        if is_numeric(val):
            return val <= self.value
        
        return self.value in val

    # Return the string representation of the object
    def __repr__(self):
        condition = "contains"
        return "Does %s %s %s?" % (
            header[self.col], condition, str(self.value))


# %%
def class_counts(rows):
    counts = {}
    for row in rows:
        label = row[0]

        if label not in counts:
            counts[label] = 0
        
        counts[label] += 1
    return counts

def gini(rows):
    counts = class_counts(rows)
    impurity = 1
    for lbl in counts:
        prob_of_lbl = counts[lbl] / float(len(rows))
        impurity -= prob_of_lbl**2
    return impurity

def misclassifcation_error(rows):
    counts = class_counts(rows)
    max_prob = 0
    for lbl in counts:
        prob_of_lbl = counts[lbl] / float(len(rows))
        if prob_of_lbl > max_prob:
            max_prob = prob_of_lbl
    return 1 - max_prob

def entropy(rows):
    counts = class_counts(rows)
    impurity = 0
    for lbl in counts:
        prob_of_lbl = counts[lbl] / float(len(rows))
        impurity -= prob_of_lbl*log2(prob_of_lbl)
    return impurity

def info_gain(left, right, current_uncertainty, func):
    p = float(len(left))/(len(left)+len(right))
    return current_uncertainty - p*func(left) - (1-p)*func(right)


# %%
class Leaf:
    def __init__(self, rows):
        self.predictions = class_counts(rows)


# %%
class Decision_Node:
    def __init__(self, question, true_branch, false_branch):
        self.question = question
        self.true_branch = true_branch
        self.false_branch = false_branch


# %%
questions = []

for x in unigram_counts:
    questions.append(Question(3, x[0]))

for x in bigram_counts:
    questions.append(Question(4, x[0]))
    
for x in trigram_counts:
    questions.append(Question(5, x[0]))

for x in pos_counts:
    questions.append(Question(6, x[0]))
    
questions.append(Question(2, avg_length))    
    
print(len(questions))
print(questions[0:5])
# print(questions[1500])


# %%
# Returns a split of true rows and false rows for a particular question (single feature)
def partition(rows, question):
    rows_true = []
    rows_false = []
    
    for r in rows:
        if question.match(r):
            rows_true.append(r)
        else:
            rows_false.append(r)
    
    return rows_true, rows_false


# %%
def find_best_split(rows, questions, func):   
    best_gain = 0
    best_question = None
    current_uncertainty = func(rows)
    
    for q in questions:
        rows_true, rows_false = partition(rows, q)
        if len(rows_true) == 0 or len(rows_false) == 0:
            continue
        
        gain = info_gain(rows_true, rows_false, current_uncertainty, func) # Calculating the information gain
        # Updating best gain
        if gain >= best_gain:
            best_gain, best_question = gain, q
    
    return best_gain, best_question   


# %%
# Recursive Function to form the decision tree
# using partitioning (question list is updated periodically)
def form_tree(rows, questions, func):
    # Find the best gain and best question
    gain, question = find_best_split(rows, questions, func)
    if gain == 0:
        return Leaf(rows)
    
    rows_true, rows_false = partition(rows, question)
    questions.remove(question)
    
    true_branch = form_tree(rows_true, questions, func)
    false_branch = form_tree(rows_false, questions, func)
    
    return Decision_Node(question, true_branch, false_branch)


# %%
def classify_row(node, row):
    if isinstance(node, Leaf):
        return node.predictions
    
    if node.question.match(row):
        return classify_row(node.true_branch, row)
    else:
        return classify_row(node.false_branch, row)


# %%
def train(data, questions, func):
    return form_tree(data, deepcopy(questions), func)

def classify(root, rows):
    predictions = [max(classify_row(root, r).items(), key=operator.itemgetter(1))[0] for r in rows]
    return predictions


# %%
def get_data_in_index(data, index):
    l = []
    for i in range(len(data)):
        if i in index:
            l.append(data[i])
    return l

def get_actual_labels(act_data):
    act_labels = []
    
    for d in act_data:
        act_labels.append(d[0])
    
    return act_labels


# %%
kfold = KFold(10, True, 1)
precision,recall,f_score = [],[],[]
i = 0

for trainInd, testInd in kfold.split(data):
    train_data = get_data_in_index(data, trainInd)
    test_data = get_data_in_index(data, testInd)
    
    root = train(train_data, questions, gini)
    prediction = classify(root, test_data)
    actual = get_actual_labels(test_data)
    predicted = prediction
    
    precision.append(precision_score(actual, predicted, average='macro'))
    recall.append(recall_score(actual, predicted, average='macro'))
    f_score.append(f1_score(actual, predicted, average='macro'))
     
    print("Training...")

print('\nGini Index')
print("Precision Score = "+str(mean(precision)))
print("Recall Score = "+str(mean(recall)))
print("F Score = "+str(mean(f_score)))

# %% [markdown]
# ## Part 2
# - All
# - Unigram, Bigram, Trigram, POS
# - Unigram, Bigram, Trigram

# %%
classes = ['ABBR', 'DESC', 'ENTY', 'HUM', 'LOC', 'NUM']

def getReport(train_data, test_data, uniFlag=True, biFlag=True, triFlag=True, posFlag=True, lenFlag=True, func=gini):
    allQuestions = []
    
    if uniFlag:
        for x in unigram_counts:
            allQuestions.append(Question(3, x[0]))

    if biFlag:
        for x in bigram_counts:
            allQuestions.append(Question(4, x[0]))

    if triFlag:
        for x in trigram_counts:
            allQuestions.append(Question(5, x[0]))

    if posFlag:
        for x in pos_counts:
            allQuestions.append(Question(6, x[0]))

    if lenFlag:
        allQuestions.append(Question(2, avg_length))    

    print("No of questions = " + str(len(allQuestions)))

    print("Training...")
    root = train(train_data, allQuestions, func)
    
    print("Predicting...")
    prediction = classify(root, test_data)        
    actual = get_actual_labels(test_data)
    
    print("Prediction done...")
    matrix = confusion_matrix(actual, prediction)
    class_report = classification_report(actual, prediction)
    acc = matrix.diagonal()/matrix.sum(axis=1)
    accuracy_report = dict(zip(classes, acc))
    
    return accuracy_report, class_report, root, prediction, actual

test_data = build_data('./test_data.txt')[0]
print(len(test_data))


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, func=entropy)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, func=misclassifcation_error)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False, func=entropy)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False, func=misclassifcation_error)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False, posFlag=False)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False, posFlag=False, func=entropy)
print(accuracy_report)
print(class_report)


# %%
accuracy_report, class_report, root, prediction, actual = getReport(train_data=data, test_data=test_data, lenFlag=False, posFlag=False, func=misclassifcation_error)
print(accuracy_report)
print(class_report)

# %% [markdown]
# # Error Analysis

# %%
def get_wrong_prediction(prediction, actual, dataset):
    data_list = [dataset[i] for i in range(len(prediction)) if prediction[i] != actual[i]]
    return data_list


# %%
_ , class_matrix, root_gini, prediction_gini, actual_gini  = getReport(train_data=data, test_data=test_data)
wrong_data = get_wrong_prediction(prediction_gini, actual_gini, test_data)


# %%
# Printing the wrong data length
print('Len of wrong data for gini', len(wrong_data))


# %%
_ , class_matrix, root_entropy, prediction_entropy, actual_entropy  = getReport(train_data=data, test_data=wrong_data, func=entropy)
wrong_data_en = get_wrong_prediction(prediction_entropy, actual_entropy, wrong_data)
print('Len of wrong data for entropy is', len(wrong_data_en))


# %%
_ , class_matrix, root_mis, prediction_mis, actual_mis  = getReport(train_data=data, test_data=wrong_data, func=misclassifcation_error)
wrong_data_mis = get_wrong_prediction(prediction_entropy, actual_entropy, wrong_data)
print('Len of wrong data for misclassifcation_error is', len(wrong_data_mis))


# %%
print('Entropy correctly classifies', (len(wrong_data) - len(wrong_data_en)), 'as compared to GINI metric')


# %%
print('Misclassification error correctly classifies', (len(wrong_data) - len(wrong_data_mis)), 'as compared to GINI metric')


