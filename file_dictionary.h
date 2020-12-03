//
// Created by White on 10/7/2020.
//

#ifndef CSCE4200_PROJECT_1_FILE_DICTIONARY_H
#define CSCE4200_PROJECT_1_FILE_DICTIONARY_H


#include <fstream>
#include <string>
#include <vector>
#include <map>

/*
 * File Dictionary class made for future project use
 */
class file_dictionary {
private:
    //Makes unordered map


public:
    std::map <int,std::string> file_dict;
    //generates unique id
    //Prints to file
    int return_key(std::string document);
    void make_file_dict(std::string document, int docNum);

    //print function. To be made later
    void print_dict();
};

#endif //CSCE4200_PROJECT_1_FILE_DICTIONARY_H
