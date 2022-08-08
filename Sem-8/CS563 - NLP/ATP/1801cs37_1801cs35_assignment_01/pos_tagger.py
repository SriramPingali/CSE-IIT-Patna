import sys
import os
import json
import math
import numpy
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
from collections import Counter

strt_ch = '*'
stp_ch = 'STOP'
rare_ch = '_RARE_'
rare_word_max = 5
log_prob_zero = -1000

# Receives a list of tagged sentences and processes each sentence to generate a list of words and a list of tags.
# Each sentence is a string of space separated "WORD/TAG" tokens, with a newline character in the end.
# Remember to include start and stop symbols in yout returned lists, as defined by the constants strt_ch and stp_ch.
# brown_words (the list of words) should be a list where every element is a list of the tags of a particular sentence.
# brown_tags (the list of tags) should be a list where every element is a list of the tags of a particular sentence.
def tag_split(json_data):
    brown_tags = []
    brown_words = []
    
    for sentence in json_data:
        words = sentence[0].split(' ')
        words = [strt_ch, strt_ch] + words + [stp_ch]
        tags = [strt_ch, strt_ch] + sentence[1] + [stp_ch]
        assert len(words) == len(tags)
        brown_words.append(words)
        brown_tags.append(tags)
    return(brown_words, brown_tags)


# This function takes tags from the training data and calculates tag trigram probabilities.
# It returns a python dictionary where the keys are tuples that represent the tag trigram, and the values are the log probability of that trigram
def get_bigrams_list(item):
    # bigrams_temp = []
    # for i in range(len(item)-1):
    #     bigrams_temp.append((item[i], item[i+1]))
    return([(item[i], item[i+1]) for i in range(len(item) - 1)])

def get_trigrams_list(item):
    # trigrams_temp = [(item[i], item[i+1], item[i+2]) for i in range(len(item) - 2)]
    # for i in range(len(item)-2):
    #     trigrams_temp.append((item[i], item[i+1], item[i+2]))
    return([(item[i], item[i+1], item[i+2]) for i in range(len(item) - 2)])


def get_q_trigram(brown_tags):
    q_values = {}

    num_bigrams = {}
    num_trigrams = {}

    for item in brown_tags:

        bigram_tmp = get_bigrams_list(item)
        trigram_tmp = get_trigrams_list(item)

        for bigram in bigram_tmp:
            if bigram not in num_bigrams:
                num_bigrams[bigram] = 1
            else:
                num_bigrams[bigram] += 1

        for trigram in trigram_tmp:
            if trigram not in num_trigrams:
                num_trigrams[trigram] = 1
            else:
                num_trigrams[trigram] += 1

    for trigram in num_trigrams:
        q_values[trigram] = math.log(num_trigrams[trigram], 2) - math.log(num_bigrams[trigram[:2]],2)
    return(q_values)

# This function takes output from get_q_trigram() and outputs it in the proper format
def q2_otpt(q_values, filename):
    output = open(filename, "w")
    trigrams = q_values.keys()
    trigrams = sorted(trigrams)  
    for trigram in trigrams:
        out = " ".join(['TRIGRAM', trigram[0], trigram[1], trigram[2], str(q_values[trigram])])
        output.write(out + '\n')
    output.close()


# Takes the words from the training data and returns a set of all of the words that occur more than 5 times (use rare_word_max)
# brown_words is a python list where every element is a python list of the words of a particular sentence.
# Note: words that appear exactly 5 times should be considered rare!
def compute_known(brown_words):
    known_words = set([])
    
    word_count = {}

    for item in brown_words:
        for word in item:
            if word not in word_count:
                word_count[word] = 1
            else:
                word_count[word] += 1

    for item in word_count:
        if word_count[item] > rare_word_max:
            known_words.add(item)
	    
    return(known_words)

# Takes the words from the training data and a set of words that should not be replaced for '_RARE_'
# Returns the equivalent to brown_words but replacing the unknown words by '_RARE_' (use rare_ch constant)
def replace_rare_words(brown_words, known_words):
    brown_words_rare = []

    for item in brown_words:
        # tmp = [rare_ch if word not in known_words for word in item else word]
        # for word in item:
        #     if word not in known_words:
        #         tmp.append(rare_ch)
        #     else:
        #         tmp.append(word)
        brown_words_rare.append([rare_ch if word not in known_words else word for word in item])

    return brown_words_rare

