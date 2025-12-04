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
                 DailySchedule& sched, vector<int>& workerShifts);

// helper to check if worker is already scheduled for this day
bool workerAlreadyScheduled(int day, Worker_T worker, const DailySchedule& sched);

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
    
    // start trying to fill the schedule from day 0, slot 0
    return trySchedule(0, 0, avail, dailyNeed, maxShifts, sched, workerShifts);
}

// this function tries to fill one slot at a time using backtracking
bool trySchedule(int day, int slot, const AvailabilityMatrix& avail, 
                 const size_t dailyNeed, const size_t maxShifts, 
                 DailySchedule& sched, vector<int>& workerShifts)
{
    int numDays = avail.size();
    int numWorkers = avail[0].size();
    
    // if we filled all days successfully, we found a solution
    if(day == numDays) {
        return true;
    }
    
    // if we filled all slots for this day, move to next day
    if(slot == dailyNeed) {
        return trySchedule(day + 1, 0, avail, dailyNeed, maxShifts, sched, workerShifts);
    }
    
    // aggressive pruning: check if remaining workers can fill remaining slots
    int remaining_slots_today = dailyNeed - slot;
    int available_workers_today = 0;
    for(int w = 0; w < numWorkers; w++) {
        if(avail[day][w] && workerShifts[w] < maxShifts && 
           !workerAlreadyScheduled(day, w, sched)) {
            available_workers_today++;
        }
    }
    if(available_workers_today < remaining_slots_today) {
        return false; // impossible to fill remaining slots
    }
    
    // try workers in order of how many shifts they have (least first for better distribution)
    vector<pair<int, int>> worker_order;
    for(int worker = 0; worker < numWorkers; worker++) {
        if(avail[day][worker] && workerShifts[worker] < maxShifts && 
           !workerAlreadyScheduled(day, worker, sched)) {
            worker_order.push_back({workerShifts[worker], worker});
        }
    }
    sort(worker_order.begin(), worker_order.end());
    
    // try each available worker
    for(auto& pair : worker_order) {
        int worker = pair.second;
        
        // try scheduling this worker
        sched[day].push_back(worker);
        workerShifts[worker]++;
        
        // recursively try to fill the rest
        if(trySchedule(day, slot + 1, avail, dailyNeed, maxShifts, sched, workerShifts)) {
            return true; // found a solution
        }
        
        // backtrack - this didnt work so remove the worker
        sched[day].pop_back();
        workerShifts[worker]--;
    }
    
    // tried all workers and none worked
    return false;
}

// check if a worker is already scheduled to work on a specific day
bool workerAlreadyScheduled(int day, Worker_T worker, const DailySchedule& sched) {
    for(int i = 0; i < sched[day].size(); i++) {
        if(sched[day][i] == worker) {
            return true;
        }
    }
    return false;
}

