//
// Created by White on 11/5/2020.
//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

#include "indexer.h"
#include "file_paths.h"
#include "word_dictionary.h"
#include "file_dictionary.h"

using namespace std;

//update frequency


//add node to doc
void forward_index::addNode(int doc, int w, word_dictionary& wordDictionary, struct forward_list_node new_word) {
    //Check to find word in doc if in, if in, then the freq is updated
    for (int i = 0; i < fwd_idx.at(doc).size(); ++i) {
        if (fwd_idx.at(doc).at(i).word == w) {
            //update frequency
            fwd_idx.at(doc).at(i).word_freq++;
            return;
        }
    }
    //new word if word doesn't exist
    new_word.word = w;
    new_word.word_freq = 1;
    fwd_idx.at(doc).push_back(new_word);

}
//new document entry
void forward_index::addDocument(int doc, std::string word, word_dictionary& wordDictionary) {
    struct forward_list_node new_word{};
    int w = wordDictionary.return_key(word);
    //if the previous entry is the same id as doc, the add node and return
    if(fwd_idx.size()-1 == doc) {
        addNode(doc, w, wordDictionary, new_word);
        return;
    }
    //else new doc
    new_word.word = w;
    new_word.word_freq = 1;

    vector <forward_list_node> temp;
    temp.push_back(new_word);
    fwd_idx.push_back(temp);
}


void forward_index::retrive_dict() {
    ifstream fin;
    input_files paths;
    struct forward_list_node new_word{};
    string line, word, freq, doc = "doc #", docNum, token, check = "\tWord id: ";
    fin.open(paths.fwd_index_file);

    while (!fin.eof()) {
        getline(fin, line);
        //cout << line << endl;
        if (line.find(doc) == string::npos ) {
//            istringstream split(line);
//            //while (!split.eof()) {
//                getline(split, token, '#');
//                getline(split,docNum); //doc number
//            cout << "check" << endl;
            //}
            vector<forward_list_node> temp;
            while(!fin.eof()) {
                getline(fin, line);
                //cout << line << endl;
                //vector<forward_list_node> temp;
                if (line.find(check) != string::npos) {
                    istringstream split(line);
                    getline(split, token, ':');

                    if (token.front() == ' ') {
                        token.erase(token.begin());
                    }
                    getline(split, token, ':');
                    getline(split, word);
                    getline(split, token, ' ');

                    if (token.front() == ' ') {
                        token.erase(token.begin());
                    }

                    freq = token;

                    new_word.word = stoi(word);
                    new_word.word_freq = stoi(freq);
                    temp.push_back(new_word);

                }
                if(line == "--------------") {
                    fwd_idx.push_back(temp);
                    temp.clear();
                }
            }
        }

    }
//    for (int i = 0; i < fwd_idx.size(); ++i) {
//        cout << "Doc #" << i << endl;
//        for (int j = 0; j < fwd_idx[i].size(); ++j) {
//            cout << "\tWord id: " << fwd_idx[i][j].word << " Frequency: " << fwd_idx[i][j].word_freq << endl;
//        }
//        //fout << "--------------" << endl; //for parsing later
//    }
}

void forward_index::print() {
    //Print
    input_files path;
    ofstream fout;
    fout.open(path.fwd_index_file);
    for (int i = 0; i < fwd_idx.size(); ++i) {
        fout << "Doc #" << i << endl;
        for (int j = 0; j < fwd_idx[i].size(); ++j) {
            fout << "\tWord id: " << fwd_idx[i][j].word << " Frequency: " << fwd_idx[i][j].word_freq << endl;
        }
        fout << "--------------" << endl; //for parsing later
    }
}

void inverted_index::addword(word_dictionary wordDictionary,  forward_index forwardIndex) {
    string current_word;
    invert_node invertNode{};
    int current_id;
    for (int i = 0; i < wordDictionary.word_dict.size(); ++i) {
        current_word = wordDictionary.word_dict[i];
        current_id = wordDictionary.return_key(current_word);
        //call forward index
        // go through docs
        vector<invert_node> temp;
        for (int j = 0; j < forwardIndex.fwd_idx.size(); j++) {
            // words in doc
            //words if word appears in doc break and return previous loop
            //Mark in entry of word, go to next doc
            for (int k = 0; k < forwardIndex.fwd_idx[j].size(); ++k) {
                if(current_id == forwardIndex.fwd_idx[j][k].word) {
                    invertNode.doc = j;
                    invertNode.freq = forwardIndex.fwd_idx[j][k].word_freq;
                    temp.push_back(invertNode);
                    break;
                }
            }
        }
        ivs_idx.emplace_back(i, temp);
        temp.clear();
    }
}

void inverted_index::retrive_dict() {

    input_files path;
    invert_node node{};
    string word, doc, freq, line, token, wordCheck = "Word", docCheck = "\tDoc", freqCheck = " Freq:";
    ifstream fin;
    fin.open((path.ivs_index_file));
    vector<invert_node> temp;
    while (!fin.eof()) {
        getline(fin, line);
        if(line.find(wordCheck) != string::npos) {
            istringstream split(line);
            getline(split, token, ' ');
            getline(split, word);
            //cout << word << endl;
        }
        if(line.find(docCheck) != string::npos) {
            istringstream split(line);
            getline(split, token, ' ');
            getline(split, doc, ' ');
            getline(split, token, ' ');
            getline(split, freq);
            //cout << doc << " " << freq << endl;
            node.doc = stoi(doc);
            node.freq = stoi(freq);
            temp.push_back(node);
        }
        if(line == "--------------") {
            ivs_idx.push_back(make_pair(stoi(word), temp));
            temp.clear();
        }


    }
//    for (int i = 0; i < ivs_idx.size(); i++) {
//        cout << "Word " << ivs_idx[i].first << endl;
//        for (int j = 0; j < ivs_idx[i].second.size(); ++j) {
//            cout << "\tDoc " << ivs_idx[i].second[j].doc << " Freq: " << ivs_idx[i].second[j].freq << endl;
//        }
//        //fout << "--------------" << endl; //for parsing later
//    }

}

//grab frequency of inverted index

void inverted_index::print_ivs() {
    input_files path;
    ofstream fout;
    fout.open(path.ivs_index_file);

    for (int i = 0; i < ivs_idx.size(); i++) {
        fout << "Word " << ivs_idx[i].first << endl;
        for (int j = 0; j < ivs_idx[i].second.size(); ++j) {
            fout << "\tDoc " << ivs_idx[i].second[j].doc << " Freq: " << ivs_idx[i].second[j].freq << endl;
        }
        fout << "--------------" << endl; //for parsing later
    }
}

//Inverted Index


