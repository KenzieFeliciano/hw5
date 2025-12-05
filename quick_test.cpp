#include <iostream>
#include <chrono>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;
using namespace std::chrono;

int main() {
    // Load dictionary
    set<string> dict;
    readDictWords("dict-eng.txt", dict);
    cout << "Dictionary loaded with " << dict.size() << " words." << endl;
    
    // Test 1: Empty pattern (should be fast)
    cout << "Test 1: Empty pattern..." << endl;
    auto start = high_resolution_clock::now();
    set<string> result1 = wordle("", "", dict);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Result: " << result1.size() << " words in " << duration.count() << " ms" << endl;
    
    // Test 2: Simple 5-letter pattern with no floating letters
    cout << "\nTest 2: Simple 5-letter pattern '-----'..." << endl;
    start = high_resolution_clock::now();
    set<string> result2 = wordle("-----", "", dict);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Result: " << result2.size() << " words in " << duration.count() << " ms" << endl;
    
    // Test 3: Pattern with one fixed letter
    cout << "\nTest 3: Pattern 'a----'..." << endl;
    start = high_resolution_clock::now();
    set<string> result3 = wordle("a----", "", dict);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Result: " << result3.size() << " words in " << duration.count() << " ms" << endl;
    
    // Test 4: Pattern with floating letters (the problematic case)
    cout << "\nTest 4: Pattern '-----' with floating letter 'e'..." << endl;
    start = high_resolution_clock::now();
    set<string> result4 = wordle("-----", "e", dict);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Result: " << result4.size() << " words in " << duration.count() << " ms" << endl;
    
    return 0;
}
