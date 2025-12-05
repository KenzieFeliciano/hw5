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

// main wordle function - generates all combos then checks dict
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

// recursive function to build words
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
    
    // current position is a dash, try all 26 letters (loop 5)
    for (char c = 'a'; c <= 'z'; c++) {
        current_word += c;
        buildWords(pattern, floating, pos + 1, current_word, dict, results, floating_needed);
        current_word.pop_back(); // backtrack
    }
}
