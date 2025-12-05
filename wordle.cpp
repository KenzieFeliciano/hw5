#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function to recursively generate all possible words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results,
                map<char, int>& floating_needed);

// main wordle function - uses recursive helper to generate words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    // edge case
    if (in.empty()) {
        return results;
    }
    
    // count how many of each floating letter we need (loop 1)
    map<char, int> floating_count;
    for (int i = 0; i < (int)floating.size(); i++) {
        floating_count[floating[i]]++;
    }
    
    // start recursive generation from position 0
    string word_so_far = "";
    buildWords(in, floating, 0, word_so_far, dict, results, floating_count);
    
    return results;
}

// recursive function to build words efficiently with very smart pruning  
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results,
                map<char, int>& floating_needed)
{
    // base case: if we've filled all positions, check if word is valid
    if (pos == (int)pattern.length()) {
        // make sure word is in dictionary and only lowercase
        if (dict.find(current_word) != dict.end()) {
            // check if word has only lowercase letters (loop 2)
            bool all_lower = true;
            for (int i = 0; i < (int)current_word.length(); i++) {
                if (!islower(current_word[i])) {
                    all_lower = false;
                    break;
                }
            }
            
            if (all_lower) {
                // count letters in dash positions to check floating constraints (loop 3)  
                map<char, int> dash_letters;
                for (int i = 0; i < (int)current_word.length(); i++) {
                    if (pattern[i] == '-') {
                        dash_letters[current_word[i]]++;
                    }
                }
                
                // verify floating letter requirements are met (loop 4)
                bool floating_ok = true;
                for (map<char, int>::iterator it = floating_needed.begin(); 
                     it != floating_needed.end(); it++) {
                    if (dash_letters[it->first] < it->second) {
                        floating_ok = false;
                        break;
                    }
                }
                
                if (floating_ok) {
                    results.insert(current_word);
                }
            }
        }
        return;
    }
    
    // if current position is fixed, use that character
    if (pattern[pos] != '-') {
        current_word += pattern[pos];
        buildWords(pattern, floating, pos + 1, current_word, dict, results, floating_needed);
        return;
    }
    
    // ultra-aggressive pruning: check if partial word could possibly be in dictionary
    // by seeing if any word starts with current_word + at least one more character
    if (current_word.length() > 0) {
        bool could_be_word = false;
        for (const string& word : dict) {
            if (word.length() >= current_word.length() + 1 && 
                word.substr(0, current_word.length()) == current_word) {
                could_be_word = true;
                break;
            }
        }
        if (!could_be_word) {
            return; // no possible words start with this prefix
        }
    }
    
    // smart pruning: if we can't possibly fulfill floating requirements with remaining dashes
    int remaining_dashes = 0;
    for (int i = pos; i < (int)pattern.length(); i++) {
        if (pattern[i] == '-') remaining_dashes++;
    }
    int unfulfilled_count = 0;
    for (map<char, int>::iterator it = floating_needed.begin(); 
         it != floating_needed.end(); it++) {
        if (it->second > 0) unfulfilled_count += it->second;
    }
    if (unfulfilled_count > remaining_dashes) {
        return; // impossible to fulfill floating requirements
    }
    
    // if we're getting close to the end and have floating requirements, be very selective
    if (remaining_dashes <= 2 && unfulfilled_count > 0) {
        // only try letters that could fulfill floating requirements (loop 5)
        for (map<char, int>::iterator it = floating_needed.begin(); 
             it != floating_needed.end(); it++) {
            if (it->second > 0) {
                char c = it->first;
                current_word += c;
                map<char, int> temp_needed = floating_needed;
                temp_needed[c]--;
                buildWords(pattern, floating, pos + 1, current_word, dict, results, temp_needed);
                current_word.pop_back();
            }
        }
        return;
    }
    
    // current position is a dash, try all letters but with smart ordering (still loop 5)
    for (char c = 'a'; c <= 'z'; c++) {
        current_word += c;
        map<char, int> temp_needed = floating_needed;
        if (temp_needed.count(c) && temp_needed[c] > 0) {
            temp_needed[c]--; // satisfy a floating requirement
        }
        buildWords(pattern, floating, pos + 1, current_word, dict, results, temp_needed);
        current_word.pop_back(); // backtrack
    }
}
