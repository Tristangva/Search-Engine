//
// Created by White on 11/27/2020.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <list>
#include <algorithm>

#include "file_paths.h"
#include "tokenizer.h"
#include "query_processor.h"
#include "indexer.h"
#include "word_dictionary.h"
#include "file_dictionary.h"

using namespace std;


//cosine similarity from book
void cosine_similarities(vector<string> terms,  int N, inverted_index invertedIndex, forward_index forwardIndex, file_dictionary fileDictionary, word_dictionary word_dictionary) {

    vector<int> str_lengths;
    string term;
    pair<float, int> scores[N] = {make_pair(scores->first = 0.0, scores->second = 0)};
    float length[N] = {0}; //length init

    int K = 10, df, tf = 0, curDoc;
    float wftd, idf;
    //go through each list
    for (int t = 0; t < terms.size(); t++) {
        term = terms[t];
        //fetch inverse index posting list
        //word lengths
        for (auto itr = word_dictionary.word_dict.begin(); itr != word_dictionary.word_dict.end(); ++itr) {
            //cout << itr->second << endl;
            if (itr->second == term.substr(0, itr->second.length()) && itr->second.length() > 1) {
                //cout << itr->second << " " << itr->second.length()<< endl;
                str_lengths.push_back(itr->second.length());
            }
        }

        for (auto itr = word_dictionary.word_dict.begin(); itr != word_dictionary.word_dict.end(); ++itr) {
            //(itr->second == term.substr(0, itr->second.length()) )&&(itr->second.length() == closest_length)
            if (terms[t] == itr->second) {
                //auto itr2 = itr;
                df = static_cast<int>(invertedIndex.ivs_idx[itr->first].second.size());
                idf = static_cast<float>(log10(N / df)); //idf for term
                cout << itr->second << endl;
                for (int d = 0; d < invertedIndex.ivs_idx[itr->first].second.size(); d++) {
//                    cout << "\tDocument: " << invertedIndex.ivs_idx[itr->first].second[d].doc
//                         << " freq: " << invertedIndex.ivs_idx[itr->first].second[d].freq << endl;
                    curDoc = invertedIndex.ivs_idx[itr->first].second[d].doc;
                    if (invertedIndex.ivs_idx[itr->first].second[d].freq > 0) {
                        wftd = static_cast<float>(1 + log(invertedIndex.ivs_idx[itr->first].second[d].freq));
                    } else {
                        wftd = 0;
                    }
                    scores[curDoc].first+= (wftd* idf);
                    for (int i = 0; i < invertedIndex.ivs_idx[itr->first].second.size(); i++) {
                        tf += invertedIndex.ivs_idx[itr->first].second[i].freq;
                    }
                    //array for length
                    length[curDoc] = tf * idf; //find length w/ normalization
                    scores[curDoc].first = scores[curDoc].first / length[curDoc]; //score
                    scores[curDoc].second = invertedIndex.ivs_idx[itr->first].second[d].doc; //doc id

                }
                break; //break as needed data found
            }
        }
    }
    //return top scores
    sort(scores, scores+N);
    for (int j = N-1; j >= N-K; j--) {
        cout  << scores[j].first << " " << scores[j].second << " " << fileDictionary.file_dict.at(scores[j].second) << endl;
    }
}


std::vector<pair<int, std::vector<std::string>>> query; //it made me post it here


void topics_file_reader(){
    input_files files, paths;
    int numb;
    string line, num = "<num> Number:", title = "<title>", token;
    vector<string> stop_words;
    bool check;
    ifstream fin, stops;
    fin.open(files.topics_list);
    //cout << "check" << endl;
    //parse file
    stops.open(paths.stop_word_list);
    while (stops >> line) {
        stop_words.push_back(line);
    }
    while(!fin.eof()) {
        vector<string> temp;
        getline(fin, line);
        if (line.find(num) != string::npos || line.find(title) != string::npos) {
            istringstream find(line);
            //if number
            if(line.find(num) != string::npos) {
                size_t pos = string::npos;
                // Search for the substring in string in a loop untill nothing is found
                while ((pos  = line.find(num) )!= std::string::npos) {
                    // If found then erase it from string
                    line.erase(pos, num.length());
                }

                numb = stoi(line);
            }

            //if title
            if(line.find(title) != string::npos) {

                if (token.front() == ' ') {
                    token.erase(token.begin());
                }

                while(!find.eof()) {
                    getline(find, token, ' ');
                    if((token != title)&& !token.empty()) {
                        token = toLowerCase(token);
                        check = false;
                        for (int i = 0; i < stop_words.size(); i++) {
                            if(token == stop_words[i]) {
                                check = true;
                                break;
                            }
                        }
                        if (!check) {
                            //cout << token << endl;
                            token = stem_string(token);
                            //cout << token << endl;
                            temp.push_back(token);
                        }
                    }
                }
                //cout << temp.size() << endl;
                query.emplace_back(numb, temp);
               temp.clear();
            }

        }

    }
}


void query_processor(file_dictionary fileDictionary, word_dictionary word_dictionary, forward_index forwardIndex, inverted_index invertedIndex, bool manual) {

    //get query from topics.txt
    vector<pair<int, vector<float>>> scores;
    vector < string> stop_words;
    input_files paths;
    string line;
    ifstream stops;
    stops.open(paths.stop_word_list);
    //make stopword
    while (stops >> line) {
        stop_words.push_back(line);
    }
    if (!manual) {
        //enter in manually with function
    } else {
       topics_file_reader();
        for (int i = 0; i < query.size(); ++i) {
            cout << "Topic: " << query.at(i).first << endl << endl;
//            for (int j = 0; j < query.at(i).second.size(); ++j) {
//                cout << "\t" << query.at(i).second[j] << endl;
//            }
            cosine_similarities(query.at(i).second, forwardIndex.fwd_idx.size(), invertedIndex, forwardIndex, fileDictionary, word_dictionary);
        }

    }
}


void index_retrieval(vector<string> words, file_dictionary fileDictionary, word_dictionary word_dictionary,inverted_index invertedIndex, forward_index forwardIndex) {
    string word;
    std::vector<std::pair<int, std::vector<invert_node>>> query_indexes;
    float scores[10];
    int N = forwardIndex.fwd_idx.size(); //total amount of documents
    int df;
    double idf;

    for (int i = 0; i < words.size(); ++i) {
        word = toLowerCase(words.at(i));
        for (auto itr = word_dictionary.word_dict.begin(); itr != word_dictionary.word_dict.end(); ++itr) {
            //checks to see if substring
            bool substring = true;
            //if substring and length greater than 1, put in vector to test sizes
            //find closest in length
            if ((itr->second == word.substr(0, itr->second.size())) && itr->second.length() > 1) {
                cout << itr->second << endl;
                df = invertedIndex.ivs_idx[itr->first].second.size();

                //send invertedIndex.ivs_idx[itr->first].second to cosine function?

                for (int j = 0; j < invertedIndex.ivs_idx[itr->first].second.size(); j++) {
                    cout << "\tDocument: " << invertedIndex.ivs_idx[itr->first].second[j].doc
                         << " freq: " << invertedIndex.ivs_idx[itr->first].second[j].freq << endl;
                }
                idf = log10(N/df); //idf for term
            }
        }
    }
}