# This function takes the ouput from replace_rare_words and outputs it to a file
def q3_otpt(rare, filename):
    output = open(filename, 'w')
    for sentence in rare:
        output.write(' '.join(sentence[2:-1]) + '\n')
    output.close()


# Calculates emission probabilities and creates a set of all possible tags
# The first return value is a python dictionary where each key is a tuple in which the first element is a word
# and the second is a tag, and the value is the log probability of the emission of the word given the tag
# The second return value is a set of all possible tags for this data set
def compute_emission(brown_words_rare, brown_tags):
    e_values = {}
    taglist = set([])

    tag_count = {}
    word_tag_count = {}
    for i in range(len(brown_tags)):
        sentence = brown_words_rare[i]
        tags = brown_tags[i]

        for j in range(len(tags)):
            word = sentence[j]
            tag = tags[j]

            if (word, tag) not in word_tag_count:
                word_tag_count[(word,tag)] =1
            else:
            	word_tag_count[(word,tag)] += 1
            
            if tag not in tag_count:
                tag_count[tag] = 1
            else:
            	tag_count[tag] += 1

    for item in word_tag_count:
        e_values[item] = math.log(word_tag_count[item],2) - math.log(tag_count[item[1]],2)

    for item in tag_count:
        taglist.add(item)

    return(e_values, taglist)

# This function takes the output from compute_emissions() and outputs it
def q4_otpt(e_values, filename):
    output = open(filename, "w")
    emissions = e_values.keys()
    emissions = sorted(emissions)  
    for item in emissions:
        out = " ".join([item[0], item[1], str(e_values[item])])
        output.write(out + '\n')
    output.close()


# This function takes data to tag (brown_dev_words), a set of all possible tags (taglist), a set of all known words (known_words),
# trigram probabilities (q_values) and emission probabilities (e_values) and outputs a list where every element is a tagged sentence 
# (in the WORD/TAG format, separated by spaces and with a newline in the end, just like our input tagged data)
# brown_dev_words is a python list where every element is a python list of the words of a particular sentence.
# taglist is a set of all possible tags
# known_words is a set of all known words
# q_values is from the return of get_q_trigram()
# e_values is from the return of compute_emissions()
# The return value is a list of tagged sentences in the format "WORD/TAG", separated by spaces. Each sentence is a string with a 
# terminal newline, not a list of tokens. Remember also that the output should not contain the "_RARE_" symbol, but rather the
# original words of the sentence!
def viterbi(brown_dev_words, taglist, known_words, q_values, e_values, most_frequent_tag):
    tagged = []

    print(len(brown_dev_words))

    Pi_Init = {}
    for u in taglist:
        for v in taglist:
            Pi_Init[(u,v)] = log_prob_zero

    for item in brown_dev_words:

        sentence = item + [stp_ch]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[(strt_ch, strt_ch)] = [strt_ch, strt_ch]
        Bigram_Pre = [(strt_ch, strt_ch)]           

        Pi_Pre = {}
        Pi_Pre[(strt_ch, strt_ch)] = 0
        
        while cur_len < n:
            Path_Cur = {}
            Bigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = stp_ch
                tagspace = [stp_ch]
            else:
                word = sentence[cur_len]
                if word not in known_words:
                    word = rare_ch
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
                        if trigram_cur not in q_values:
                            q_values[trigram_cur] = log_prob_zero
                        if (u,v) not in Pi_Cur:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+q_values[trigram_cur]+e_values[emi_tmp]
                            w_tmp = w
                        elif Pi_Pre[(w,u)]+q_values[trigram_cur]+e_values[emi_tmp] > Pi_Cur[(u,v)]:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+q_values[trigram_cur]+e_values[emi_tmp]
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
            if converted_sentence[i] == '_RARE_':
                st = st + sentence[i]+'/'+most_frequent_tag+' '
            else:
                st = st + sentence[i]+'/'+tag+' '
        tagged.append(st.strip()+'\n')
        if len(tagged) % 100 == 0:
            print(len(tagged))

    return tagged

def most_freq_tag(brown_tags):
    all_tags = []
    for tags in brown_tags:
        all_tags.extend(tags)
    tag_counts = dict(Counter(all_tags))
    sort_counts = sorted(tag_counts.items(), key=lambda item: item[1], reverse=True)
    return sort_counts[0][0]

# This function takes the output of viterbi() and outputs it to file
def q5_otpt(tagged, filename):
    output = open(filename, 'w')
    for sentence in tagged:
        output.write(sentence)
    output.close()


