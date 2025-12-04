// Simple test to analyze wordle performance
#include <iostream>
#include <chrono>
#include <set>
#include <string>

using namespace std;
using namespace std::chrono;

// Simple test of algorithm complexity
int main() {
    cout << "Testing algorithm complexity..." << endl;
    
    // Simulate 5-letter word with all positions open (26^5 combinations)
    long long total_combinations = 26 * 26 * 26 * 26 * 26; // 26^5
    cout << "For 5 dashes (-----), total combinations to check: " << total_combinations << endl;
    cout << "At 1M checks per second, this would take: " << (total_combinations / 1000000.0) << " seconds" << endl;
    
    // More reasonable approach - iterate through dictionary instead
    // Typical English dictionary has ~100K 5-letter words
    int dict_size = 100000;
    cout << "\nDictionary approach:" << endl;
    cout << "Dictionary size: " << dict_size << " words" << endl;
    cout << "Time to check all: ~" << (dict_size / 1000000.0) << " seconds" << endl;
    
    return 0;
}
