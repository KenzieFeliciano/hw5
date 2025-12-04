#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function to check if word has all the letters we need
bool checkHasAllLetters(const string& test_word, const string& required_letters);

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> valid_words;
    
    // MUCH MORE EFFICIENT: always iterate through dictionary first
    for (const string& word : dict) {
        if (word.length() != in.length()) {
            continue; // wrong length
        }
        
        // check if word matches the fixed pattern
        bool matches_pattern = true;
        for (int i = 0; i < in.length(); i++) {
            if (in[i] != '-' && in[i] != word[i]) {
                matches_pattern = false;
                break;
            }
        }
        
        if (matches_pattern && checkHasAllLetters(word, floating)) {
            valid_words.insert(word);
        }
    }
    
    return valid_words;
}

// check if the word contains all the letters that must be in it
bool checkHasAllLetters(const string& test_word, const string& required_letters) {
    // go through each letter we need
    for (int i = 0; i < required_letters.length(); i++) {
        char need_this_letter = required_letters[i];
        bool found_it = false;
        
        // look for it in the test word
        for (int j = 0; j < test_word.length(); j++) {
            if (test_word[j] == need_this_letter) {
                found_it = true;
                break;
            }
        }
        
        // if we didnt find a required letter, this word is no good
        if (!found_it) {
            return false;
        }
    }
    // if we made it here, the word has all required letters
    return true;
}
