/*
 * Copyright ©2022 Phuoc Huynh & Isabella Bunnell.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <iostream>
#include <string>
#include <map>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::string;

template <typename T>
bool ReadValue(ifstream &in, T *output);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        cout << "Please provide a filename as the first argument only.";
        return EXIT_FAILURE;
    }
    map<string, int> wordcount;
    string filename = argv[1];
    ifstream txtfile;
    string word;
    txtfile.open(filename.c_str());
    if (!txtfile.is_open()) {
        cout << "Unable to open file";
        return EXIT_FAILURE;
    }
    while (ReadValue<string>(txtfile, &word)) {
        // finally, iterate through map
        // see if word is already in map
        // then increase the count, otherwise create a new key.
        if (wordcount.find(word) == wordcount.end()) {
            wordcount[word] = 1;  // set to 1
        } else {
            wordcount[word]++;  // increment word count
        }
    }
    txtfile.close();
    for (auto const &x : wordcount) {
        cout << x.first << " " << x.second << endl;
    }
    return EXIT_SUCCESS;
}

template <typename T>
        bool ReadValue(ifstream &in, T *output) {
    if (in >> (*output) && !in.eof()) {
        return true;
    } else {
        return false;
    }
}
