#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map> 
#include <functional>
#include <memory>
#include "trie_implementation.cc"

void FilterWordSize(int min, int max, std::ifstream& infile, std::ofstream& outfile) {
    std::string line;
    while (std::getline(infile, line)) {
        if (line.length() >= min && line.length() <= max) {
            outfile << line << std::endl;
        }
    }
}

std::vector<std::string> SplitWordIntoVector(const std::string& word, int n) {
    std::vector<std::string> parts;
    if (word.length() % n != 0) {
        return parts;
    }
    int part_length = word.length() / n;
    for (int i = 0; i < n; i++) {
        parts.push_back(word.substr(i * part_length, part_length));
    }
    return parts;
}

bool is_symmetric(const std::vector<std::vector<std::string>>& matrix) {
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      if (matrix[i][j] != matrix[j][i]) {
        return false;
      }
    }
  }
  return true;
}

std::vector<std::vector<std::string>> GeneralizedSymmetricWordMatrix(std::vector<std::string>& word_list, int split_size, Trie& trie) {
    int& n = split_size;
    std::vector<std::vector<std::string>> output; // List of n-word pairs that form a symmetric nxn matrix if split into n equal parts
    std::vector<std::string> current_word_list; // List of words that form the current matrix
    std::vector<std::vector<std::string>> current_matrix(n, std::vector<std::string>(n)); // Current nxn matrix, each row is a word split into n equal parts

    // Recursive helper function to generate symmetric word matrix
    std::function<void()> SymmetricWordMatrixHelper;
    SymmetricWordMatrixHelper = [&]() {
        if (current_word_list.size() == n) {
            if (is_symmetric(current_matrix)) { // use MatrixSymmetric instead of is_symmetric
                output.push_back(current_word_list);
            }
            return;
        }

        for (auto&& word : word_list) {
            std::vector<std::string> parts = SplitWordIntoVector(word, n);
            if (parts.empty()) {
                if (is_symmetric(current_matrix)) { // use MatrixSymmetric instead of is_symmetric
                    output.push_back(current_word_list);
                }
                return;
            }

            // Try all possible words for current part of current word
            std::vector<std::string> matching_words = trie.MatchPrefix(parts.back());
            for (auto&& word : matching_words) {
                current_word_list.push_back(word);
                current_matrix.back() = SplitWordIntoVector(word, n); // Replace last row in current matrix with new word
                SymmetricWordMatrixHelper();
                current_word_list.pop_back();
                current_matrix.back().clear(); // Clear last row in current matrix
            }
        }
    };

    // Call the recursive helper function to generate the symmetric word matrix
    SymmetricWordMatrixHelper();

    return output;
}

// main
int main() {
    // Read in words from file
    int n = 3;
    std::ifstream infile("words.txt");
    std::ofstream outfile("filtered_words.txt");
    FilterWordSize(n*n, n*n, infile, outfile);
    infile.close();
    outfile.close();

    // Read in filtered words from file
    std::ifstream filtered_words_file("filtered_words.txt");
    std::string line;
    std::vector<std::string> word_list;
    while (std::getline(filtered_words_file, line)) {
        word_list.push_back(line);
    }
    filtered_words_file.close();

    // Create trie
    Trie<char> trie;
    for (auto&& word : word_list) {
        std::vector<char> char_vec(word.begin(), word.end());
        trie.Insert(char_vec);
    }   

    // Generate solution space
    std::vector<std::vector<std::string>> symmetric_word_matrix = GeneralizedSymmetricWordMatrix(word_list, n, trie);

    // output solution space to file
    std::ofstream output_file("result.txt");
    for (auto&& word_list : symmetric_word_matrix) {
        for (auto&& word : word_list) {
            output_file << word << " ";
        }
        output_file << std::endl;
    }

    return 0;
}