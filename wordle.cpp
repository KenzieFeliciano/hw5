#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function to recursively build words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results);

// helper to check if word contains all floating letters
bool hasAllFloating(const string& word, const string& floating);

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    buildWords(in, floating, 0, "", dict, results);
    return results;
}

// recursive function to build all possible words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results)
{
    // base case: if we've filled the entire word
    if (pos == pattern.length()) {
        // check if it's a real word and has all floating letters
        if (dict.find(current_word) != dict.end() && hasAllFloating(current_word, floating)) {
            results.insert(current_word);
        }
        return;
    }
    
    // if this position is fixed, use that letter
    if (pattern[pos] != '-') {
        buildWords(pattern, floating, pos + 1, current_word + pattern[pos], dict, results);
    } else {
        // try all letters a-z for this position
        for (char c = 'a'; c <= 'z'; c++) {
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results);
        }
    }
}

// check if word contains all required floating letters
bool hasAllFloating(const string& word, const string& floating) {
    for (int i = 0; i < floating.length(); i++) {
        bool found = false;
        for (int j = 0; j < word.length(); j++) {
            if (word[j] == floating[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    return true;
}
