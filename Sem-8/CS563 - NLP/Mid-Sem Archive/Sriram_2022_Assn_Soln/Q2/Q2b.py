import sys
import os
import json
import math
import numpy
from sklearn.metrics import classification_report

START_CHAR = '*'
STOP_CHAR = 'STOP'
RARE_CHAR = '_RARE_'
RARE_WORD_MAX_FREQ = 5
LOG_PROB_OF_ZERO = -1000

def fetch_data(data):
    total_words = []
    total_tags = []
    for sentence in data:
        words = sentence[0].split(' ')
        words = [START_CHAR, START_CHAR] + words + [STOP_CHAR]
        tags = [START_CHAR, START_CHAR] + sentence[1] + [STOP_CHAR]
        # assert len(words) == len(tags)
        total_words.append(words)
        total_tags.append(tags)
    return total_words, total_tags

def fetch_bigrams(item):
    temp_bigrams = []
    for i in range(len(item)-1):
        temp_bigrams.append((item[i], item[i+1]))
    return temp_bigrams

def fetch_trigrams(item):
    temp_trigrams = []
    for i in range(len(item)-2):
        temp_trigrams.append((item[i], item[i+1], item[i+2]))
    return temp_trigrams

def fetch_transitions_probs(penn_tags):
    transition_dict = {}

    no_of_bigram = {}
    no_of_trigram = {}

    for item in penn_tags:
        temp_bigram = fetch_bigrams(item)
        temp_trigram = fetch_trigrams(item)

        for bigram in temp_bigram:
            if bigram not in no_of_bigram:
                no_of_bigram[bigram] = 1
            else:
                no_of_bigram[bigram] += 1

        for trigram in temp_trigram:
            if trigram not in no_of_trigram:
                no_of_trigram[trigram] = 1
            else:
                no_of_trigram[trigram] +=1

    for trigram in no_of_trigram:
        transition_dict[trigram] = math.log(no_of_trigram[trigram], 2) - math.log(no_of_bigram[trigram[:2]],2)
    return transition_dict

def compute_known(penn_words):
    known_words = set([])
    
    no_of_words = {}

    for item in penn_words:
        for word in item:
            if word in no_of_words:
                no_of_words[word] += 1
            else:
                no_of_words[word] = 1

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
    word_no_of_tags = {}
    for i in range(len(penn_tags)):
        sentence = penn_words_rare[i]
        tags = penn_tags[i]
        for j in range(1, len(tags)):
            word = sentence[j]
            tag = tags[j]
            tag_prev = tags[j-1]
            if (word, tag, tag_prev) in word_no_of_tags:
                word_no_of_tags[(word,tag,tag_prev)] += 1
            else:
                word_no_of_tags[(word,tag,tag_prev)] =1
            if (tag, tag_prev) in no_of_tags:
                no_of_tags[tag, tag_prev] += 1
            else:
                no_of_tags[tag, tag_prev] = 1

    for item in word_no_of_tags:
        eigen[item] = math.log(word_no_of_tags[item],2) - math.log(no_of_tags[item[1:]],2)

    for item in no_of_tags:
        list_of_tags.add(item[0])
        list_of_tags.add(item[1])

    return eigen, list_of_tags

