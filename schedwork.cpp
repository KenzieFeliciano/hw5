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

// helper function to try scheduling workers recursively
bool fillSchedule(int day, int slot, const AvailabilityMatrix& avail, 
                  const size_t dailyNeed, const size_t maxShifts, 
                  DailySchedule& sched, vector<int>& worker_shift_count);

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
    
    int days = avail.size();
    int workers = avail[0].size();
    
    // setup the schedule - each day needs a vector for worker IDs (loop 1)
    for(int d = 0; d < days; d++){
        vector<Worker_T> day_workers;
        sched.push_back(day_workers);
    }
    
    // keep track of how many shifts each worker has been assigned (loop 2)
    vector<int> worker_shift_count(workers, 0);
    
    // start trying to fill from day 0, slot 0
    return fillSchedule(0, 0, avail, dailyNeed, maxShifts, sched, worker_shift_count);
}
// recursive backtracking function to fill one slot at a time
bool fillSchedule(int day, int slot, const AvailabilityMatrix& avail, 
                  const size_t dailyNeed, const size_t maxShifts, 
                  DailySchedule& sched, vector<int>& worker_shift_count)
{
    int days = avail.size();
    int workers = avail[0].size();
    
    // base case: successfully filled all days
    if(day == days) {
        return true;
    }
    
    // if we finished this day, move to next day
    if(slot == (int)dailyNeed) {
        return fillSchedule(day + 1, 0, avail, dailyNeed, maxShifts, sched, worker_shift_count);
    }
    
    // try each worker for this slot (loop 3)
    for(int w = 0; w < workers; w++) {
        // check if this worker can work this slot
        if(!avail[day][w]) continue; // not available
        if(worker_shift_count[w] >= (int)maxShifts) continue; // too many shifts
        
        // check if worker already scheduled this day
        bool already_working = false;
        for(int i = 0; i < (int)sched[day].size(); i++) {
            if(sched[day][i] == w) {
                already_working = true;
                break;
            }
        }
        if(already_working) continue;
        
        // try assigning this worker
        sched[day].push_back(w);
        worker_shift_count[w]++;
        
        // recursive call to fill next slot
        if(fillSchedule(day, slot + 1, avail, dailyNeed, maxShifts, sched, worker_shift_count)) {
            return true; // found solution
        }
        
        // backtrack if no solution found
        sched[day].pop_back();
        worker_shift_count[w]--;
    }
    
    return false; // no valid assignment found
}

