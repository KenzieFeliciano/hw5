#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper function to recursively build words
void findWords(const string& pattern, int position, 
               string& currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& remainingFloating);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    if (in.empty()) {
        return results;
    }
    
    // need to track floating letters and how many times each one appears
    map<char, int> remainingFloating;
    for (int i = 0; i < (int)floating.size(); i++) {
        remainingFloating[floating[i]]++;
    }
    
    string currentWord = in;
    
    findWords(in, 0, currentWord, dict, results, remainingFloating);
    
    return results;
}

void findWords(const string& pattern, int position, 
               string& currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& remainingFloating)
{
    // Base case: we've filled all positions in the word
    if (position == (int)pattern.length()) {
        // check if we used all the floating letters
        for (const auto& pair : remainingFloating) {
            if (pair.second > 0) {
                return; // still have unused floating letters, this word won't work
            }
        }
        
        // see if this word exists in the dictionary
        if (dict.find(currentWord) != dict.end()) {
            results.insert(currentWord);
        }
        return;
    }
    
    // if this spot already has a letter (not a dash), just move to the next position
    if (pattern[position] != '-') {
        findWords(pattern, position + 1, currentWord, dict, results, remainingFloating);
        return;
    }
    
    // count remaining dashes and floating letters once
    int dashesLeft = 0;
    for (int i = position; i < (int)pattern.length(); i++) {
        if (pattern[i] == '-') {
            dashesLeft++;
        }
    }
    
    int floatingLeft = 0;
    for (const auto& pair : remainingFloating) {
        floatingLeft += pair.second;
    }
    
    // early termination if impossible
    if (floatingLeft > dashesLeft) {
        return;
    }
    
    // try floating letters first (more constrained)
    for (const auto& pair : remainingFloating) {
        if (pair.second > 0) {
            char c = pair.first;
            currentWord[position] = c;
            remainingFloating[c]--;
            findWords(pattern, position + 1, currentWord, dict, results, remainingFloating);
            remainingFloating[c]++; // backtrack
        }
    }
    
    // then try non-floating letters if we have room
    if (floatingLeft < dashesLeft) {
        for (char c = 'a'; c <= 'z'; c++) {
            if (remainingFloating.count(c) == 0 || remainingFloating[c] == 0) {
                currentWord[position] = c;
                findWords(pattern, position + 1, currentWord, dict, results, remainingFloating);
            }
        }
    }
}