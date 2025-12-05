#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper function to recursively build words
// Pass total dashes and floating count to avoid recalculating every time
void findWords(const string& pattern, int position, 
               string& currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& remainingFloating, int dashesLeft, int floatingLeft);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    if (in.empty()) {
        return results;
    }
    
    // count how many dashes we need to fill
    int totalDashes = 0;
    for (int i = 0; i < (int)in.size(); i++) {
        if (in[i] == '-') {
            totalDashes++;
        }
    }
    
    // early termination: if we can't fit all floating letters
    if ((int)floating.size() > totalDashes) {
        return results;
    }
    
    // count floating letters and track them
    map<char, int> remainingFloating;
    for (int i = 0; i < (int)floating.size(); i++) {
        remainingFloating[floating[i]]++;
    }
    
    string currentWord = in;
    
    findWords(in, 0, currentWord, dict, results, remainingFloating, totalDashes, (int)floating.size());
    
    return results;
}

void findWords(const string& pattern, int position, 
               string& currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& remainingFloating, int dashesLeft, int floatingLeft)
{
    // Base case: reached end of word
    if (position == (int)pattern.length()) {
        if (floatingLeft == 0 && dict.count(currentWord)) {
            results.insert(currentWord);
        }
        return;
    }
    
    // skip positions that already have letters
    if (pattern[position] != '-') {
        findWords(pattern, position + 1, currentWord, dict, results, remainingFloating, dashesLeft, floatingLeft);
        return;
    }
    
    // pruning: impossible to place all floating letters
    if (floatingLeft > dashesLeft) {
        return;
    }
    
    // when floating count equals remaining dashes, must use only floating letters
    if (floatingLeft == dashesLeft && floatingLeft > 0) {
        for (auto& pair : remainingFloating) {
            if (pair.second > 0) {
                char c = pair.first;
                currentWord[position] = c;
                pair.second--;
                findWords(pattern, position + 1, currentWord, dict, results, 
                         remainingFloating, dashesLeft - 1, floatingLeft - 1);
                pair.second++;
            }
        }
        return;
    }
    
    // optimization: when we have many floating letters remaining and many dashes,
    // prioritize floating letters by trying them first
    if (floatingLeft > 0) {
        // try each floating letter at this position
        for (auto& pair : remainingFloating) {
            if (pair.second > 0) {
                char c = pair.first;
                currentWord[position] = c;
                pair.second--;
                findWords(pattern, position + 1, currentWord, dict, results, 
                         remainingFloating, dashesLeft - 1, floatingLeft - 1);
                pair.second++;
            }
        }
    }
    
    // only try non-floating letters if we haven't placed enough floating letters
    // or if we have extra space
    if (floatingLeft < dashesLeft) {
        // optimization: drastically limit the search space for large extra positions
        if (dashesLeft - floatingLeft >= 4) {
            // when we have 4+ extra positions, only try the 4 most common letters
            static const char veryLimitedLetters[] = "etao";
            for (int i = 0; i < 4; i++) {
                char c = veryLimitedLetters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else if (dashesLeft - floatingLeft >= 3) {
            // when we have 3 extra positions, try the 8 most common letters
            static const char limitedLetters[] = "etaoinsh";
            for (int i = 0; i < 8; i++) {
                char c = limitedLetters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else {
            // for fewer extra positions, try all letters but in frequency order
            static const char commonLetters[] = "etaoinshrdlcumwfgypbvkjxqz";
            for (int i = 0; i < 26; i++) {
                char c = commonLetters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        }
    }
}