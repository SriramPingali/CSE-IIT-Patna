import os
import json
import math

START = '*'
STOP = 'STOP'
RARE = '_RARE_'
RARE_FREQ_MAX = 5
ZERO_LOG_PROB = -1000

OUTPUT_PATH = 'output/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

# Generate a list of words and a list of tags from a list of tagged sentences
def get_data(data):
    total_words, total_tags = [], []
    for sentence in data:
        words = sentence[0].split(' ')
        words = [START, START] + words + [STOP]
        tags = [START, START] + sentence[1] + [STOP]

        assert len(words) == len(tags)
        total_words.append(words)
        total_tags.append(tags)
    return total_words, total_tags

# Calculate Bigrams
def bigrams(item):
    return [(item[i], item[i+1]) for i in range(len(item)-1)]

# Calculate Trigrams
def trigrams(item):
    return [(item[i], item[i+1], item[i+2]) for i in range(len(item)-2)]

# Calculate Transition Probabilities
def calc_trans_pb(penn_tags):
    tr_values = {}
    bi_tally = {}
    tri_tally = {}

    for item in penn_tags:
        t_bi = bigrams(item)
        t_tri = trigrams(item)
        for bigram in t_bi:
            if bigram in bi_tally:
                bi_tally[bigram] += 1
            if bigram not in bi_tally:
                bi_tally[bigram] = 1

        for trigram in t_tri:
            if trigram in tri_tally:
                tri_tally[trigram] += 1
            if trigram not in tri_tally:
                tri_tally[trigram] =1

    for trigram in tri_tally:
        tr_values[trigram] = math.log(tri_tally[trigram], 2) - math.log(bi_tally[trigram[:2]],2)
    return tr_values

# Return a set of all words in training data that occur more than RARE_MAX_FREQ times
def get_known_words(words_penn):
    known_words_set = set([])
    word_tally = {}
    for item in words_penn:
        for word in item:
            if word in word_tally:
                word_tally[word] += 1
            if word not in word_tally:
                word_tally[word] = 1

    for item in word_tally:
        if word_tally[item] > RARE_FREQ_MAX:
            known_words_set.add(item)
	    
    return known_words_set

# Replace unknown words by '_RARE_'
def swap_rare_words(words_penn, known_words_set):
    words_penn_rare = []
    for item in words_penn:
        tlist = []
        for word in item:
            if word in known_words_set:
                tlist.append(word)
            if word not in known_words_set:
                tlist.append(RARE)
        words_penn_rare.append(tlist)

    return words_penn_rare

# Calculate emission probabilities and a set of all possible tags
def calc_emi_pb(words_penn_rare, penn_tags):
    eigen_vals = {}
    tag_list = set([])

    tag_tally = {}
    word_tag_tally = {}
    for i in range(len(penn_tags)):
        sentence = words_penn_rare[i]
        tags = penn_tags[i]
        for j in range(1, len(tags)):
            word = sentence[j]
            tag = tags[j]
            tag_prev = tags[j-1]
            if (word, tag, tag_prev) in word_tag_tally:
                word_tag_tally[(word,tag,tag_prev)] += 1
            if (word, tag, tag_prev) not in word_tag_tally:
                word_tag_tally[(word,tag,tag_prev)] =1
            
            if (tag, tag_prev) in tag_tally:
                tag_tally[tag, tag_prev] += 1
            if (tag, tag_prev) not in tag_tally:
                tag_tally[tag, tag_prev] = 1

    for item in word_tag_tally:
        eigen_vals[item] = math.log(word_tag_tally[item],2) - math.log(tag_tally[item[1:]],2)

    for item in tag_tally:
        tag_list.add(item[0])
        tag_list.add(item[1])

    return eigen_vals, tag_list

# Viterbi algorithm
def viterbi(penn_dev_words, tag_list, known_words_set, tr_values, eigen_vals):
    tagged = []
    print('No. of Penn Dev Words: '+str(len(penn_dev_words)))

    Pi_Init = {}
    for u in tag_list:
        for v in tag_list:
            Pi_Init[(u,v)] = ZERO_LOG_PROB

    for ctr, item in enumerate(penn_dev_words):

        sentence = item + [STOP]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[(START, START)] = [START, START]
        Bigram_Pre = [(START, START)]           

        Pi_Pre = {}
        Pi_Pre[(START, START)] = 0
        
        # For each sentence
        while cur_len < n:
            Path_Cur = {}
            Bigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = STOP
                tagspace = [STOP]
            else:
                word = sentence[cur_len]
                if word not in known_words_set:
                    word = RARE
                tagspace = list(tag_list)
            converted_sentence.append(word)

            for v in tagspace:
                for u in tag_list:
                    emi_tlist = (word, v, u)
                    if emi_tlist not in eigen_vals:
                        eigen_vals[emi_tlist] = ZERO_LOG_PROB
                    w_tlist = ''
                    for w in tag_list:
                        if (w,u) not in Bigram_Pre:
                            continue
                        trigram_cur = (w,u,v)
                        if trigram_cur not in tr_values:
                            tr_values[trigram_cur] = ZERO_LOG_PROB
                        if (u,v) not in Pi_Cur:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+tr_values[trigram_cur]+eigen_vals[emi_tlist]
                            w_tlist = w
                        elif Pi_Pre[(w,u)]+tr_values[trigram_cur]+eigen_vals[emi_tlist] > Pi_Cur[(u,v)]:
                            Pi_Cur[(u,v)] = Pi_Pre[(w,u)]+tr_values[trigram_cur]+eigen_vals[emi_tlist]
                            w_tlist = w
                    if w_tlist != '':
                        Path_Cur[(u,v)] =  Path_Pre[(w_tlist,u)]+[v]
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
            print(str(len(tagged))+' done')

    return tagged

# Write final output to file
def write_output(tagged, filename):
    outfile = open(filename, 'w', encoding='utf-8')
    for sentence in tagged:
        outfile.write(sentence)
    outfile.close()

if __name__ == "__main__":
    with open('midsem_data/penn-data.json', 'r') as infile:
        json_data = json.load(infile)

    # split words and tags, and add start and stop symbols (question 1)
    total_words, total_tags = get_data(json_data)
    
    # calculate transition probabilities
    tr_values = calc_trans_pb(total_tags)

    # get all known words in training data
    known_words_set = get_known_words(total_words)

    # get all known words in training data
    words_penn_rare = swap_rare_words(total_words, known_words_set)

    # calculate emission probabilities
    eigen_vals, tag_list = calc_emi_pb(words_penn_rare, total_tags)

    # Test Data
    t_sentence = 'That former Sri Lanka skipper and ace batsman Aravinda De Silva is a man of few words was very much evident on Wednesday when the legendary batsman , who has always let his bat talk , struggled to answer a barrage of questions at a function to_F promote.'
    t_words = t_sentence.split(' ')
    test_put = [START, START]
    test_put.extend(t_words)
    test_put.append(STOP)
    test_data = [test_put]

    # perform viterbi algorithm
    pos_tags = viterbi(test_data, tag_list, known_words_set, tr_values, eigen_vals)
    write_output(pos_tags, OUTPUT_PATH + 'output_q2b.txt')
    print('Output has been written to output_q2b.txt')