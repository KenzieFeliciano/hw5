#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// main function that finds all valid wordle words
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> valid_words;
    
    // aggressive early exit optimizations
    if (in.empty()) {
        return valid_words;
    }
    
    // pre-compute pattern info for faster checking
    int target_length = in.length();
    bool has_floating = !floating.empty();
    
    // optimize floating letter checking with char counts
    int floating_counts[26] = {0};
    if (has_floating) {
        for (char c : floating) {
            floating_counts[c - 'a']++;
        }
    }
    
    // iterate dictionary with maximum pruning
    for (const string& word : dict) {
        // length check first - fastest elimination
        if (word.length() != target_length) {
            continue;
        }
        
        // pattern check with early exit
        bool matches_pattern = true;
        for (int i = 0; i < target_length; i++) {
            if (in[i] != '-' && in[i] != word[i]) {
                matches_pattern = false;
                break;
            }
        }
        if (!matches_pattern) {
            continue;
        }
        
        // optimized floating letter check using character counts
        if (has_floating) {
            int word_counts[26] = {0};
            for (char c : word) {
                word_counts[c - 'a']++;
            }
            
            bool has_all_floating = true;
            for (int i = 0; i < 26; i++) {
                if (floating_counts[i] > 0 && word_counts[i] < floating_counts[i]) {
                    has_all_floating = false;
                    break;
                }
            }
            
            if (!has_all_floating) {
                continue;
            }
        }
        
        valid_words.insert(word);
    }
    
    return valid_words;
}
