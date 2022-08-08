'''
NLP Assignment 2
1801cs31, 1801cs33
'''
import sys
import os
import json
import math
import numpy
from sklearn.metrics import classification_report

START_SYMBOL = '*'
STOP_SYMBOL = 'STOP'
RARE_SYMBOL = '$RARE$'
RARE_WORD_MAX_FREQ = 5
LOG_PROB_OF_ZERO = -1000


# Split dataset into list of words and tags, padded with the start and stop tokens
def get_words_tags_trigram(data):
    all_words, all_tags = [], []
    temp_words, temp_tags = [START_SYMBOL, START_SYMBOL], [START_SYMBOL, START_SYMBOL]
    for word_tag in data:
        clean_word_tag = word_tag.strip('\n')
        if clean_word_tag == '':
            temp_words.append(STOP_SYMBOL)
            temp_tags.append(STOP_SYMBOL)
            all_words.append(temp_words)
            all_tags.append(temp_tags)
            temp_words, temp_tags = [START_SYMBOL, START_SYMBOL], [START_SYMBOL, START_SYMBOL]
        else:
            split_data = clean_word_tag.split('\t')
            word, tag = split_data[0], split_data[1]
            temp_words.append(word)
            temp_tags.append(tag)
    return all_words, all_tags

# Calculate the transition probabilities
def get_bigrams(item):
    bigrams_tmp = []
    for i in range(len(item)-1):
        bigrams_tmp.append((item[i], item[i+1]))
    return bigrams_tmp

def get_trigrams(item):
    trirams_tmp = []
    for i in range(len(item)-2):
        trirams_tmp.append((item[i], item[i+1], item[i+2]))
    return trirams_tmp



def get_transitions_probs(ner_tags):
    transition_values = {}

    bigram_count = {}
    trigram_count = {}

    for item in ner_tags:
        bigram_tmp = get_bigrams(item)
        trigram_tmp = get_trigrams(item)

        for bigram in bigram_tmp:
            if bigram in bigram_count:
                bigram_count[bigram] += 1
            else:
                bigram_count[bigram] = 1

        for trigram in trigram_tmp:
            if trigram in trigram_count:
                trigram_count[trigram] += 1
            else:
                trigram_count[trigram] =1

    for trigram in trigram_count:
        transition_values[trigram] = math.log(trigram_count[trigram], 2) - math.log(bigram_count[trigram[:2]],2)
    return transition_values

# This function takes output from get_transitions_probs() and outputs it in the proper format
def transitions_output(transition_values, filename):
    outfile = open(filename, "w")
    trigrams = transition_values.keys()
    trigrams = sorted(trigrams)  
    for trigram in trigrams:
        output = " ".join(['TRIGRAM', trigram[0], trigram[1], trigram[2], str(transition_values[trigram])])
        outfile.write(output + '\n')
    outfile.close()


# All Known Words: Which occur more than 5 times
def calc_known(ner_words):
    known_words = set([])
    
    word_count = {}

    for item in ner_words:
        for word in item:
            if word in word_count:
                word_count[word] += 1
            else:
                word_count[word] = 1

    for item in word_count:
        if word_count[item] > RARE_WORD_MAX_FREQ:
            known_words.add(item)
	    
    return known_words

# create dataset by replacing rare words with rare token
def replace_rare(ner_words, known_words):
    ner_words_rare = []

    for item in ner_words:
        tmp = []
        for word in item:
            if word in known_words:
                tmp.append(word)
            else:
                tmp.append(RARE_SYMBOL)
        ner_words_rare.append(tmp)

    return ner_words_rare

# This function takes the ouput from replace_rare and outputs it to a file
def s3_output(rare, filename):
    outfile = open(filename, 'w')
    for sentence in rare:
        outfile.write(' '.join(sentence[2:-1]) + '\n')
    outfile.close()


# Emission Probability
def get_emission_probs(ner_words_rare, ner_tags):
    e_values = {}
    taglist = set([])

    tag_count = {}
    word_tag_count = {}
    for i in range(len(ner_tags)):
        sentence = ner_words_rare[i]
        tags = ner_tags[i]
        for j in range(1, len(tags)):
            word = sentence[j]
            tag = tags[j]
            tag_prev = tags[j-1]
            if (word, tag, tag_prev) in word_tag_count:
                word_tag_count[(word,tag,tag_prev)] += 1
            else:
                word_tag_count[(word,tag,tag_prev)] =1
            if (tag, tag_prev) in tag_count:
                tag_count[tag, tag_prev] += 1
            else:
                tag_count[tag, tag_prev] = 1

    for item in word_tag_count:
        e_values[item] = math.log(word_tag_count[item],2) - math.log(tag_count[item[1:]],2)

    for item in tag_count:
        taglist.add(item[0])
        taglist.add(item[1])

    return e_values, taglist

# This function takes the output from get_emission_probs() and outputs it
def emissions_output(e_values, filename):
    outfile = open(filename, "w")
    emissions = e_values.keys()
    emissions = sorted(emissions)  
    for item in emissions:
        output = " ".join([item[0], item[1], item[2], str(e_values[item])])
        outfile.write(output + '\n')
    outfile.close()


