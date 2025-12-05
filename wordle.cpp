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
    
    // OK so this is a dash position - I need to try letters here
    // First, let me figure out how many empty spots I have left
    int dashesLeft = 0;
    for (int i = position; i < (int)pattern.length(); i++) {
        if (pattern[i] == '-') {
            dashesLeft++;
        }
    }
    
    // And count how many floating letters I still need to place
    int floatingLeft = 0;
    for (const auto& pair : remainingFloating) {
        floatingLeft += pair.second;
    }
    
    // If I have more floating letters than empty spots, this path is impossible
    if (floatingLeft > dashesLeft) {
        return;
    }
    
    // Now I'll try each letter from a to z in this position
    for (char c = 'a'; c <= 'z'; c++) {
        currentWord[position] = c;
        
        // Check if this letter is one of my required floating letters
        if (remainingFloating.count(c) && remainingFloating[c] > 0) {
            // Perfect! I can use one of my floating letters here
            remainingFloating[c]--;
            findWords(pattern, position + 1, currentWord, dict, results, remainingFloating);
            remainingFloating[c]++; // backtrack - put it back
        } else {
            // This isn't a floating letter, but maybe I can still use it
            // I just need to make sure I have enough spots left for my floating letters
            int spotsAfterThis = dashesLeft - 1;
            if (floatingLeft <= spotsAfterThis) {
                findWords(pattern, position + 1, currentWord, dict, results, remainingFloating);
            }
        }
    }
}