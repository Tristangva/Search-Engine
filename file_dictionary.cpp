//
// Created by White on 10/12/2020.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstring>

#include "file_dictionary.h"
#include "tokenizer.h"
#include "file_paths.h"

using namespace std;

//adds entry to file dictionary
void file_dictionary::make_file_dict(std::string document, int docNum) {
    file_dict.insert({docNum, document });
}

int file_dictionary::return_key(std::string document) {

    for(auto it = file_dict.begin(); it != file_dict.end(); ++it){
        if(it->second == document) {
            return it->first;
        }
    }

}

void file_dictionary::retrieve_dict() {
    ifstream fin;
    input_files paths;
    string line, name, num;
    fin.open(paths.file_dict);

    while (!fin.eof()) {
        getline(fin, name, ' ');
        getline(fin, num);
        file_dict.insert({stoi(num), name});
    }
}

void file_dictionary::print_dict() {

    // Enter in parser_output.txt path
    ofstream fout;
    input_files paths;
    string path = "C:/Users/White/Documents/CSCE4200_Project_2/file_dict.txt";
    fout.open(paths.file_dict);
    //print temporarily in function
    for(auto itr = file_dict.begin(); itr != file_dict.end(); ++itr){
        //cout << itr->second << " " << itr->first << endl;
        fout << itr->second << " " << itr->first << endl;
    }

    fout.close();

}
