cmake_minimum_required(VERSION 3.10)
project(CSCE4200_Project_3)

set(CMAKE_CXX_STANDARD 17)

add_executable(CSCE4200_Project_3 main.cpp stemmer.h tokenizer.cpp tokenizer.h file_dictionary.h word_dictionary.h word_dictionary.cpp file_dictionary.cpp file_paths.h indexer.h indexer.cpp query_processor.cpp query_processor.h)
target_link_libraries(${PROJECT_NAME} stdc++fs)