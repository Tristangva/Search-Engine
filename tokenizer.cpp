//
// Created by Tristan White on 10/1/2020.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <chrono>

#include "tokenizer.h"
#include "word_dictionary.h"
#include "file_dictionary.h"
#include "file_paths.h"
#include "indexer.h"
#include "stemmer.h"

using namespace std;
using namespace std::chrono;

//Checks if string is number
//If string contains a non digit char, than returns false
bool isNumber(string str) {
    for (char i : str) {
        if(!isdigit(i)) {
            return false;
        }
    }
    return true;
}

// borrows from: https://stackoverflow.com/questions/23313498/c-convert-word-in-type-string-variable-to-lowercase
string toLowerCase(string word) {

    transform(word.begin(), word.end(), word.begin(), ::tolower);
    return (word);
}

// removes html tags from line
// uses https://thispointer.com/how-to-remove-substrings-from-a-string-in-c/
string DocName(string line) {

    string DocNameStr1 = "<DOCNO>", DocNameStr2 = "</DOCNO>";
    string docname;

    size_t pos = string::npos;
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = line.find(DocNameStr1) )!= std::string::npos)
    {
        // If found then erase it from string
        line.erase(pos, DocNameStr1.length());
    }

    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = line.find(DocNameStr2) )!= std::string::npos)
    {
        // If found then erase it from string
        line.erase(pos, DocNameStr2.length());
    }

    return line;
}

string stem_string(string word_name) {
    char *temp;
    int stemmed_size;


    strcpy(temp, word_name.c_str()); //temp is cstring

    stemmed_size = stem(temp, 0, (strlen(temp) - 1)); //stemmer called

    word_name.resize(static_cast<unsigned long>(stemmed_size)); //resize to make stem*/
    return word_name;
}

bool eleminate_stop_word(string word, vector<string> word_list) {
    for (int i = 0; i < word_list.size(); ++i) {
        if(word_list.at(i) == word) {
            return true;
        }
    }
    return false;
}

// NOTE TO SELF, MODIFY LATER
/*
 * Tokenizer function
 * Brings in vector files, docIn ifstream, and class instance
 */
