#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// complaint placement intention
// ->
// com pla int
// pla cem ent
// int ent ion

void FilterWordSize(int n, std::string filename, std::string new_filename) {
// go through file and copy all lines with at least n characters to a new file
    std::ifstream infile(filename);
    std::ofstream outfile(new_filename);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.length() >= n) {
            outfile << line << std::endl;
        }
    }
}

void RemoveWordDuplicates(std::string filename) {
// remove duplicates in a file
    std::ifstream infile(filename);
    std::ofstream outfile(filename);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    std::sort(lines.begin(), lines.end());
    lines.erase(std::unique(lines.begin(), lines.end()), lines.end());
    for (auto line : lines) {
        outfile << line << std::endl;
    }
}

// takes a word with 3*n letters (n>0), outputs a vector of all possible three word triples where:
//1. the first word is input word, (you can think of the word as something consisting out of three parts of equal size (size n))
//2. second word is a word found in the dictionary, the first n letters of the word are the second part of the first word, last three letters are second part of the third word
//3. third word is a word found in the dictionary, the firsts n letters of the word are the third part of the first word
void SymmetricWordTriple(std::string word_input, std::string filename, std::string output_filename) {
    // copy of input word
    std::string word1 = word_input;
    // convert to lowercase
    std::transform(word1.begin(), word1.end(), word1.begin(), ::tolower);

    // split word1 into three parts
    int n = word1.length() / 3;
    std::string firstpart_word1 = word1.substr(0, n);
    std::string secondpart_word1 = word1.substr(n, n);
    std::string thirdpart_word1 = word1.substr(2*n, n);

    // go through the file with a loop
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        // convert to lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        
        // we check if the first n letters of the second word in the current line are equal to the second part of the first word
        if (line.substr(0, n) != secondpart_word1) {
            continue;
        }
        // first check succeeded. Now make a copy of the last n letters of the second word
        std::string thirdpart_word2 = line.substr(line.length() - n, n);
        // make another loop to search for a third word that fits with the two words we have
        std::ifstream infile2(filename);
        std::string line2;
        while (std::getline(infile2, line2)) {
            // convert to lowercase
            std::transform(line2.begin(), line2.end(), line2.begin(), ::tolower);
            // check if the first n letters of the word in the current line are equal to the third part of the first word
            if (line2.substr(0, n) != thirdpart_word1) {
                continue;
            }
            // check if the last second part of the third (current line) word is equal to the third part of the third word
            if (line2.substr(line2.length() - n, n) != thirdpart_word2) {
                continue;
            }
            
            // all checks succeeded. Add the three words to the result vector
            std::vector<std::string> triple;
            triple.push_back(word1);
            triple.push_back(line);
            triple.push_back(line2);
            
            // add the triple to the output file
            std::ofstream outfile(output_filename);
            for (auto&& word : triple) {
                outfile << word << " ";
            }
        }
    }
}


// test it
int main() {
    // filter the dictionary file to only contain words with at least 9 letters
    FilterWordSize(9, "words.txt", "words_filtered.txt");
    // remove duplicates
    RemoveWordDuplicates("words_filtered.txt");
    // words are now prepared

    // find all symmetric word triples and output them to a file
    SymmetricWordTriple("complaint", "words_filtered.txt", "result.txt");
}