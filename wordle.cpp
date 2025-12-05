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
                map<char, int>& floating_needed);

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
    
    // Count required floating letters
    map<char, int> floating_needed;
    for(char c : floating) {
        floating_needed[c]++;
    }
    
    buildWords(in, floating, 0, "", dict, results, floating_needed);
    return results;
}

// recursive function to build words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results,
                map<char, int>& floating_needed)
{
    // ULTRA AGGRESSIVE EARLY PRUNING - do this FIRST before any other work
    if (!floating.empty() && pos < (int)pattern.length()) {
        int remaining_spots = (int)pattern.length() - pos;
        int still_need = 0;
        for (auto& p : floating_needed) {
            still_need += p.second;
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
            for (auto& p : floating_needed) {
                if (p.second > 0) {
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
        bool was_floating = false;
        if (floating_needed.find(fixed_char) != floating_needed.end()) {
            floating_needed[fixed_char]--;
            if (floating_needed[fixed_char] == 0) {
                floating_needed.erase(fixed_char);
            }
            was_floating = true;
        }
        
        buildWords(pattern, floating, pos + 1, current_word + fixed_char, dict, results, floating_needed);
        
        // restore state
        if (was_floating) {
            floating_needed[fixed_char]++;
        }
        return;
    }
    
    // for blank positions, try all letters but with smart ordering
    // if we have floating letters, try them first
    if (!floating.empty()) {
        for (char c : floating) {
            // only try this floating letter if we still need it
            if (floating_needed.find(c) != floating_needed.end()) {
                // modify state
                floating_needed[c]--;
                bool should_erase = (floating_needed[c] == 0);
                if (should_erase) {
                    floating_needed.erase(c);
                }
                
                buildWords(pattern, floating, pos + 1, current_word + c, dict, results, floating_needed);
                
                // restore state
                if (should_erase) {
                    floating_needed[c] = 1;
                } else {
                    floating_needed[c]++;
                }
            }
        }
    }
    
    // then try all other letters
    for (char c = 'a'; c <= 'z'; c++) {
        // skip if this is a floating letter we already tried above
        if (floating.find(c) == string::npos) {
            // for non-floating letters, don't modify the floating_needed map
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results, floating_needed);
        }
    }
}
