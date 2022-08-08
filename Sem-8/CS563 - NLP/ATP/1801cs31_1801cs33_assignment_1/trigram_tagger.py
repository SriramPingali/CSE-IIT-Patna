import sys
import os
import json
import math
import numpy
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
from collections import Counter

START_SYMBOL = '*'
STOP_SYMBOL = 'STOP'
RARE_SYMBOL = '_RARE_'
RARE_WORD_MAX_FREQ = 5
LOG_PROB_OF_ZERO = -1000

# Split dataset into list of words and tags, padded with the start and stop tokens
def split_wordtags(json_data):
    penn_words = []
    penn_tags = []
    
    for sentence in json_data:
        words = sentence[0].split(' ')
        words = [START_SYMBOL, START_SYMBOL] + words + [STOP_SYMBOL]
        tags = [START_SYMBOL, START_SYMBOL] + sentence[1] + [STOP_SYMBOL]
        assert len(words) == len(tags)
        penn_words.append(words)
        penn_tags.append(tags)
    return penn_words, penn_tags


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



def get_transitions_probs(penn_tags):
    transition_values = {}

    bigram_count = {}
    trigram_count = {}

    for item in penn_tags:
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
def s2_output(transition_values, filename):
    outfile = open(filename, "w")
    trigrams = transition_values.keys()
    trigrams = sorted(trigrams)  
    for trigram in trigrams:
        output = " ".join(['TRIGRAM', trigram[0], trigram[1], trigram[2], str(transition_values[trigram])])
        outfile.write(output + '\n')
    outfile.close()


# TODO: IMPLEMENT THIS FUNCTION
# Takes the words from the training data and returns a set of all of the words that occur more than 5 times (use RARE_WORD_MAX_FREQ)
# penn_words is a python list where every element is a python list of the words of a particular sentence.
# Note: words that appear exactly 5 times should be considered rare!
def calc_known(penn_words):
    known_words = set([])
    
    word_count = {}

    for item in penn_words:
        for word in item:
            if word in word_count:
                word_count[word] += 1
            else:
                word_count[word] = 1

    for item in word_count:
        if word_count[item] > RARE_WORD_MAX_FREQ:
            known_words.add(item)
	    
    return known_words

# TODO: IMPLEMENT THIS FUNCTION
# Takes the words from the training data and a set of words that should not be replaced for '_RARE_'
# Returns the equivalent to penn_words but replacing the unknown words by '_RARE_' (use RARE_SYMBOL constant)
def replace_rare(penn_words, known_words):
    penn_words_rare = []

    for item in penn_words:
        tmp = []
        for word in item:
            if word in known_words:
                tmp.append(word)
            else:
                tmp.append(RARE_SYMBOL)
        penn_words_rare.append(tmp)

    return penn_words_rare

# This function takes the ouput from replace_rare and outputs it to a file
def s3_output(rare, filename):
    outfile = open(filename, 'w')
    for sentence in rare:
        outfile.write(' '.join(sentence[2:-1]) + '\n')
    outfile.close()


# TODO: IMPLEMENT THIS FUNCTION
# Calculates emission probabilities and creates a set of all possible tags
# The first return value is a python dictionary where each key is a tuple in which the first element is a word
# and the second is a tag, and the value is the log probability of the emission of the word given the tag
# The second return value is a set of all possible tags for this data set
def get_emission_probs(penn_words_rare, penn_tags):
    e_values = {}
    taglist = set([])

    tag_count = {}
    word_tag_count = {}
    for i in range(len(penn_tags)):
        sentence = penn_words_rare[i]
        tags = penn_tags[i]
        for j in range(len(tags)):
            word = sentence[j]
            tag = tags[j]
            if (word, tag) in word_tag_count:
                word_tag_count[(word,tag)] += 1
            else:
                word_tag_count[(word,tag)] =1
            if tag in tag_count:
                tag_count[tag] += 1
            else:
                tag_count[tag] = 1

    for item in word_tag_count:
        e_values[item] = math.log(word_tag_count[item],2) - math.log(tag_count[item[1]],2)

    for item in tag_count:
        taglist.add(item)

    return e_values, taglist

# This function takes the output from calc_emissions() and outputs it
def s4_output(e_values, filename):
    outfile = open(filename, "w")
    emissions = e_values.keys()
    emissions = sorted(emissions)  
    for item in emissions:
        output = " ".join([item[0], item[1], str(e_values[item])])
        outfile.write(output + '\n')
    outfile.close()


