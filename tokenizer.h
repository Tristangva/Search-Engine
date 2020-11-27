//
// Created by White on 10/1/2020.
//
/*
 * To see the functions implemented, go to tokenizer.cpp
 */

#ifndef CSCE4200_PROJECT_1_TOKENIZER_H
#define CSCE4200_PROJECT_1_TOKENIZER_H

#include <string>
#include <fstream>
#include <vector>
#include "file_dictionary.h"
#include "word_dictionary.h"
#include "indexer.h"

using namespace std;

// remove numbers
bool isNumber(string str);

//go to lower case
string toLowerCase(string word);

//Gets Doc Name
string DocName(string Word);

string stem_string(string word_name);

bool eleminate_stop_word(string word, vector<string> word_list);

//Toekenizer function
void tokenizer(vector <string> files, ifstream& fin, word_dictionary& wordDictionary,
               file_dictionary& fileDictionary, forward_index& forwardIndex, inverted_index& invertedIndex, bool index = false);



#endif //CSCE4200_PROJECT_1_TOKENIZER_H