OUTPUT_PATH = 'outputs/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

def replace_tags(all_tags):
    noun_tags = ['NN', 'NNS', 'NNP', 'NNPS', 'PRP', 'PRP$', 'WP', 'WP$']
    verb_tags = ['VB', 'VBD', 'VBG', 'VBN', 'VBP', 'VBZ']
    adj_adv_tags = ['JJ', 'JJR', 'JJS', 'RB', 'RBR', 'RBS', 'WRB']
    reconstruct_tag = []
    for tags in all_tags:
        new_tags = []
        for tag in tags:
            if tag == '*' or tag == 'STOP':
                new_tags.append(tag)
            elif tag in verb_tags:
                new_tags.append('V')
            elif tag in adj_adv_tags:
                new_tags.append('A')
            elif tag in noun_tags:
                new_tags.append('N')
            else:
                new_tags.append('O')
        reconstruct_tag.append(new_tags)
    return reconstruct_tag

def seperate_devtags(dev_sentences):
    brown_words = []
    brown_tags = []

    for item in dev_sentences:
        words_tmp = []
        tags_tmp = []
        sentence = item.strip().split(' ')
        for token in sentence:
            words_tmp.append(token.rsplit('/',1)[0])
            tags_tmp.append(token.rsplit('/',1)[1])
        brown_words.append(words_tmp)
        brown_tags.append(tags_tmp)

    return brown_words, brown_tags

def give_accuracy(test_brown_tags, test_tags, suffix):
    # Flattening all original tags
    flat_orig_tags = []
    for orig_tag in test_brown_tags:
        flat_orig_tags.extend(orig_tag[2:-1])

    # Flattening all original tags
    flat_predict_tags = []
    for predict_tag in test_tags:
        flat_predict_tags.extend(predict_tag[2:-1])

    assert len(flat_orig_tags) == len(flat_predict_tags)
    print(classification_report(flat_orig_tags, flat_predict_tags))
    output_file = os.path.join(OUTPUT_PATH, 'classification_matrix'+suffix+'.txt')
    with open(output_file, 'w') as output:
        print(classification_report(flat_orig_tags, flat_predict_tags), file=output)

if __name__ == "__main__": 
    # open Brown training data
    print("Choices")
    print("1. 36 POS Tags")
    print("2. 4 POS Tags")
    print("Enter choice:", end=' ')
    choice = input()

    with open('penn-data.json', 'r') as infile:
        json_data = json.load(infile)

    # split words and tags, and add start and stop symbols (question 1)
    all_brown_words, all_brown_tags = tag_split(json_data)
    if choice=="1":
        suffix = '_tag_36'
    elif choice=="2":
        all_brown_tags = replace_tags(all_brown_tags)
        suffix = '_tag_4'

    most_frequent_tag = most_freq_tag(all_brown_tags)
    train_brown_words, test_brown_words, train_brown_tags, test_brown_tags = train_test_split(all_brown_words, all_brown_tags, test_size=0.2, random_state=43)
    
    # calculate tag trigram probabilities (question 2)
    q_values = get_q_trigram(train_brown_tags)

    # question 2 output
    q2_otpt(q_values, OUTPUT_PATH + 'B2'+suffix+'.txt')

    # calculate list of words with count > 5 (question 3)
    known_words = compute_known(train_brown_words)

    # get a version of brown_words with rare words replace with '_RARE_' (question 3)
    brown_words_rare = replace_rare_words(train_brown_words, known_words)

    # question 3 output
    q3_otpt(brown_words_rare, OUTPUT_PATH + "B3"+suffix+'.txt')

    # calculate emission probabilities (question 4)
    e_values, taglist = compute_emission(brown_words_rare, train_brown_tags)

    # question 4 output
    q4_otpt(e_values, OUTPUT_PATH + "B4"+suffix+'.txt')

    # delete unneceessary data
    del brown_words_rare

    # do viterbi on brown_dev_words (question 5)
    viterbi_tagged = viterbi(test_brown_words, taglist, known_words, q_values, e_values, most_frequent_tag)

    # question 5 output
    q5_otpt(viterbi_tagged, OUTPUT_PATH + 'B5'+suffix+'.txt')

    # Calculating accuracy
    test_words, test_predicted_tags = seperate_devtags(viterbi_tagged)
    give_accuracy(test_brown_tags, test_predicted_tags, suffix)