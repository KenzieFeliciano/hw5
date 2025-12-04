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
    
    cout << "Dictionary loaded with " << dict.size() << " words" << endl;
    
    // Test case 1: Empty floating (should be fast)
    cout << "\nTest 1: Empty floating letters" << endl;
    auto start = high_resolution_clock::now();
    set<string> result1 = wordle("-----", "", dict);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms, Results: " << result1.size() << endl;
    
    // Test case 2: Simple pattern with floating
    cout << "\nTest 2: Simple pattern with floating" << endl;
    start = high_resolution_clock::now();
    set<string> result2 = wordle("a----", "e", dict);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms, Results: " << result2.size() << endl;
    
    // Test case 3: More complex
    cout << "\nTest 3: More complex" << endl;
    start = high_resolution_clock::now();
    set<string> result3 = wordle("--a--", "er", dict);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms, Results: " << result3.size() << endl;
    
    return 0;
}
