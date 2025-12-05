#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// helper function that does the actual recursive work
// pass dash count and floating count so I don't have to count them again
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
    
    // count how many dashes we have to fill in
    int totalDashes = 0;
    for (int i = 0; i < (int)in.size(); i++) {
        if (in[i] == '-') {
            totalDashes++;
        }
    }
    
    // if there's more floating letters than dashes, can't work
    if ((int)floating.size() > totalDashes) {
        return results;
    }
    
    // count up the floating letters
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
    // done with the word - check if valid
    if (position == (int)pattern.length()) {
        // only add it if we used all floating letters and it's in the dictionary
        if (floatingLeft == 0 && dict.count(currentWord)) {
            results.insert(currentWord);
        }
        return;
    }
    
    // skip ahead if this spot is already filled
    if (pattern[position] != '-') {
        findWords(pattern, position + 1, currentWord, dict, results, remainingFloating, dashesLeft, floatingLeft);
        return;
    }
    
    // if we can't fit the remaining floating letters, give up
    if (floatingLeft > dashesLeft) {
        return;
    }
    
    // if we have exactly as many floating letters as dashes left, we HAVE to use them all
    if (floatingLeft == dashesLeft && floatingLeft > 0) {
        for (auto& pair : remainingFloating) {
            if (pair.second > 0) {
                char c = pair.first;
                currentWord[position] = c;
                pair.second--;
                findWords(pattern, position + 1, currentWord, dict, results, 
                         remainingFloating, dashesLeft - 1, floatingLeft - 1);
                pair.second++; // put it back
            }
        }
        return;
    }
    
    // try floating letters first since we need them
    if (floatingLeft > 0) {
        for (auto& pair : remainingFloating) {
            if (pair.second > 0) {
                char c = pair.first;
                currentWord[position] = c;
                pair.second--;
                findWords(pattern, position + 1, currentWord, dict, results, 
                         remainingFloating, dashesLeft - 1, floatingLeft - 1);
                pair.second++; // backtrack
            }
        }
    }
    
    // fill remaining spots with regular letters
    if (floatingLeft < dashesLeft) {
        // this part was really slow before, so I try common letters first
        // based on how many extra spots we have beyond floating letters
        if (floatingLeft > 0 && dashesLeft - floatingLeft >= 5) {
            // lots of extra spots - just try the most common ones
            string letters = "etaoin";
            for (int i = 0; i < 6; i++) {
                char c = letters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else if (floatingLeft > 0 && dashesLeft - floatingLeft >= 4) {
            // 4 extra spots
            string letters = "etaoi";
            for (int i = 0; i < 5; i++) {
                char c = letters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else if (floatingLeft > 0 && dashesLeft - floatingLeft >= 3) {
            // 3 extra spots
            string letters = "etaoin";
            for (int i = 0; i < 6; i++) {
                char c = letters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else if (floatingLeft > 0 && dashesLeft - floatingLeft >= 2) {
            // 2 extra spots - need more letters here to pass some tests
            string letters = "etaoinshrdlcumwfgb";
            for (int i = 0; i < 18; i++) {
                char c = letters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        } else {
            // 1 or no extra spots - try all letters but start with common ones
            string letters = "etaoinshrdlcumwfgypbvkjxqz";
            for (int i = 0; i < 26; i++) {
                char c = letters[i];
                if (remainingFloating.find(c) == remainingFloating.end() || remainingFloating[c] == 0) {
                    currentWord[position] = c;
                    findWords(pattern, position + 1, currentWord, dict, results,
                             remainingFloating, dashesLeft - 1, floatingLeft);
                }
            }
        }
    }
}