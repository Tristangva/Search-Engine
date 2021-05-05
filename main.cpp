/*
 * main function of the program
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <ctime>
#include <chrono>

#include "file_paths.h"
#include "tokenizer.h"
#include "file_dictionary.h"
#include "word_dictionary.h"
#include "indexer.h"
#include "query_processor.h"


using namespace std;
namespace fs = experimental::filesystem;
using namespace std::chrono;


int main(int argc, char *argv[]) {
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

    cout << "Welcome to search engine!\nPress 1 for search\nPress 2 to tokenize and index docs\n";
    cin >> option;

    if (option == '2') {
        //parses through directory putting files+paths into files vector
        auto start = high_resolution_clock::now();
        for (const auto &entry : fs::directory_iterator(paths.ft911_directory)) {
            files.push_back(entry.path());
        }
        cout << "Directory files colllected\nTokenizing..." << endl;
        //Tokenizer is called to make dict
        tokenizer(files, docIn, word_dictionary1, file_dictionary1, forwardIndex, invertedIndex);

        //indexer tokenizer
        //one of the worst programming designs I've ever done. I'm sorry to those who see this
        cout << "Tokenizing again for index" << endl;
        tokenizer(files, docIn, word_dictionary1, file_dictionary1, forwardIndex, invertedIndex, true);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        cout << "Data processing time: " << duration.count() << endl;
        cout << "Run again but press 1 to get the search" << endl;
    }
    if(option == '1') {

        //read in the files containing indexes and dictionaries
        file_dictionary1.retrieve_dict();
        word_dictionary1.retrieve_dict();
        forwardIndex.retrive_dict();
        invertedIndex.retrive_dict();

        //begin query
        cout << "Processing queries\n";
        query_processor(file_dictionary1, word_dictionary1, forwardIndex, invertedIndex);
    }


    return 0;
}