def viterbi(penn_dev_words, list_of_tags, known_words, transition_dict, eigen):
    tagged = []

    print(len(penn_dev_words))

    Pi_Init = {}
    for u in list_of_tags:
        for v in list_of_tags:
            Pi_Init[(u,v)] = LOG_PROB_OF_ZERO

    for ctr, item in enumerate(penn_dev_words):

        sentence = item + [STOP_CHAR]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[(START_CHAR, START_CHAR)] = [START_CHAR, START_CHAR]
        Bigram_Pre = [(START_CHAR, START_CHAR)]           

        Pi_Pre = {}
        Pi_Pre[(START_CHAR, START_CHAR)] = 0
        
        # For each sentence
        while cur_len < n:
            Path_Cur = {}
            Bigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = STOP_CHAR
                tagspace = [STOP_CHAR]
            else:
                word = sentence[cur_len]
                if word not in known_words:
                    word = RARE_CHAR
                tagspace = list(list_of_tags)
            converted_sentence.append(word)

            for v in tagspace:
                for u in list_of_tags:
                    emi_tmp = (word, v, u)
                    if emi_tmp not in eigen:
                        eigen[emi_tmp] = LOG_PROB_OF_ZERO
                    w_tmp = ''
                    for w in list_of_tags:
                        if (w,u) not in Bigram_Pre:
                            continue
                        trigram_cur = (w,u,v)
                        if trigram_cur not in transition_dict:
                            transition_dict[trigram_cur] = LOG_PROB_OF_ZERO
                        if (u,v) not in Pi_Cur:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+transition_dict[trigram_cur]+eigen[emi_tmp]
                            w_tmp = w
                        elif Pi_Pre[(w,u)]+transition_dict[trigram_cur]+eigen[emi_tmp] > Pi_Cur[(u,v)]:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+transition_dict[trigram_cur]+eigen[emi_tmp]
                            w_tmp = w
                    if w_tmp != '':
                        Path_Cur[(u,v)] =  Path_Pre[(w_tmp,u)]+[v]
                        Bigram_Cur.append((u,v))

            Pi_Pre = dict(Pi_Cur)
            Bigram_Pre = list(Bigram_Cur)
            Path_Pre = dict(Path_Cur)
            cur_len += 1

        st = ''
        bigram_max = Bigram_Pre[0]
        for bigram in Bigram_Pre:
            if Pi_Cur[bigram] > Pi_Cur[bigram_max]:
                bigram = bigram_max
        for i,tag in enumerate(Path_Cur[bigram_max][2:-1]):
            st = st + sentence[i]+'/'+tag+' '
        tagged.append(st.strip()+'\n')
        if len(tagged) % 100 == 0:
            print(len(tagged))

    return tagged

# This function takes the output of viterbi() and outputs it to file
def fetch_output(tagged, filename):
    outfile = open(filename, 'w', encoding='utf-8')
    for sentence in tagged:
        outfile.write(sentence)
    outfile.close()


OUTPUT_PATH = 'output/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

def seperate_dev_tags(dev_sentences):
    penn_words = []
    penn_tags = []

    for item in dev_sentences:
        words_tmp = []
        tags_tmp = []
        sentence = item.strip().split(' ')
        for token in sentence:
            words_tmp.append(token.rsplit('/',1)[0])
            tags_tmp.append(token.rsplit('/',1)[1])
        penn_words.append(words_tmp)
        penn_tags.append(tags_tmp)

    return penn_words, penn_tags

def evaluate(test_penn_tags, test_tags, padding):
    # Flattening all original tags
    flat_orig_tags = []
    for orig_tag in test_penn_tags:
        flat_orig_tags.extend(orig_tag[2:-1])

    # Flattening all original tags
    reduce_orig_tags = []
    for predict_tag in test_tags:
        reduce_orig_tags.extend(predict_tag[2:-1])

    # assert len(flat_orig_tags) == len(reduce_orig_tags)
    print(classification_report(flat_orig_tags, reduce_orig_tags))

def modify_tags(total_tags, choice):
    reconstruct_tag = []
    for list_of_tags in total_tags:
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
    # open penn training data
    with open('midsem_data/penn-data.json', 'r') as infile:
        json_data = json.load(infile)

    # split words and tags, and add start and stop symbols (question 1)
    total_words, total_tags = fetch_data(json_data)
    padding = 'result'
    
    # calculate tag trigram probabilities (question 2)
    transition_dict = fetch_transitions_probs(total_tags)

    # calculate list of words with count > 5 (question 3)
    known_words = compute_known(total_words)

    # get a version of penn_words with rare words replace with '_RARE_' (question 3)
    penn_words_rare = swap_unique(total_words, known_words)

    # calculate emission probabilities (question 4)
    eigen, list_of_tags = fetch_emission_prob(penn_words_rare, total_tags)

    # delete unneceessary data
    del penn_words_rare

    # Test Data
    t_sentence = 'That former Sri Lanka skipper and ace batsman Aravinda De Silva is a man of few words was very much evident on Wednesday when the legendary batsman , who has always let his bat talk , struggled to answer a barrage of questions at a function to_F promote.'
    t_words = t_sentence.split(' ')
    test_put = [START_CHAR, START_CHAR]
    test_put.extend(t_words)
    test_put.append(STOP_CHAR)
    test_data = [test_put]

    tag_out = viterbi(test_data, list_of_tags, known_words, transition_dict, eigen)
    fetch_output(tag_out, OUTPUT_PATH + 'tagged'+padding+'.txt')

if __name__ == "__main__": 
    main()
