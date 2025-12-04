#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function to build words recursively 
void buildWord(string word_so_far, int position, const string& fixed_pattern, 
               const string& must_have_letters, const set<string>& dictionary, 
               set<string>& answer_words);

// helper function to check if word has all the letters we need
bool checkHasAllLetters(const string& test_word, const string& required_letters);

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> valid_words;
    
    // start building words from the beginning
    buildWord("", 0, in, floating, dict, valid_words);
    
    return valid_words;
}

// this function tries to build a word one letter at a time
void buildWord(string word_so_far, int position, const string& fixed_pattern, 
               const string& must_have_letters, const set<string>& dictionary, 
               set<string>& answer_words)
{
    // if we filled the whole word, check if its valid
    if (position == fixed_pattern.length()) {
        // check if its a real word in the dictionary first (faster than letter check)
        if (dictionary.find(word_so_far) != dictionary.end()) {
            // only then check if it has all the letters we need
            if (checkHasAllLetters(word_so_far, must_have_letters)) {
                answer_words.insert(word_so_far);
            }
        }
        return;
    }
    
    // if theres already a letter here, just use it
    if (fixed_pattern[position] != '-') {
        word_so_far = word_so_far + fixed_pattern[position];
        buildWord(word_so_far, position + 1, fixed_pattern, must_have_letters, dictionary, answer_words);
    } else {
        // this spot is empty so try every letter from a to z
        for (char letter = 'a'; letter <= 'z'; letter++) {
            string new_word = word_so_far + letter;
            buildWord(new_word, position + 1, fixed_pattern, must_have_letters, dictionary, answer_words);
        }
    }
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
