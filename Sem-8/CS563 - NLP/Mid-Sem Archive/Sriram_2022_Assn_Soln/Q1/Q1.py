import sys
import os
import json
import math
import numpy
from sklearn.metrics import accuracy_score, f1_score, precision_recall_fscore_support

START_CHAR = '*'
STOP_CHAR = 'STOP'
RARE_CHAR = '_RARE_'
RARE_WORD_MAX_FREQ = 5
LOG_PROB_OF_ZERO = -1000

def fetch_data(data):
    penn_words, penn_tags = [], []
    temp_words, temp_tags = [START_CHAR], [START_CHAR]
    for raw_word_tag in data:
        word_tag = raw_word_tag.strip('\n')
        if word_tag != '_,_':
            split_data = word_tag.split(',')
            word, tag = split_data[0], split_data[1]
            temp_words.append(word)
            temp_tags.append(tag)
        else:
        	temp_words.append(STOP_CHAR)
        	temp_tags.append(STOP_CHAR)
        	penn_words.append(temp_words)
        	penn_tags.append(temp_tags)
        	temp_words, temp_tags = [START_CHAR], [START_CHAR]
    return penn_words, penn_tags

def fetch_bigrams(item):
    temp_bigrams = []
    for i in range(len(item)-1):
        temp_bigrams.append((item[i], item[i+1]))
    return temp_bigrams

def fetch_transitions_probs(penn_tags):
    transition_dict = {}

    no_of_bigrams = {}
    no_of_unigrams = {}

    for item in penn_tags:
        temp_bigram = fetch_bigrams(item)

        for bigram in temp_bigram:
            if bigram not in no_of_bigrams:
                no_of_bigrams[bigram] = 1
            else:
                no_of_bigrams[bigram] += 1

        for unigram in item:
            if unigram not in no_of_unigrams:
                no_of_unigrams[unigram] = 1
            else:
                no_of_unigrams[unigram] += 1

    for bigram in no_of_bigrams:
        transition_dict[bigram] = math.log(no_of_bigrams[bigram], 2) - math.log(no_of_unigrams[bigram[0]],2)
    return transition_dict

def compute_known(penn_words):
    known_words = set([])
    
    no_of_words = {}

    for item in penn_words:
        for word in item:
            if word not in no_of_words:
                no_of_words[word] = 1
            else:
                no_of_words[word] += 1

    for item in no_of_words:
        if no_of_words[item] > RARE_WORD_MAX_FREQ:
            known_words.add(item)
	    
    return known_words

def swap_unique(penn_words, known_words):
    penn_words_rare = []

    for item in penn_words:
        tmp = []
        for word in item:
            if word in known_words:
                tmp.append(word)
            else:
                tmp.append(RARE_CHAR)
        penn_words_rare.append(tmp)

    return penn_words_rare

def fetch_emission_prob(penn_words_rare, penn_tags):
    eigen = {}
    list_of_tags = set([])

    no_of_tags = {}
    raw_word_no_of_tags = {}
    for i in range(len(penn_tags)):
        concat = penn_words_rare[i]
        tags = penn_tags[i]
        for j in range(len(tags)):
            word = concat[j]
            tag = tags[j]
            if (word, tag) not in raw_word_no_of_tags:
                raw_word_no_of_tags[(word,tag)] = 1
            else:
                raw_word_no_of_tags[(word,tag)] +=1
            if tag not in no_of_tags:
                no_of_tags[tag] = 1
            else:
                no_of_tags[tag] += 1

    for item in raw_word_no_of_tags:
        eigen[item] = math.log(raw_word_no_of_tags[item],2) - math.log(no_of_tags[item[1]],2)

    for item in no_of_tags:
        list_of_tags.add(item)

    return eigen, list_of_tags

