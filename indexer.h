//
// Created by White on 11/2/2020.
//

#ifndef CSCE4200_PROJECT_2_INDEXER_H
#define CSCE4200_PROJECT_2_INDEXER_H

#include <string>
#include <list>
#include <vector>
#include <utility>

#include "word_dictionary.h"

struct forward_list_node {
    int word;
    int word_freq;
};

struct invert_node {
    int doc;
    int freq;
};
//forward index
class forward_index{

public:
    //add node
    void addNode(int doc, int word, word_dictionary& wordDictionary, forward_list_node new_word);
    //new document
    void addDocument(int doc, std::string word, word_dictionary& wordDictionary);

    void retrive_dict();
    //print
    void print();

//vector of documents
//list doc: word, frequecy -> ...
    std::vector<std::vector<forward_list_node>> fwd_idx;
};
//backwards index


class inverted_index : forward_index{
public:

    //make inverted index
    void addword(word_dictionary wordDictionary, forward_index forwardIndex);

    void retrive_dict();

    //int word num, vector of structs
    void print_ivs();

    std::vector<std::pair<int, std::vector<invert_node>>> ivs_idx;
};

#endif //CSCE4200_PROJECT_2_INDEXER_H
