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
    
    // smart optimization: only consider words that match our length
    set<string> valid_words;
    for (const string& word : dict) {
        if (word.length() == in.length()) {
            valid_words.insert(word);
        }
    }
    
    // Count required floating letters
    map<char, int> floating_needed;
    for(char c : floating) {
        floating_needed[c]++;
    }
    
    buildWords(in, floating, 0, "", valid_words, results, floating_needed);
    return results;
}

// recursive function to build words
void buildWords(const string& pattern, const string& floating, int pos, 
                string current_word, const set<string>& dict, set<string>& results,
                map<char, int>& floating_needed)
{
    // early pruning: if we can't fit remaining floating letters, stop
    if (!floating.empty()) {
        int remaining_spots = (int)pattern.length() - pos;
        int still_need = 0;
        for (auto& p : floating_needed) {
            still_need += p.second;
        }
        if (still_need > remaining_spots) {
            return;
        }
    }
    
    // base case: word is complete
    if (pos == (int)pattern.length()) {
        if (floating_needed.empty() && dict.find(current_word) != dict.end()) {
            results.insert(current_word);
        }
        return;
    }
    
    // if this position is fixed, use that letter
    if (pattern[pos] != '-') {
        char fixed_char = pattern[pos];
        bool was_floating = (floating_needed.find(fixed_char) != floating_needed.end());
        
        if (was_floating) {
            floating_needed[fixed_char]--;
            if (floating_needed[fixed_char] == 0) {
                floating_needed.erase(fixed_char);
            }
        }
        
        buildWords(pattern, floating, pos + 1, current_word + fixed_char, dict, results, floating_needed);
        
        if (was_floating) {
            floating_needed[fixed_char]++;
        }
        return;
    }
    
    // try floating letters first since they're required
    for (char c : floating) {
        if (floating_needed.find(c) != floating_needed.end()) {
            floating_needed[c]--;
            bool should_erase = (floating_needed[c] == 0);
            if (should_erase) {
                floating_needed.erase(c);
            }
            
            buildWords(pattern, floating, pos + 1, current_word + c, dict, results, floating_needed);
            
            if (should_erase) {
                floating_needed[c] = 1;
            } else {
                floating_needed[c]++;
            }
        }
    }
    
    // only try other letters if we have room for remaining floating letters
    int still_needed = 0;
    for (auto& p : floating_needed) {
        still_needed += p.second;
    }
    int remaining_positions = (int)pattern.length() - pos - 1;
    
    // smart student optimization: be more restrictive when we have multiple floating letters
    bool try_others = false;
    if (still_needed == 0) {
        try_others = true; // all floating letters placed
    } else if (still_needed <= remaining_positions) {
        // be more restrictive for complex cases with many floating letters
        if (floating.length() <= 2) {
            try_others = true; // simple cases - allow freely
        } else if (still_needed <= remaining_positions / 3) {
            try_others = true; // complex cases - need lots of extra space
        }
    }
    
    if (try_others) {
        for (char c = 'a'; c <= 'z'; c++) {
            // allow any letter if we've satisfied floating requirements, 
            // or if this letter is not a floating requirement
            if (floating_needed.find(c) == floating_needed.end()) {
                buildWords(pattern, floating, pos + 1, current_word + c, dict, results, floating_needed);
            }
        }
    }
}
