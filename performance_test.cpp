#include <iostream>
#include <chrono>
#include "wordle.h"
#include "dict-eng.h"
#include "schedwork.h"

using namespace std;
using namespace std::chrono;

void testWordlePerformance() {
    cout << "Testing Wordle performance..." << endl;
    
    // Load dictionary
    const set<string>& dict = readDictWords("dict-eng.txt");
    
    // Test case 1: Empty pattern with floating letters
    auto start = high_resolution_clock::now();
    set<string> result1 = wordle("-----", "abcde", dict);
    auto end = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(end - start);
    cout << "Test 1 (-----/abcde): " << result1.size() << " words, " 
         << duration1.count() << "ms" << endl;
    
    // Test case 2: Pattern with some fixed letters
    start = high_resolution_clock::now();
    set<string> result2 = wordle("-i--", "dn", dict);
    end = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(end - start);
    cout << "Test 2 (-i--/dn): " << result2.size() << " words, " 
         << duration2.count() << "ms" << endl;
    
    // Test case 3: No floating letters
    start = high_resolution_clock::now();
    set<string> result3 = wordle("----", "", dict);
    end = high_resolution_clock::now();
    auto duration3 = duration_cast<milliseconds>(end - start);
    cout << "Test 3 (----/empty): " << result3.size() << " words, " 
         << duration3.count() << "ms" << endl;
}

void testSchedulePerformance() {
    cout << "\nTesting Schedule performance..." << endl;
    
    // Test case: Simple 4x4 matrix
    AvailabilityMatrix avail = {
        {true, true, true, true},
        {true, false, true, false}, 
        {true, true, false, true},
        {true, false, false, true}
    };
    
    DailySchedule sched;
    
    auto start = high_resolution_clock::now();
    bool result = schedule(avail, 2, 2, sched);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    
    cout << "Schedule test: " << (result ? "Success" : "Failed") 
         << ", " << duration.count() << "ms" << endl;
}

int main() {
    testWordlePerformance();
    testSchedulePerformance();
    return 0;
}
