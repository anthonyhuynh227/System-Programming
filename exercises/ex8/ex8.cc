#include <iostream>
#include <string>
#include <map>
#include <ifstream>
using namespace std;
int main(int argc, char* argv[]) {
    if(argc < 2 || argc > 3) {
        cout << "Please provide a filename as the first argument only.";
    }
    map<string, int> wordcount;
    string filename = argv[1];
    bool printFile = ReadValue(filename, &wordcount)
    // finally, iterate through map (since map stores strings in alphabetical
    // order anyway) and print them out
    for (auto const& [key,val] : wordcount) {
        cout << key << " " << val << endl;
    }
}

template <typename T> bool ReadValue(ifstream& in, T* output) {
    // create map pairing for word to how many times it appears in the file
    // https://courses.cs.washington.edu/courses/cse333/22wi/lectures/15/code/animals.cc.html

    ifstream txtfile(filename);

    map<string, int> wordcount = *output;
    if (txtfile.is_open()) {
        txtfile.open(filename.c_str());
        while (txtfile.good()) {
            string next_word;
            txtfile >> next_word;

            // see if word is already in map
            if (wordcount.find(next_word) == wordscount.end()) { // encountered the word for first time.
                wordcount[next_word] = 1; // set to 1
            }
            else {
                wordcount[next_word]++; // increment word count
            }
        }
    } else {
        cout << "Unable to open file";
        return false; 
    }
    txtfile.close();
}

