Tristan White
Contact: tristanwhite@my.unt.edu tristanwhite.gva@gmail.com Canvas

NOTE: This program was created with CLION which is free with a .edu email

To run:
Put all files in zip in a directory. Keep folders in zip in folders and put them in dir.
Given Files are just from assignment
compile with gcc or g++

Ensure c++17 is installed
use -std=c++17 or -std=gnu++17 to compile

-03 and -stdc++fs must be used in compile arguments
(-stdc++fs maybe not as it is already in the CMakeLists.txt)

CMakeLists.txt is crutial to run the program
as CMakeLists.txt make us the target and executable

To include the files needed, go to file_paths.h and enter int the file paths on the respective string


Run the program and press '2' to tokenize and index the files and after this is done,
Run the program again and press '1' to run the query search

If the files forward_index.txt, inverse_index.txt, file_dict.txt, word_dict.txt exist,
you can immediately go towards 2 as long as they are in correct format

If pressed 1 initially, you will have to chose 1 or 2 for the different query formats
1 for just "title" 2 for "title" and "desc". I had no time to incorporate

To compare the results, you will have to see what documents in the main.grels are deemed relevent, and
compare them to the displayed results. You can also go to the retrieved documents


If you have any questions, contact me.