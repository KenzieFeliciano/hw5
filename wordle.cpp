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

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    // CRITICAL OPTIMIZATION: handle empty pattern immediately
    if (in.empty()) {
        return results; // empty set for empty pattern
    }
    
    buildWords(in, floating, 0, "", dict, results);
    return results;
}

//recursive function with prefix-based pruning
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results)
{
    // base case: if we've filled the entire word
    if (pos == (int)pattern.length()) {
        // check if it's a real word first (fast dictionary lookup)
        if (dict.find(current_word) != dict.end()) {
            // then check floating letters - efficient single loop
            for (int i = 0; i < (int)floating.length(); i++) {
                if (current_word.find(floating[i]) == string::npos) {
                    return; // missing required floating letter
                }
            }
            results.insert(current_word);
        }
        return;
    }

    // REVOLUTIONARY OPTIMIZATION: Use dictionary as prefix filter
    // Instead of trying all 26 letters, only try letters that could lead to valid words
    set<char> valid_next_chars;
    
    // Find all possible next characters by checking dictionary prefixes
    for (const string& word : dict) {
        if (word.length() == pattern.length() && 
            word.length() > pos &&
            word.substr(0, pos) == current_word) {
            valid_next_chars.insert(word[pos]);
        }
    }
    
    // SMART PRUNING: early exit if impossible to place remaining floating letters
    if (!floating.empty()) {
        int remaining_spots = (int)pattern.length() - pos;
        int still_need = 0;
        for (int i = 0; i < (int)floating.length(); i++) {
            if (current_word.find(floating[i]) == string::npos) {
                still_need++;
            }
        }
        if (still_need > remaining_spots) {
            return; // can't fit remaining floating letters
        }
    }

    // if this position is fixed, use that letter
    if (pattern[pos] != '-') {
        char required_char = pattern[pos];
        if (valid_next_chars.find(required_char) != valid_next_chars.end()) {
            buildWords(pattern, floating, pos + 1, current_word + required_char, dict, results);
        }
    } else {
        // Only try letters that could lead to valid dictionary words
        for (char c : valid_next_chars) {
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results);
        }
    }
}
