#include <iostream>
#include <set>
#include <vector>
#include "wordle.h"
#include "schedwork.h"
#include "dict-eng.h"

using namespace std;

void testWordle() {
    cout << "\n=== Testing Wordle ===" << endl;
    
    // Load dictionary
    set<string> dict = readDictWords("dict-eng.txt");
    cout << "Dictionary loaded with " << dict.size() << " words" << endl;
    
    // Test case 1: -i-- with dn
    set<string> result1 = wordle("-i--", "dn", dict);
    cout << "Test 1 (-i-- with dn): Found " << result1.size() << " words" << endl;
    cout << "First few results: ";
    int count = 0;
    for(const string& word : result1) {
        if(count++ >= 5) break;
        cout << word << " ";
    }
    cout << endl;
    
    // Test case 2: simple case
    set<string> result2 = wordle("----", "a", dict);
    cout << "Test 2 (---- with a): Found " << result2.size() << " words" << endl;
    
    // Test case 3: fixed letters only
    set<string> result3 = wordle("cat", "", dict);
    cout << "Test 3 (cat with no floating): Found " << result3.size() << " words" << endl;
    if(result3.size() > 0) {
        cout << "Result: " << *(result3.begin()) << endl;
    }
}

void testSchedule() {
    cout << "\n=== Testing Schedule ===" << endl;
    
    // Test case 1: Simple 4x4 case from instructions
    AvailabilityMatrix avail = {
        {true, true, true, true},   // Day 0: all available
        {true, false, true, false}, // Day 1: 0,2 available  
        {true, true, false, true},  // Day 2: 0,1,3 available
        {true, false, false, true}  // Day 3: 0,3 available
    };
    
    DailySchedule sched;
    bool result = schedule(avail, 2, 2, sched);
    
    if(result) {
        cout << "Test 1 (4x4 example): SUCCESS" << endl;
        for(int day = 0; day < sched.size(); day++) {
            cout << "Day " << day << ": ";
            for(int worker : sched[day]) {
                cout << worker << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Test 1 (4x4 example): FAILED - no solution found" << endl;
    }
    
    // Test case 2: Impossible case
    AvailabilityMatrix avail2 = {
        {true, false},   // Day 0: only worker 0 available
        {false, true}    // Day 1: only worker 1 available
    };
    
    DailySchedule sched2;
    bool result2 = schedule(avail2, 2, 1, sched2); // Need 2 workers per day, max 1 shift each
    
    if(!result2) {
        cout << "Test 2 (impossible case): SUCCESS - correctly identified no solution" << endl;
    } else {
        cout << "Test 2 (impossible case): FAILED - found solution when none should exist" << endl;
    }
}

int main() {
    cout << "Running simple tests for HW5..." << endl;
    
    testWordle();
    testSchedule();
    
    cout << "\nTests completed!" << endl;
    return 0;
}
