//
// Created by White on 10/7/2020.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstring>

#include "word_dictionary.h"
#include "file_dictionary.h"
#include "tokenizer.h"
#include "file_paths.h"
#include "indexer.h"

using namespace std;
//return key for word to include in index
int word_dictionary::return_key(std::string word) {
    for (auto &it: word_dict) {
        if(it.second == word) {
            return it.first;
        }
    }
}

/*
 * Insert entry to word dict. Assign id as key, and word name as value
 */
void word_dictionary::make_dictionary(string word_name, int doc_num) {
    //change to list of pairs
    word_dict.insert({doc_num, word_name});
}


void word_dictionary::print_dict() {
    ofstream fout;
    input_files paths;
    fout.open(paths.word_dict);

    //print temporarily in function
    for(auto itr = word_dict.begin(); itr != word_dict.end(); ++itr){
        fout << itr->second << " " << itr->first <<  endl;
        //cout << itr->second << " " << itr->first <<  endl;
    }
    fout.close();
}
