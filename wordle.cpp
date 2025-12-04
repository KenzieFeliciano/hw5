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
    buildWords(in, floating, 0, "", dict, results);
    return results;
}

// recursive function to build all possible words
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
    
    // ULTRA AGGRESSIVE PRUNING: early exit if impossible to place remaining floating letters
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
        buildWords(pattern, floating, pos + 1, current_word + pattern[pos], dict, results);
    } else {
        // CRITICAL OPTIMIZATION: try floating letters first if we need them
        if (!floating.empty()) {
            // try floating letters that we haven't placed yet
            for (int i = 0; i < (int)floating.length(); i++) {
                char needed_char = floating[i];
                if (current_word.find(needed_char) == string::npos) {
                    buildWords(pattern, floating, pos + 1, current_word + needed_char, dict, results);
                }
            }
        }
        
        // then try all other letters
        for (char c = 'a'; c <= 'z'; c++) {
            // skip if this is a floating letter we already tried
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
}
