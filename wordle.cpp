#ifndef RECCHECK
#include <iostream>
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper function to recursively build words
void findWords(const string& pattern, const string& floating, int position, 
               string currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& floatingCount);

std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    set<string> results;
    
    if (in.empty()) {
        return results;
    }
    
    // Count how many of each floating letter we need
    map<char, int> floatingCount;
    for (int i = 0; i < (int)floating.size(); i++) {
        floatingCount[floating[i]]++;
    }
    
    // Start building words from the beginning
    string currentWord = "";
    findWords(in, floating, 0, currentWord, dict, results, floatingCount);
    
    return results;
}

void findWords(const string& pattern, const string& floating, int position, 
               string currentWord, const set<string>& dict, set<string>& results,
               map<char, int>& floatingCount)
{
    // If we've filled all positions, check if this is a valid word
    if (position == (int)pattern.length()) {
        // First check if the word exists in our dictionary
        if (dict.find(currentWord) != dict.end()) {
            // Count letters that appear in dash positions
            map<char, int> dashLetters;
            for (int i = 0; i < (int)currentWord.length(); i++) {
                if (pattern[i] == '-') {
                    dashLetters[currentWord[i]]++;
                }
            }
            
            // Check if we have all required floating letters
            map<char, int> requiredLetters;
            for (int i = 0; i < (int)floating.length(); i++) {
                requiredLetters[floating[i]]++;
            }
            
            bool hasAllFloating = true;
            for (map<char, int>::iterator it = requiredLetters.begin(); 
                 it != requiredLetters.end(); it++) {
                if (dashLetters[it->first] < it->second) {
                    hasAllFloating = false;
                    break;
                }
            }
            
            if (hasAllFloating) {
                results.insert(currentWord);
            }
        }
        return;
    }
    
    // If this position has a fixed letter, use it
    if (pattern[position] != '-') {
        currentWord += pattern[position];
        findWords(pattern, floating, position + 1, currentWord, dict, results, floatingCount);
        return;
    }
    
    // Check if we can still place all remaining floating letters
    int dashesLeft = 0;
    for (int i = position; i < (int)pattern.length(); i++) {
        if (pattern[i] == '-') {
            dashesLeft++;
        }
    }
    
    int floatingLeft = 0;
    for (map<char, int>::iterator it = floatingCount.begin(); 
         it != floatingCount.end(); it++) {
        if (it->second > 0) {
            floatingLeft += it->second;
        }
    }
    
    if (floatingLeft > dashesLeft) {
        return; // Not enough spaces for remaining floating letters
    }
    
    // Try each letter for this dash position
    for (char letter = 'a'; letter <= 'z'; letter++) {
        currentWord += letter;
        
        // Update floating letter count if we used one
        map<char, int> newFloatingCount = floatingCount;
        if (newFloatingCount.count(letter) && newFloatingCount[letter] > 0) {
            newFloatingCount[letter]--;
        }
        
        findWords(pattern, floating, position + 1, currentWord, dict, results, newFloatingCount);
        
        // Remove the letter we just tried (backtrack)
        currentWord.pop_back();
    }
}