def viterbi(penn_dev_words, list_of_tags, known_words, transition_dict, eigen):
    tagged = []

    print(len(penn_dev_words))


    for item in penn_dev_words:

        concat = item + [STOP_CHAR]
        converted_concat = []
        n = len(concat)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[START_CHAR] = [START_CHAR]
        Unigram_Pre = [START_CHAR]           

        Pi_Pre = {}
        Pi_Pre[START_CHAR] = 0
        
        while cur_len < n:
            Path_Cur = {}
            Unigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = STOP_CHAR
                tagspace = [STOP_CHAR]
            else:
                word = concat[cur_len]
                if word not in known_words:
                    word = RARE_CHAR
                tagspace = list(list_of_tags)
            converted_concat.append(word)

            for v in tagspace:
                emi_tmp = (word, v)
                if emi_tmp not in eigen:
                    continue
                w_tmp = ''
                for w in list_of_tags:
                    if w not in Unigram_Pre:
                        continue
                    bigram_cur = (w, v)
                    if bigram_cur not in transition_dict:
                        transition_dict[bigram_cur] = LOG_PROB_OF_ZERO
                    if v not in Pi_Cur:
                        Pi_Cur[v] = Pi_Pre[w]+transition_dict[bigram_cur]+eigen[emi_tmp]
                        w_tmp = w
                    elif Pi_Pre[w]+transition_dict[bigram_cur]+eigen[emi_tmp] > Pi_Cur[v]:
                        Pi_Cur[v] = Pi_Pre[w]+transition_dict[bigram_cur]+eigen[emi_tmp]
                        w_tmp = w
                if w_tmp != '':
                    Path_Cur[v] =  Path_Pre[w_tmp]+[v]
                    Unigram_Cur.append(v)

            Pi_Pre = dict(Pi_Cur)
            Unigram_Pre = list(Unigram_Cur)
            Path_Pre = dict(Path_Cur)
            cur_len += 1

        st = ''
        unigram_max = Unigram_Pre[0]
        for bigram in Unigram_Pre:
            if Pi_Cur[bigram] > Pi_Cur[unigram_max]:
                bigram = unigram_max
        for i,tag in enumerate(Path_Cur[unigram_max][1:-1]):
            st = st + concat[i]+'/'+tag+' '
        tagged.append(st.strip()+'\n')
        if len(tagged) % 100 == 0:
            print(len(tagged))

    return tagged

# This function takes the output of viterbi() and outputs it to file
def fetch_output(tagged, filename):
    outfile = open(filename, 'w', encoding='utf-8')
    for concat in tagged:
        outfile.write(concat)
    outfile.close()


OUTPUT_PATH = 'output/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

def seperate_dev_tags(dev_concats):
    penn_words = []
    penn_tags = []

    for item in dev_concats:
        temp_words = []
        temp_tags = []
        concat = item.strip().split(' ')
        for token in concat:
        	temp_tags.append(token.rsplit('/',1)[1])
        	temp_words.append(token.rsplit('/',1)[0])
        penn_words.append(temp_words)
        penn_tags.append(temp_tags)

    return penn_words, penn_tags

def evaluate(test_penn_tags, test_tags, padding):
    # Flattening all original tags
    true_tags = []
    for tag in test_penn_tags:
        true_tags.extend(tag[1:-1])

    # Flattening all predicted tags
    pred_tags = []
    for tag in test_tags:
        pred_tags.extend(tag[1:-1])

    # assert len(true_tags) == len(pred_tags)
    print('Accuracy', accuracy_score(true_tags, pred_tags))
    print('F score', f1_score(true_tags, pred_tags, average='macro'))
    print('Precison, Recall, F score', precision_recall_fscore_support(true_tags, pred_tags, average='macro'))



def modify_tags(penn_tags, choice):
    reconstruct_tag = []
    for list_of_tags in penn_tags:
        new_tags = []
        for tag in list_of_tags:
            split_tag = tag.split('-')
            if len(split_tag) == 1:
                new_tags.append(tag)
            else:
                if choice=='1':
                    new_tags.append(split_tag[0])
                else:
                    new_tags.append(split_tag[1])
        reconstruct_tag.append(new_tags)
    return reconstruct_tag


def main():

    with open('midsem_data/train_data.txt', 'r') as infile:
        train_words, train_tags = fetch_data(infile.readlines())
    with open('midsem_data/test_data.txt', 'r') as infile:
        test_words, test_tags = fetch_data(infile.readlines())   

    padding = 'results'        
    
    # calculate tag trigram probabilities (question 2)
    transition_dict = fetch_transitions_probs(train_tags)

    # calculate list of words with count > 5 (question 3)
    known_words = compute_known(train_words)

    # get a version of penn_words with rare words replace with '_RARE_' (question 3)
    penn_words_rare = swap_unique(train_words, known_words)

    # calculate emission probabilities (question 4)
    eigen, list_of_tags = fetch_emission_prob(penn_words_rare, train_tags)

    # delete unneceessary data
    del penn_words_rare

    # do viterbi on penn_dev_words (question 5)
    viterbi_tagged = viterbi(test_words, list_of_tags, known_words, transition_dict, eigen)

    # question 5 output
    fetch_output(viterbi_tagged, OUTPUT_PATH + 'S5'+padding+'.txt')

    # Calculating accuracy
    test_words, test_predicted_tags = seperate_dev_tags(viterbi_tagged)
    evaluate(test_tags, test_predicted_tags, padding)

if __name__ == "__main__": 
    main()
