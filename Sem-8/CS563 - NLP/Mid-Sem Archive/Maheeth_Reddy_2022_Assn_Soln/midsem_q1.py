import os
import math
from sklearn.metrics import accuracy_score, precision_recall_fscore_support

START = '*'
STOP = 'STOP'
RARE = '_RARE_'
RARE_MAX_FREQ = 5
ZERO_LOG_PROB = -1000

OUTPUT_PATH = 'output/'
if not os.path.exists(OUTPUT_PATH):
    os.mkdir(OUTPUT_PATH)

# Generate a list of words and a list of tags from a list of tagged sentences
def get_data(data):
    words_penn, tags_penn = [], []
    twords, ttags = [START], [START]
    for wtag in data:
        clean_wtag = wtag.strip('\n')
        if clean_wtag == '_,_':
            twords.append(STOP)
            ttags.append(STOP)
            words_penn.append(twords)
            tags_penn.append(ttags)
            twords, ttags = [START], [START]
        if clean_wtag != '_,_':
            split_data = clean_wtag.split(',')
            word, tag = split_data[0], split_data[1]
            twords.append(word)
            ttags.append(tag)
    return words_penn, tags_penn

# Calculate Bigrams
def bigrams(item):
    return [(item[i], item[i+1]) for i in range(len(item)-1)]

# Calculate Transition Probabilities
def calc_trans_pb(tags_penn):
    tr_values = {}
    bi_tally = {}
    uni_tally = {}

    for item in tags_penn:
        t_bi = bigrams(item)
        for bigram in t_bi:
            if bigram in bi_tally:
                bi_tally[bigram] += 1
            if bigram not in bi_tally:
                bi_tally[bigram] = 1

        for unigram in item:
            if unigram in uni_tally:
                uni_tally[unigram] += 1
            if unigram not in uni_tally:
                uni_tally[unigram] = 1

    for bigram in bi_tally:
        tr_values[bigram] = math.log(bi_tally[bigram], 2) - math.log(uni_tally[bigram[0]],2)
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
        if word_tally[item] > RARE_MAX_FREQ:
            known_words_set.add(item)
	    
    return known_words_set

# Replace unknown words by '_RARE_'
def swap_rare_words(words_penn, known_words):
    words_penn_rare = []
    for item in words_penn:
        tlist = []
        for word in item:
            if word in known_words:
                tlist.append(word)
            if word not in known_words:
                tlist.append(RARE)
        words_penn_rare.append(tlist)

    return words_penn_rare

# Calculate emission probabilities and a set of all possible tags
def calc_emi_pb(words_penn_rare, tags_penn):
    eigen_vals = {}
    tag_list = set([])

    tag_tally = {}
    wtag_tally = {}
    for i in range(len(tags_penn)):
        sentence = words_penn_rare[i]
        tags = tags_penn[i]
        for j in range(len(tags)):
            word = sentence[j]
            tag = tags[j]
            if (word, tag) in wtag_tally:
                wtag_tally[(word,tag)] += 1
            if (word, tag) not in wtag_tally:
                wtag_tally[(word,tag)] =1
            
            if tag in tag_tally:
                tag_tally[tag] += 1
            if tag not in tag_tally:
                tag_tally[tag] = 1

    for item in wtag_tally:
        eigen_vals[item] = math.log(wtag_tally[item],2) - math.log(tag_tally[item[1]],2)

    for item in tag_tally:
        tag_list.add(item)

    return eigen_vals, tag_list

# Viterbi algorithm
def viterbi(penn_dev_words, tag_list, known_words, tr_values, eigen_vals):
    tagged = []
    print('No. of Penn Dev Words: '+str(len(penn_dev_words)))
    for item in penn_dev_words:
        sentence = item + [STOP]
        converted_sentence = []
        n = len(sentence)

        cur_len = 0
        Path_Pre = {} 
        Path_Pre[START] = [START]
        Unigram_Pre = [START]           

        Pi_Pre = {}
        Pi_Pre[START] = 0
        
        while cur_len < n:
            Path_Cur = {}
            Unigram_Cur = []
            Pi_Cur = {}

            if cur_len == n-1:
                word = STOP
                tagspace = [STOP]
            else:
                word = sentence[cur_len]
                if word not in known_words:
                    word = RARE
                tagspace = list(tag_list)
            converted_sentence.append(word)

            for v in tagspace:
                emi_tmp = (word, v)
                if emi_tmp not in eigen_vals:
                    continue
                w_tmp = ''
                for w in tag_list:
                    if w not in Unigram_Pre:
                        continue
                    bigram_cur = (w, v)
                    if bigram_cur not in tr_values:
                        tr_values[bigram_cur] = ZERO_LOG_PROB
                    if v not in Pi_Cur:
                        Pi_Cur[v] = Pi_Pre[w]+tr_values[bigram_cur]+eigen_vals[emi_tmp]
                        w_tmp = w
                    elif Pi_Pre[w]+tr_values[bigram_cur]+eigen_vals[emi_tmp] > Pi_Cur[v]:
                        Pi_Cur[v] = Pi_Pre[w]+tr_values[bigram_cur]+eigen_vals[emi_tmp]
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

def dev_tags_split(dev_sentences):
    words_penn = []
    tags_penn = []

    for item in dev_sentences:
        t_words = []
        t_tags = []
        sentence = item.strip().split(' ')
        for token in sentence:
            t_words.append(token.rsplit('/',1)[0])
            t_tags.append(token.rsplit('/',1)[1])
        
        words_penn.append(t_words)
        tags_penn.append(t_tags)

    return words_penn, tags_penn

def disp_accuracy(test_tags_penn, test_tags):
    # Flattening all original tags
    orig_tags = []
    for orig_tag in test_tags_penn:
        orig_tags.extend(orig_tag[1:-1])

    # Flattening all predicted tags
    pred_tags = []
    for predict_tag in test_tags:
        pred_tags.extend(predict_tag[1:-1])

    assert len(orig_tags) == len(pred_tags)
    print('Accuracy:', accuracy_score(orig_tags, pred_tags)*100)
    prf = precision_recall_fscore_support(orig_tags, pred_tags, average='macro')
    print('Precison:', prf[0]*100)
    print('Recall:', prf[1]*100)
    print('F score:', prf[2]*100)

# perform the procedure on given test file
def driver_function(test_file):
    filename = test_file.split('/')[-1]
    print('Running on '+filename)
    print("===============")

    with open('midsem_data/train_data.txt', 'r') as infile:
        train_words, train_tags = get_data(infile.readlines())
    with open(test_file, 'r') as infile:
        test_words, test_tags = get_data(infile.readlines())

    # calculate transition probabilities
    tr_values = calc_trans_pb(train_tags)

    # get all known words in training data
    known_words_list = get_known_words(train_words)

    # replace rare words with '_RARE_'
    words_penn_rare = swap_rare_words(train_words, known_words_list)

    # calculate emission probabilities
    eigen_vals, tag_list = calc_emi_pb(words_penn_rare, train_tags)

    # perform viterbi algorithm
    viterbi_tagged = viterbi(test_words, tag_list, known_words_list, tr_values, eigen_vals)

    # write output to file
    write_output(viterbi_tagged, "output/output_"+filename.split('.')[0]+".txt")

    # Show accuracy
    test_words, test_predicted_tags = dev_tags_split(viterbi_tagged)
    disp_accuracy(test_tags, test_predicted_tags)

# run the driver function for test_data.txt and p1.txt
if __name__ == "__main__":
    driver_function(test_file='midsem_data/test_data.txt')
    print()
    driver_function(test_file='midsem_data/p1.txt')