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

void print_queries() {

}

//cosine similarity algotithm from book
//https://nlp.stanford.edu/IR-book/pdf/06vect.pdf
void cosine_similarities(vector<string> terms, int topic, ofstream& fout,  int N, inverted_index invertedIndex, forward_index forwardIndex, file_dictionary fileDictionary, word_dictionary word_dictionary) {

    vector<int> str_lengths;
    string term;
    pair<float, int> scores[N] = {make_pair(scores->first = 0.0, scores->second = 0)};
    float length[N] = {0}; //length init
    int K = 10, df = 0, curDoc, temp;
    float wftd, wfqd, idf, tfq, tfd;
    //go through each list

    //idf-tf
    //tf term t in document d
    //tf = count of t / words in d
    for (int t = 0; t < terms.size(); ++t) {
        term = terms[t]; //terms for easier coding
        auto postings_list = word_dictionary.word_dict.begin(); //idk it wanted me to initalize like this
        //find word frequency by finding amount of inverse index entries for term
        for (auto itr = word_dictionary.word_dict.begin(); itr != word_dictionary.word_dict.end(); ++itr) {
            if (term == itr->second) {
                postings_list = itr;
                df = static_cast<int>(invertedIndex.ivs_idx[itr->first].second.size()); // temrs / words in doc doesn't include freq
                break; //exit loop to save time
            }
        }
        //idf found
        idf = static_cast<float>(log10(N / df));

        //tf found
        temp = 0;
        for(int i = 0; i < terms.size(); i++){
            if(term == terms[i]) {
                temp++;
            }
        }

        tfq = static_cast<float>(temp) / (terms.size()); //amount of times term in query / query size
        wfqd = tfq*idf; //for query
        for (int d = 0; d < invertedIndex.ivs_idx[postings_list->first].second.size(); d++) {
            temp = 0;
            curDoc = invertedIndex.ivs_idx[postings_list->first].second[d].doc; //current doc id for array
            //if term save freq in temp and add to doc size
            //Current doc id used for forward index
            for (int i = 0; i < forwardIndex.fwd_idx[curDoc].size(); i++) {
                temp += forwardIndex.fwd_idx[curDoc][i].word_freq;
            }
            //cout << temp << endl;
            //find tf for doc
            tfd = static_cast<float>(1 + log10(temp));
                    //static_cast<float > (invertedIndex.ivs_idx[postings_list->first].second[d].freq) / static_cast<float > (temp);
            //tf-idf for doc
            wftd = tfd*idf;

            //wftd X wfqd to count array at curDoc
            scores[curDoc].first+= (wftd * wfqd);

            //array for length
            length[curDoc] = invertedIndex.ivs_idx[postings_list->first].second.size()*tfd; //find length w/ normalization
            scores[curDoc].first  /= length[curDoc]; //score
            scores[curDoc].second = invertedIndex.ivs_idx[postings_list->first].second[d].doc; //doc id

        }
        //score / length

    }
    //calculate scores/lengths

    //return top scores and print them
    if(!terms.empty()) { //i have no idea why I need this if statement
        sort(scores, scores + N);
        for (int j = N - 1; j >= N - K; j--) {
            cout << topic << " " << fileDictionary.file_dict.at(scores[j].second) << " " << scores[j].second << " "
                 << " " << scores[j].first << endl;
            fout << topic << " " << fileDictionary.file_dict.at(scores[j].second) << " " << scores[j].second << " "
                 << " " << scores[j].first << endl;
        }
    }
}


std::vector<pair<int, std::vector<std::string>>> query; //it made me post it here


void topics_file_reader(){
    input_files files, paths;
    int numb;
    string line, num = "<num> Number:", title = "<title>", desc = "<desc> Description: ", narr = "<narr> Narrative:", token;
    vector<string> stop_words;
    bool check;
    int option;
    ifstream fin, stops;
    fin.open(files.topics_list);
    cout << "Setting:\n1 for just title\n2 for title and desc" << endl;
    cin >> option;
    //parse file
    stops.open(paths.stop_word_list);
    while (stops >> line) {
        stop_words.push_back(line);
    }
    while(!fin.eof()) {
        vector<string> temp;
        getline(fin, line);
        //cout << line << endl;
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
                            cout << token << endl;
                            token = stem_string(token);
                            cout << token << endl;
                            temp.push_back(token);
                        }
                    }
                }
                //cout << temp.size() << endl;

            }
            if(option == 1) {
                sort(temp.begin(), temp.end());
//                temp.erase(unique(temp.begin(), temp.end()), temp.end());
                query.emplace_back(numb, temp);
                temp.clear();
            }
        }
        if(line == desc && option == 2) {
            while(!fin.eof()) {
                getline(fin, line);
                if(line.empty()) {break;}
                //cout << line << endl;
                istringstream find(line);
                while (!find.eof()) {
                    getline(find, token, ' ');
                    if((token != title)&& !token.empty()) {
                        token.erase(std::remove_if(token.begin(), token.end(),
                                                   std::not1(std::ptr_fun((int (*)(int)) std::isalnum))), token.end());
                        token = toLowerCase(token);
                        check = false;
                        for (int i = 0; i < stop_words.size(); i++) {
                            if(token == stop_words[i]) {
                                check = true;
                                break;
                            }
                        }
                        if (!check) {
                            cout << token << endl;
                            token = stem_string(token);
                            cout << token << endl;
                            temp.push_back(token);
                        }
                    }
                }
            }
            sort(temp.begin(), temp.end());
//            temp.erase(unique(temp.begin(), temp.end()), temp.end());
            query.emplace_back(numb, temp);
            temp.clear();
        }
    }
}


void query_processor(file_dictionary fileDictionary, word_dictionary word_dictionary, forward_index forwardIndex, inverted_index invertedIndex) {

    //get query from topics.txt
    vector<pair<int, vector<float>>> scores;
    vector < string> stop_words;
    input_files paths;
    string line;
    ifstream stops;
    ofstream fout;
    char option; // 0 = query, 1 = query and desc, 2 = query, narrative, and desc
    stops.open(paths.stop_word_list);
    //make stopword
    while (stops >> line) {
        stop_words.push_back(line);
    }

    topics_file_reader();
    cout << "check" << endl;
    fout.open(paths.query_output);
    for (int i = 0; i < query.size(); ++i) {
        cout << "\nTopic: " << query.at(i).first << endl;
        cosine_similarities(query.at(i).second, query.at(i).first, fout, forwardIndex.fwd_idx.size(), invertedIndex, forwardIndex, fileDictionary, word_dictionary);
    }


}