void tokenizer(vector <string> files, ifstream& docIn, word_dictionary &wordDictionary, file_dictionary &fileDictionary, forward_index &forwardIndex, inverted_index &invertedIndex) {

    string line, token, delim, temp, word1, word2; // line from file, tokens, deminitor
    string document_name; //contains doc name
    string DocNameStr1 = "<DOCNO>", DocNameStr2 = "</DOCNO>"; //used to find doc name
    string temp_doc;

    vector <string> collected_tokens; //contains tokens vector
    vector <string> documents; //contains document titles
    ifstream stop_word_in; //used to read in stopwords
    vector <string> stop_words; //contains stopwords
    vector <vector<string>> words_per_doc; //2d vector to include words in document
    vector <string> tempr;
    int  doccount = 0;
    bool firstdoc = true;
    input_files paths;

    //reads in from file stopwordlist.txt
    stop_word_in.open(paths.stop_word_list);
    while (stop_word_in >> line) {
        stop_words.push_back(line);
    }

    for (int j = 0; j < files.size(); ++j) { //For loop
        docIn.open(files.at(j));
        while (!docIn.eof()) {
            getline(docIn, line);
            istringstream split(line);
            //find document name
            if (line.find(DocNameStr1) != string::npos && line.find(DocNameStr2) != string::npos) {
                document_name = DocName(line);
                if (firstdoc) {
                    temp_doc = document_name;
                    firstdoc = false;
                }

                fileDictionary.make_file_dict(document_name, doccount); //makefile
                if (temp_doc != document_name) {
                    words_per_doc.push_back(tempr);
                    //cout << words_per_doc.size() << endl;
                    tempr.clear();
                }
                temp_doc = document_name;
                doccount++;
            }
            while (!split.eof()) {
                getline(split, token, ' ');
                //remove whitespace
                if (token.front() == ' ') {
                    token.erase(token.begin());
                }

                if (token.front() != '<') {

                    //remove hyphens between words if - found
                    // else will go straight to lowercase
                    if (token.find('-') != std::string::npos) {
                        istringstream t(token); //split line with '-'
                        while (!t.eof()) {
                            getline(t, word1, '-');
                            //erase non alpha numeric characters
                            //uses https://stackoverflow.com/questions/6319872/how-to-strip-all-non-alphanumeric-characters-from-a-string-in-c#comment25384474_6319898
                            word1.erase(std::remove_if(word1.begin(), word1.end(),
                                                       std::not1(std::ptr_fun((int (*)(int)) std::isalnum))), word1.end());
                            word1 = toLowerCase(word1); //makes lowercase
                            if (!isNumber(word1)) {
                                if(!eleminate_stop_word(word1, stop_words)) {
                                    collected_tokens.push_back(word1);
                                    tempr.push_back(word1);
                                }
                            }
                        }
                    } else {
                        token.erase(std::remove_if(token.begin(), token.end(),
                                                   std::not1(std::ptr_fun((int (*)(int)) std::isalnum))), token.end());
                        token = toLowerCase(token); //makes lowercase
                        if (!isNumber(token)) {
                            if(!eleminate_stop_word(token, stop_words)) {
                                collected_tokens.push_back(token);
                                tempr.push_back(token);
                            }
                        }
                    }
                }
            }
        }
        docIn.close();
    }
    if(doccount<=1) {
        words_per_doc.push_back(tempr);
        tempr.clear();
    }
    fileDictionary.print_dict();
    //eliminate duplicates from collected_tokens
    // https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
    sort(collected_tokens.begin(), collected_tokens.end());
    collected_tokens.erase(unique(collected_tokens.begin(), collected_tokens.end()), collected_tokens.end());
    /*
     * Tokenize list of words to get stemmed form and id
     * Uses bool fo find out if word is already stemmed to eliminate duplicate stemmed words
     * If check is not true, then the newly stemmed word is put in dictionary
     */
    int j = 0;
    bool check;

    //make the word list and stem word
    for (int i = 0; i < collected_tokens.size(); i++) {
        check = false;
        word1 = collected_tokens[i];
        word2 = stem_string(word1); // stem word here
        //check to see if stemmed word already exists in word dictionary
        for (auto itr = wordDictionary.word_dict.begin(); itr != wordDictionary.word_dict.end(); ++itr) {
            if(itr->second == word2) {
                check = true;
                break;
            }
        }
        //if not in dictionary, add to dictionary
        if(!check) {
            wordDictionary.make_dictionary(word2, j);
            j++;
        }
    }
    cout << "Word dict size: " << wordDictionary.word_dict.size() << endl;
    wordDictionary.print_dict();


    cout << "Indexing " << words_per_doc.size() << " documents" << endl;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < words_per_doc.size(); i++) {
        sort(words_per_doc.at(i).begin(), words_per_doc.at(i).end());
        //stem for index
        for (int j = 0; j < words_per_doc.at(i).size(); j++) {
            word1 = stem_string(words_per_doc[i][j]);
            forwardIndex.addDocument(i, word1, wordDictionary);
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Forward index size: "<< forwardIndex.fwd_idx.size() << endl;
    //get time to compute forward index
    cout << "Forward Index time: " << duration.count() << endl;
    forwardIndex.print();
    cout << "Inverted Index\n";
    start = high_resolution_clock::now();
    //make inverted index
    invertedIndex.addword(wordDictionary, forwardIndex);
    stop = high_resolution_clock::now();
    duration = duration_cast<seconds>(stop - start);
    cout << "Inverted index time: " << duration.count() << endl;
    cout << "Inverted index size: " << invertedIndex.ivs_idx.size() << endl;
    cout << "Inverted Index made\n";
    invertedIndex.print_ivs();

}