# Main function for Viterbi Decoding
def viterbi_decode(ner_dev_words, taglist, known_words, transition_values, e_values):
    tagged = []

    print(len(ner_dev_words))

    Pi_Init = {}
    for u in taglist:
        for v in taglist:
            Pi_Init[(u,v)] = LOG_PROB_OF_ZERO

    for ctr, item in enumerate(ner_dev_words):

        sentence = item + [STOP_SYMBOL]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[(START_SYMBOL, START_SYMBOL)] = [START_SYMBOL, START_SYMBOL]
        Bigram_Pre = [(START_SYMBOL, START_SYMBOL)]           

        Pi_Pre = {}
        Pi_Pre[(START_SYMBOL, START_SYMBOL)] = 0
        
        # For each sentence
        while cur_len < n:
            Path_Cur = {}
            Bigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = STOP_SYMBOL
                tagspace = [STOP_SYMBOL]
            else:
                word = sentence[cur_len]
                if word not in known_words:
                    word = RARE_SYMBOL
                tagspace = list(taglist)
            converted_sentence.append(word)

            for v in tagspace:
                for u in taglist:
                    emi_tmp = (word, v, u)
                    if emi_tmp not in e_values:
                        e_values[emi_tmp] = LOG_PROB_OF_ZERO
                    w_tmp = ''
                    for w in taglist:
                        if (w,u) not in Bigram_Pre:
                            continue
                        trigram_cur = (w,u,v)
                        if trigram_cur not in transition_values:
                            transition_values[trigram_cur] = LOG_PROB_OF_ZERO
                        if (u,v) not in Pi_Cur:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+transition_values[trigram_cur]+e_values[emi_tmp]
                            w_tmp = w
                        elif Pi_Pre[(w,u)]+transition_values[trigram_cur]+e_values[emi_tmp] > Pi_Cur[(u,v)]:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+transition_values[trigram_cur]+e_values[emi_tmp]
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

# This function takes the output of viterbi_decode() and outputs it to file
def tagged_outputs(tagged, filename):
    outfile = open(filename, 'w', encoding='utf-8')
    for sentence in tagged:
        outfile.write(sentence)
    outfile.close()


OUTPUT_PATH = 'op_trigram_with_context/'
EXTRA_PATH = 'op_trigram_with_context/probabilities/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)
if not os.path.exists(EXTRA_PATH):
    os.mkdir(EXTRA_PATH)

def split_devtags(dev_sentences):
    ner_words = []
    ner_tags = []

    for item in dev_sentences:
        words_tmp = []
        tags_tmp = []
        sentence = item.strip().split(' ')
        for token in sentence:
            words_tmp.append(token.rsplit('/',1)[0])
            tags_tmp.append(token.rsplit('/',1)[1])
        ner_words.append(words_tmp)
        ner_tags.append(tags_tmp)

    return ner_words, ner_tags

# Function for printing accuracy 
def print_accuracy(test_ner_tags, test_tags, suffix):
    # Flattening all original tags
    flat_orig_tags = []
    for orig_tag in test_ner_tags:
        flat_orig_tags.extend(orig_tag[2:-1])

    # Flattening all original tags
    flat_predict_tags = []
    for predict_tag in test_tags:
        flat_predict_tags.extend(predict_tag[2:-1])

    assert len(flat_orig_tags) == len(flat_predict_tags)
    print(classification_report(flat_orig_tags, flat_predict_tags))
    output_file = os.path.join(OUTPUT_PATH, 'classification_matrix'+suffix+'.txt')
    with open(output_file, 'w') as outfile:
        print(classification_report(flat_orig_tags, flat_predict_tags), file=outfile)

# Convert all tags based on choice (BIO or Fine-Grained)
def convert_tags(all_tags, choice):
    reconstruct_tag = []
    for taglist in all_tags:
        new_tags = []
        for tag in taglist:
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
    # open ner training data
    print("MENU")
    print("1. Consider BIO Tags")
    print("2. Consider Fine-Grained Tags")
    print("Enter 1 or 2 for choice:", end=' ')
    choice = input()

    with open('wnut16/train.txt', encoding='utf-8') as infile:
        train_data = infile.readlines()
    with open('wnut16/dev.txt', encoding='utf-8') as infile:
        val_data = infile.readlines()    
    with open('wnut16/test.txt', encoding='utf-8') as infile:
        test_data = infile.readlines()

    # split words and tags, and add start and stop symbols (question 1)
    train_words, train_tags = get_words_tags_trigram(train_data)
    val_words, val_tags = get_words_tags_trigram(val_data)
    test_words, test_tags = get_words_tags_trigram(test_data)

    #TODO: Change Tags
    if choice=="1":
        suffix = '_BIO_tags'
    elif choice=="2":
        suffix = '_Fine_tags'
    train_tags = convert_tags(train_tags, choice)
    val_tags = convert_tags(val_tags, choice)
    test_tags = convert_tags(test_tags, choice)
        
    
    # computes transition probs 
    transition_values = get_transitions_probs(train_tags)

    transitions_output(transition_values, EXTRA_PATH + 'transition_probs'+suffix+'.txt')

    known_words = calc_known(train_words)

    ner_words_rare = replace_rare(train_words, known_words)

    e_values, taglist = get_emission_probs(ner_words_rare, train_tags)

    # computes emission probs 
    emissions_output(e_values, EXTRA_PATH + "emission_probs"+suffix+'.txt')

    del ner_words_rare

    viterbi_tagged = viterbi_decode(test_words, taglist, known_words, transition_values, e_values)

    tagged_outputs(viterbi_tagged, OUTPUT_PATH + 'test_tagged'+suffix+'.txt')

    test_words, test_predicted_tags = split_devtags(viterbi_tagged)
    print_accuracy(test_tags, test_predicted_tags, suffix)

if __name__ == "__main__": 
    main()