# TODO: IMPLEMENT THIS FUNCTION
# This function takes data to tag (penn_dev_words), a set of all possible tags (taglist), a set of all known words (known_words),
# trigram probabilities (transition_values) and emission probabilities (e_values) and outputs a list where every element is a tagged sentence 
# (in the WORD/TAG format, separated by spaces and with a newline in the end, just like our input tagged data)
# penn_dev_words is a python list where every element is a python list of the words of a particular sentence.
# taglist is a set of all possible tags
# known_words is a set of all known words
# transition_values is from the return of get_transitions_probs()
# e_values is from the return of calc_emissions()
# The return value is a list of tagged sentences in the format "WORD/TAG", separated by spaces. Each sentence is a string with a 
# terminal newline, not a list of tokens. Remember also that the output should not contain the "_RARE_" symbol, but rather the
# original words of the sentence!
def viterbi_decode(penn_dev_words, taglist, known_words, transition_values, e_values, most_frequent_tag):
    tagged = []

    print(len(penn_dev_words))

    Pi_Init = {}
    for u in taglist:
        for v in taglist:
            Pi_Init[(u,v)] = LOG_PROB_OF_ZERO

    for item in penn_dev_words:

        sentence = item + [STOP_SYMBOL]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[(START_SYMBOL, START_SYMBOL)] = [START_SYMBOL, START_SYMBOL]
        Bigram_Pre = [(START_SYMBOL, START_SYMBOL)]           

        Pi_Pre = {}
        Pi_Pre[(START_SYMBOL, START_SYMBOL)] = 0
        
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
                emi_tmp = (word, v)
                if emi_tmp not in e_values:
                    continue
                for u in taglist:
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
            #TODO: Add most frequent tag here
            if converted_sentence[i] == '_RARE_':
                st = st + sentence[i]+'/'+most_frequent_tag+' '
            else:
                st = st + sentence[i]+'/'+tag+' '
        tagged.append(st.strip()+'\n')
        if len(tagged) % 100 == 0:
            print(len(tagged))

    return tagged

def get_most_frequent_tag(penn_tags):
    all_tags = []
    for tags in penn_tags:
        all_tags.extend(tags)
    tag_counts = dict(Counter(all_tags))
    sort_counts = sorted(tag_counts.items(), key=lambda item: item[1], reverse=True)
    return sort_counts[0][0]

# This function takes the output of viterbi_decode() and outputs it to file
def s5_output(tagged, filename):
    outfile = open(filename, 'w')
    for sentence in tagged:
        outfile.write(sentence)
    outfile.close()


OUTPUT_PATH = 'output/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

def convert_tags(all_tags):
    noun_tags = ['NN', 'NNS', 'NNP', 'NNPS', 'PRP', 'PRP$', 'WP', 'WP$']
    verb_tags = ['VB', 'VBD', 'VBG', 'VBN', 'VBP', 'VBZ']
    adj_adv_tags = ['JJ', 'JJR', 'JJS', 'RB', 'RBR', 'RBS', 'WRB']
    reconstruct_tag = []
    for tags in all_tags:
        new_tags = []
        for tag in tags:
            if tag == '*' or tag == 'STOP':
                new_tags.append(tag)
            elif tag in noun_tags:
                new_tags.append('N')
            elif tag in verb_tags:
                new_tags.append('V')
            elif tag in adj_adv_tags:
                new_tags.append('A')
            else:
                new_tags.append('O')
        reconstruct_tag.append(new_tags)
    return reconstruct_tag

def split_devtags(dev_sentences):
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

def print_accuracy(test_penn_tags, test_tags, suffix):
    # Flattening all original tags
    flat_orig_tags = []
    for orig_tag in test_penn_tags:
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

def main():
    # open penn training data
    print("MENU")
    print("1. Consider 36 POS Tags")
    print("2. Consider Collapsed 4 POS Tags")
    print("Enter 1 or 2 for choice:", end=' ')
    choice = input()

    with open('penn-data.json', 'r') as infile:
        json_data = json.load(infile)

    # split words and tags, and add start and stop symbols (question 1)
    all_penn_words, all_penn_tags = split_wordtags(json_data)
    #TODO: Change Tags
    if choice=="1":
        suffix = '_36tags'
    elif choice=="2":
        all_penn_tags = convert_tags(all_penn_tags)
        suffix = '_4tags'

    most_frequent_tag = get_most_frequent_tag(all_penn_tags)
    train_penn_words, test_penn_words, train_penn_tags, test_penn_tags = train_test_split(all_penn_words, all_penn_tags, test_size=0.2, random_state=43)
    
    # calculate tag trigram probabilities (question 2)
    transition_values = get_transitions_probs(train_penn_tags)

    # question 2 output
    s2_output(transition_values, OUTPUT_PATH + 'S2'+suffix+'.txt')

    # calculate list of words with count > 5 (question 3)
    known_words = calc_known(train_penn_words)

    # get a version of penn_words with rare words replace with '_RARE_' (question 3)
    penn_words_rare = replace_rare(train_penn_words, known_words)

    # question 3 output
    s3_output(penn_words_rare, OUTPUT_PATH + "S3"+suffix+'.txt')

    # calculate emission probabilities (question 4)
    e_values, taglist = get_emission_probs(penn_words_rare, train_penn_tags)

    # question 4 output
    s4_output(e_values, OUTPUT_PATH + "S4"+suffix+'.txt')

    # delete unneceessary data
    del penn_words_rare

    # do viterbi_decode on penn_dev_words (question 5)
    viterbi_tagged = viterbi_decode(test_penn_words, taglist, known_words, transition_values, e_values, most_frequent_tag)

    # question 5 output
    s5_output(viterbi_tagged, OUTPUT_PATH + 'S5'+suffix+'.txt')

    # Calculating accuracy
    test_words, test_predicted_tags = split_devtags(viterbi_tagged)
    print_accuracy(test_penn_tags, test_predicted_tags, suffix)


if __name__ == "__main__": 
    main()
