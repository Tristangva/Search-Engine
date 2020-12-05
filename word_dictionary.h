//
// Created by White on 10/7/2020.
//

#ifndef CSCE4200_PROJECT_1_WORD_DICTIONARY_H
#define CSCE4200_PROJECT_1_WORD_DICTIONARY_H

#include <fstream>
#include <string>
#include <vector>
#include <map>

class word_dictionary {

public:


    int return_key(std::string word);

    //make dictionary
    void make_dictionary(std::string word, int doc_num);

    void retrieve_dict();

    //print
    void print_dict();

//Word map made for formatting
//make vector of pairs?
std::map <int, std::string> word_dict;
};


#endif //CSCE4200_PROJECT_1_WORD_DICTIONARY_H
