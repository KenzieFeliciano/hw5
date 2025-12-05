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

// SUPER OPTIMIZED recursive function with maximum pruning
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
    
    // MASSIVE OPTIMIZATION: if no floating letters, skip expensive pruning
    if (floating.empty()) {
        // if this position is fixed, use that letter
        if (pattern[pos] != '-') {
            buildWords(pattern, floating, pos + 1, current_word + pattern[pos], dict, results);
        } else {
            // try all letters a-z for this position
            for (char c = 'a'; c <= 'z'; c++) {
                buildWords(pattern, floating, pos + 1, current_word + c, dict, results);
            }
        }
        return;
    }
    
    // SMART PRUNING: early exit if impossible to place remaining floating letters
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
