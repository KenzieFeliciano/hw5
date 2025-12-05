#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
// static const Worker_T INVALID_ID = (unsigned int)-1;


// helper function to try scheduling workers
bool trySchedule(int day, int slot, const AvailabilityMatrix& avail, 
                 const size_t dailyNeed, const size_t maxShifts, 
                 DailySchedule& sched, vector<int>& workerShifts,
                 vector<set<Worker_T>>& dayWorkers);

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    int numDays = avail.size();
    int numWorkers = avail[0].size();
    
    // set up the schedule matrix - each day needs dailyNeed workers
    for(int i = 0; i < numDays; i++){
        vector<Worker_T> daySchedule;
        sched.push_back(daySchedule);
    }
    
    // keep track of how many shifts each worker has
    vector<int> workerShifts(numWorkers, 0);
    
    // track scheduled workers per day with sets for O(log n) lookup
    vector<set<Worker_T>> dayWorkers(numDays);
    
    // start trying to fill the schedule from day 0, slot 0
    return trySchedule(0, 0, avail, dailyNeed, maxShifts, sched, workerShifts, dayWorkers);
}

// this function tries to fill one slot at a time using backtracking
bool trySchedule(int day, int slot, const AvailabilityMatrix& avail, 
                 const size_t dailyNeed, const size_t maxShifts, 
                 DailySchedule& sched, vector<int>& workerShifts,
                 vector<set<Worker_T>>& dayWorkers)
{
    int numDays = avail.size();
    int numWorkers = avail[0].size();
    
    // if we filled all days successfully, we found a solution
    if(day == numDays) {
        return true;
    }
    
    // if we filled all slots for this day, move to next day
    if(slot == (int)dailyNeed) {
        return trySchedule(day + 1, 0, avail, dailyNeed, maxShifts, sched, workerShifts, dayWorkers);
    }
    
    // aggressive pruning: check available workers for this slot
    int available_count = 0;
    for(int w = 0; w < numWorkers; w++) {
        if(avail[day][w] && workerShifts[w] < (int)maxShifts && 
           dayWorkers[day].count(w) == 0) {  // O(log n) instead of O(n)
            available_count++;
        }
    }
    if(available_count < ((int)dailyNeed - slot)) {
        return false; // not enough workers for remaining slots
    }
    
    // future days pruning: check if we can satisfy all remaining days
    for(int future_day = day + 1; future_day < numDays; future_day++) {
        int future_available = 0;
        for(int w = 0; w < numWorkers; w++) {
            if(avail[future_day][w] && workerShifts[w] < (int)maxShifts) {
                future_available++;
            }
        }
        if(future_available < (int)dailyNeed) {
            return false; // impossible to fill future day
        }
    }
    
    // try each worker for this slot - simple order
    for(int worker = 0; worker < numWorkers; worker++) {
        // check constraints using O(log n) set lookup
        if(!avail[day][worker] || workerShifts[worker] >= (int)maxShifts || 
           dayWorkers[day].count(worker) > 0) {
            continue;
        }
        
        // try scheduling this worker
        sched[day].push_back(worker);
        dayWorkers[day].insert(worker);  // O(log n) insert
        workerShifts[worker]++;
        
        // recursively try to fill the rest
        if(trySchedule(day, slot + 1, avail, dailyNeed, maxShifts, sched, workerShifts, dayWorkers)) {
            return true; // found a solution
        }
        
        // backtrack
        sched[day].pop_back();
        dayWorkers[day].erase(worker);  // O(log n) erase
        workerShifts[worker]--;
    }
    
    return false;
}

