//
// Created by White on 11/2/2020.
//

#ifndef CSCE4200_PROJECT_2_FILE_PATHS_H
#define CSCE4200_PROJECT_2_FILE_PATHS_H

#include <string>

using namespace std;

//Created to provide an easy way to read in files for users

//REPLACE THESE WITH YOUR OWN DIRECTORY

//WILL FIX LATER TO MAKE IT EASIER FOR YOU TO TYPE YOUR DIRECTORIES

struct input_files{
    //directory to read
    string ft911_directory = "C:/Users/White/Documents/CSCE4200_Project_3/ft911/";
    //stop words
    string stop_word_list = "C:/Users/White/Documents/CSCE4200_Project_3/Given files/stopwordlist.txt";
    //word dictionary
    string word_dict = "C:/Users/White/Documents/CSCE4200_Project_3/Data_Files/word_dict.txt";
    //file dictionary
    string file_dict = "C:/Users/White/Documents/CSCE4200_Project_3/Data_Files/file_dict.txt";
    //forward index
    string fwd_index_file = "C:/Users/White/Documents/CSCE4200_Project_3/Data_Files/forward_index.txt";
    //inverted index
    string ivs_index_file = "C:/Users/White/Documents/CSCE4200_Project_3/Data_Files/inverse_index.txt";
    //vector space output model output
    string topics_list = "C:/Users/White/Documents/CSCE4200_Project_3/Given files/topics.txt";
    //outputs of query
    string query_output = "C:/Users/White/Documents/CSCE4200_Project_3/Data_Files/vsm_output.txt";

};

#endif //CSCE4200_PROJECT_2_FILE_PATHS_H
