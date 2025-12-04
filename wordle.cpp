#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> valid_words;
    
    // use STL algorithms for maximum efficiency
    for (const string& word : dict) {
        // length check first - fastest elimination
        if (word.length() != in.length()) {
            continue;
        }
        
        // pattern check using STL mismatch algorithm
        auto mismatch_pair = std::mismatch(in.begin(), in.end(), word.begin(),
            [](char pattern_char, char word_char) {
                return pattern_char == '-' || pattern_char == word_char;
            });
        
        if (mismatch_pair.first != in.end()) {
            continue; // pattern doesn't match
        }
        
        // floating letter check using STL find
        bool has_all_floating = true;
        for (char letter : floating) {
            if (std::find(word.begin(), word.end(), letter) == word.end()) {
                has_all_floating = false;
                break;
            }
        }
        
        if (has_all_floating) {
            valid_words.insert(word);
        }
    }
    
    return valid_words;
}
