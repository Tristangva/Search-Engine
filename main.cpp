/*
 * main function of the program
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>

#include "file_paths.h"
#include "tokenizer.h"
#include "file_dictionary.h"
#include "word_dictionary.h"
#include "indexer.h"


using namespace std;
namespace fs = experimental::filesystem;

struct results{
    int doc;
    int freq;
};

int main() {
    //Enter path containing directory with to be parsed files
    //string path = "C:/Users/White/Documents/CSCE4200_Project_2/ft911/"; //path to directory
    input_files paths; //file path structs
    ifstream docIn, stops;
    string input, line;
    char option;
    vector<string> files;
    vector<string> stop_words;
    file_dictionary file_dictionary1;
    word_dictionary word_dictionary1;
    forward_index forwardIndex;
    inverted_index invertedIndex;

    results returned_result;
    //std::vector<std::pair<auto, std::vector<results>>> search_results;
    stops.open(paths.stop_word_list);
    //parses through directory putting files+paths into files vector
    for (const auto &entry : fs::directory_iterator(paths.ft911_directory)) {
        files.push_back(entry.path());
    }
    cout << "Directory files colllected\nTokenizing..." << endl;
    //Tokenizer is called
    tokenizer(files, docIn, word_dictionary1, file_dictionary1, forwardIndex, invertedIndex);
    //make stopword
    while (stops >> line) {
        stop_words.push_back(line);
    }
    //bare search function
    while (input != "exit!") {
        cout << "Enter in word to return. type \"exit!\" to return" << endl;
        getline(cin, input); //user input
        bool check = false;
        if (input == "exit!") { continue;}
        //checks if stopword
        for (int i = 0; i < stop_words.size(); ++i) {
            if(stop_words[i] == input) {
                check = true;
                break;
            }
        }
        if(check) {continue;} //restart if stop word

        for(auto itr = word_dictionary1.word_dict.begin(); itr != word_dictionary1.word_dict.end(); ++itr){
            //checks to see if substring
            bool substring = true;
            //if substring and length greater than 1, then print out contents of inverted index entries
            if((itr->second == input.substr(0, itr->second.size())) && itr->second.length() > 1) {
                cout << itr->second << endl;
                for(int j = 0; j < invertedIndex.ivs_idx[itr->first].second.size(); j++) {
                    cout << "\tDocument: " << invertedIndex.ivs_idx[itr->first].second[j].doc
                         << " freq: " << invertedIndex.ivs_idx[itr->first].second[j].freq << endl;
                }
            }
        }
    }
    return 0;
}