//
// Created by White on 11/27/2020.
//

#ifndef CSCE4200_PROJECT_3_QUERY_PROCESSOR_H
#define CSCE4200_PROJECT_3_QUERY_PROCESSOR_H

#include <vector>
#include <string>
#include "file_dictionary.h"
#include "indexer.h"
#include "word_dictionary.h"
//vector space

struct qrel{
    int topic_num;
    int iteration;
    int docnum;
    bool relevant;
};

//std::vector<qrel> q; //contains info

struct processed_queries {
    int topic_num;
    std::string document;
    int unique;
    double cosine;
};

void print_queries();

//cosine similarity
void cosine_similarities(vector<string> terms, int topic, ofstream& fout, int N, inverted_index invertedIndex, forward_index forwardIndex, file_dictionary fileDictionary, word_dictionary word_dictionary);


void topics_file_reader();


void query_processor(file_dictionary fileDictionary, word_dictionary wordDictionary, forward_index forwardIndex, inverted_index invertedIndex);


#endif //CSCE4200_PROJECT_3_QUERY_PROCESSOR_H
