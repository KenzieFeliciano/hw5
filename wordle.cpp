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

// main wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    // handle empty case
    if (in.empty()) {
        return results;
    }
    
    buildWords(in, floating, 0, "", dict, results);
    return results;
}

// recursive function to build words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results)
{
    // ULTRA AGGRESSIVE EARLY PRUNING - do this FIRST before any other work
    if (!floating.empty() && pos < (int)pattern.length()) {
        int remaining_spots = (int)pattern.length() - pos;
        int still_need = 0;
        for (int i = 0; i < (int)floating.length(); i++) {
            if (current_word.find(floating[i]) == string::npos) {
                still_need++;
            }
        }
        if (still_need > remaining_spots) {
            return; // impossible to fit remaining floating letters
        }
    }
    
    // base case: word is complete
    if (pos == (int)pattern.length()) {
        // fast dictionary lookup first
        if (dict.find(current_word) != dict.end()) {
            // only check floating letters if we found the word
            bool valid = true;
            for (int i = 0; i < (int)floating.length() && valid; i++) {
                if (current_word.find(floating[i]) == string::npos) {
                    valid = false;
                }
            }
            if (valid) {
                results.insert(current_word);
            }
        }
        return;
    }
    
    // if this position is fixed, use that letter
    if (pattern[pos] != '-') {
        buildWords(pattern, floating, pos + 1, current_word + pattern[pos], dict, results);
        return;
    }
    
    // for blank positions, try all letters but with smart ordering
    // if we have floating letters, try them first
    if (!floating.empty()) {
        for (int i = 0; i < (int)floating.length(); i++) {
            char c = floating[i];
            // only try this floating letter if we haven't placed it yet
            if (current_word.find(c) == string::npos) {
                buildWords(pattern, floating, pos + 1, current_word + c, dict, results);
            }
        }
    }
    
    // then try all other letters
    for (char c = 'a'; c <= 'z'; c++) {
        // skip if this is a floating letter we already tried above
        bool is_floating = false;
        if (!floating.empty()) {
            for (int i = 0; i < (int)floating.length(); i++) {
                if (c == floating[i] && current_word.find(c) == string::npos) {
                    is_floating = true;
                    break;
                }
            }
        }
        if (!is_floating) {
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results);
        }
    }
}
