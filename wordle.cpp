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
                string current_word, const set<string>& dict, set<string>& results,
                set<char>& placed_floating);

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
    
    set<char> placed_floating;
    buildWords(in, floating, 0, "", dict, results, placed_floating);
    return results;
}

// recursive function to build words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results,
                set<char>& placed_floating)
{
    // ULTRA AGGRESSIVE EARLY PRUNING - do this FIRST before any other work
    if (!floating.empty() && pos < (int)pattern.length()) {
        int remaining_spots = (int)pattern.length() - pos;
        int still_need = 0;
        for (char c : floating) {
            if (placed_floating.find(c) == placed_floating.end()) {
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
            for (char c : floating) {
                if (placed_floating.find(c) == placed_floating.end()) {
                    valid = false;
                    break;
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
        char fixed_char = pattern[pos];
        // track if this fixed char is a floating letter
        set<char> new_placed = placed_floating;
        if (floating.find(fixed_char) != string::npos) {
            new_placed.insert(fixed_char);
        }
        buildWords(pattern, floating, pos + 1, current_word + fixed_char, dict, results, new_placed);
        return;
    }
    
    // for blank positions, try all letters but with smart ordering
    // if we have floating letters, try them first
    if (!floating.empty()) {
        for (char c : floating) {
            // only try this floating letter if we haven't placed it yet
            if (placed_floating.find(c) == placed_floating.end()) {
                set<char> new_placed = placed_floating;
                new_placed.insert(c);
                buildWords(pattern, floating, pos + 1, current_word + c, dict, results, new_placed);
            }
        }
    }
    
    // then try all other letters
    for (char c = 'a'; c <= 'z'; c++) {
        // skip if this is a floating letter we already tried above
        if (floating.find(c) == string::npos) {
            // for non-floating letters, don't modify the placed_floating set
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results, placed_floating);
        }
    }
}
