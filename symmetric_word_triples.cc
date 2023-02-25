#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

void FilterWordSize(int min, int max, std::string filename, std::string new_filename) {
    std::ifstream infile(filename);
    std::ofstream outfile(new_filename);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.length() >= min && line.length() <= max) {
            outfile << line << std::endl;
        }
    }
}

void SymmetricWordTriple(std::string word_input, std::string filename, std::string output_filename, bool append_to_file) {
    std::string word1 = word_input;
    std::transform(word1.begin(), word1.end(), word1.begin(), ::tolower);

    int n = word1.length() / 3;
    std::string firstpart_word1 = word1.substr(0, n);
    std::string secondpart_word1 = word1.substr(n, n);
    std::string thirdpart_word1 = word1.substr(2*n, n);

    std::ofstream outfile;
    if (append_to_file) {
        outfile.open(output_filename, std::ios::app);
    } else {
        outfile.open(output_filename);
    }

    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (line.substr(0, n) != secondpart_word1) {
            continue;
        }
        std::string thirdpart_word2 = line.substr(2*n, n);
        std::ifstream infile2(filename);
        std::string line2;
        while (std::getline(infile2, line2)) {
            std::transform(line2.begin(), line2.end(), line2.begin(), ::tolower);

            if (line2.substr(0, n) != thirdpart_word1) {
                continue;
            }
            if (line2.substr(n, n) != thirdpart_word2) {
                continue;
            }
            std::vector<std::string> triple;
            triple.push_back(word1);
            triple.push_back(line);
            triple.push_back(line2);
            for (auto&& word : triple) {
                outfile << word << " ";
            }
            outfile << "\n";
        }
    }

    outfile.close();
}

int main() {
    std::string dictionary = "words.txt";
    std::string filtered_dictionary = "words_filtered.txt";
    std::string output_filename = "result.txt";

    FilterWordSize(9, 9, dictionary, filtered_dictionary);

    std::string curr_word;
    std::ifstream infile(filtered_dictionary);

    if (true) {
        while (std::getline(infile, curr_word)) {
            SymmetricWordTriple(curr_word, filtered_dictionary, output_filename, true);
        }
    }
}