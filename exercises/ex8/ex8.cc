#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

template <typename T>
bool ReadValue(ifstream &in, T *output);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        cout << "Please provide a filename as the first argument only.";
    }
    map<string, int> wordcount;
    string filename = argv[1];
    ifstream txtfile;
    string word;
    txtfile.open(filename.c_str());
    while (ReadValue<string>(txtfile, &word)) {
        // finally, iterate through map (since map stores strings in alphabetical
        // order anyway) and print them out
        // see if word is already in map
        if (wordcount.find(word) == wordcount.end()) {                        // encountered the word for first time.
            wordcount[word] = 1; // set to 1
        } else {
            wordcount[word]++; // increment word count
        }
    }
    txtfile.close();
    for (auto const &x : wordcount) {
        cout << x.first << " " << x.second << endl;
    }
}

template <typename T>
bool ReadValue(ifstream &in, T *output) {
    if (in >> (*output)) {
        return true;
    } else {
        return false;
    }
}
