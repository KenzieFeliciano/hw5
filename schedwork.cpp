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

// helper function for the recursive part
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
    
    // setup the schedule matrix - each day gets its own vector (loop 1)
    for(int d = 0; d < days; d++){
        vector<Worker_T> day_workers;
        sched.push_back(day_workers);
    }
    
    // track how many shifts each worker has (loop 2)
    vector<int> worker_shift_count(workers, 0);
    
    // start the recursive search from the beginning
    return fillSchedule(0, 0, avail, dailyNeed, maxShifts, sched, worker_shift_count);
}
// does the actual work - fills one slot at a time
bool fillSchedule(int day, int slot, const AvailabilityMatrix& avail, 
                  const size_t dailyNeed, const size_t maxShifts, 
                  DailySchedule& sched, vector<int>& worker_shift_count)
{
    int days = avail.size();
    int workers = avail[0].size();
    
    // finished all days
    if(day == days) {
        return true;
    }
    
    // finished this day, go to next
    if(slot == (int)dailyNeed) {
        return fillSchedule(day + 1, 0, avail, dailyNeed, maxShifts, sched, worker_shift_count);
    }
    
    // try each worker for this time slot (loop 3)
    for(int w = 0; w < workers; w++) {
        // check if this worker can work this day/slot
        if(!avail[day][w]) continue; // not available today
        if(worker_shift_count[w] >= (int)maxShifts) continue; // already at max shifts
        
        // make sure worker isn't already scheduled today
        bool already_working = false;
        for(int i = 0; i < (int)sched[day].size(); i++) {
            if(sched[day][i] == w) {
                already_working = true;
                break;
            }
        }
        if(already_working) continue;
        
        // try this worker
        sched[day].push_back(w);
        worker_shift_count[w]++;
        
        // see if we can fill the rest
        if(fillSchedule(day, slot + 1, avail, dailyNeed, maxShifts, sched, worker_shift_count)) {
            return true; // found a solution
        }
        
        // didn't work, undo
        sched[day].pop_back();
        worker_shift_count[w]--;
    }
    
    return false; // couldn't find anyone for this slot
}

