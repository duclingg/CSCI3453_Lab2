#include "scheduler.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Scheduler::advance() {
    checkUnscheduled();
    
    switch (algorithm) {
        case 0:
            fcfs();
            break;
        case 1:
            srtf();
            break;
        case 2:
            rrobin();
            break;
    }
}

// fcfs (first come first serve) scheduler
void Scheduler::fcfs() {
    if (running[0].getRemaining() == 0) {
        runningToTerminated();

        if (ready.size() > 0) {
            readyToRun();
        }
    }

    if (running.size() > 0) {
        running[0].incrementRunning(SIMULATION_INCREMENT);
    }

    for (int i = 0; i < ready.size(); i++) {
        ready[i].incrementWaiting(SIMULATION_INCREMENT);
    }
}

// srtf (shortest response time first) scheduler
void Scheduler::srtf() {
    if (running[0].getRemaining() == 0) {
        runningToTerminated();

        if (ready.size() > 0) {
            srtfSwapTuple shortest = srtfSwapTest(-1, SIMULATION_NUMERICAL_BOUND);
            ready.moveToFront(shortest.getIndex());
            readyToRun();
        }
    } else {
        srtfSwapTuple shortest = srtfSwapTest(running[0].getPID(), running[0].getRemaining());
        if (shortest.getPID() != running[0].getPID()) {
            ready.moveToFront(shortest.getIndex());
            contextSwitch();
        }
    }

    if (running.size() > 0) {
        running[0].incrementRunning(SIMULATION_INCREMENT);
    }

    for (int i = 0; i < ready.size(); i++) {
        ready[i].incrementWaiting(SIMULATION_INCREMENT);
    }
}

// rr (round robin) scheduler
void Scheduler::rrobin() {
    if (running.size() > 0 && running[0].getRemaining() == 0) {
        runningToTerminated();
    }

    if (simulationTime != 0 && thisQuantum == 0) {
        if (ready.size() > 0 && running.size() > 0) {
            contextSwitch();
        } else if (ready.size() > 0) {
            readyToRun();
        }
    }

    if (running.size() > 0) {
        running[0].incrementRunning(SIMULATION_INCREMENT);
    }

    for (int i = 0; i < ready.size(); i++) {
        ready[i].incrementWaiting(SIMULATION_INCREMENT);
    }

    advanceThisQuantum();
}

// check if process is not scheduled
void Scheduler::checkUnscheduled() {
    for (int i = 0; i < unscheduled.size(); i++) {
        if (unscheduled[i].getArrival() == simulationTime) {
            unscheduled.moveToBack(i);
            ready.push_back(unscheduled.back());
            unscheduled.pop_back();

            if (running.size() == 0) {
                readyToRun();
            }
        }
    }
}

// run process until termination
void Scheduler::runningToTerminated() {
    running[0].done(simulationTime + contextTime);
    terminated.push_back(running[0]);
    running.pop_back();
}

// process is ready to run
void Scheduler::readyToRun() {
    if (ready.front().hasResponseOccured() == false) {
        ready.front().responseHasOccured();
        float diff = simulationTime - ready.front().getArrival() + contextTime;
        ready.front().setResponse(diff);
    }

    running.push_back(ready.front());
    ready.pop_front();
}

Scheduler::srtfSwapTuple Scheduler::srtfSwapTest(int shortestPID, float shortestRemaining) {
    int readyIndex = -1;
    
    for (int i = 0; i < ready.size(); i++) {
        if (ready[i].getRemaining() < shortestRemaining) {
            shortestPID = ready[i].getPID();
            readyIndex = i;
        }
    }

    return srtfSwapTuple(shortestPID, readyIndex);
}

void Scheduler::loadUnscheduledPCBs(Queue<PCB>& simulation) {
    for (int i = 0; i < simulation.size(); i++) {
        unscheduled.push_back(simulation[i]);
    }
}

void Scheduler::advanceThisQuantum() {
    thisQuantum = (thisQuantum + 1) % quantum;
}

void Scheduler::setAlgorithm(int a) {
    algorithm = a;
}

void Scheduler::setQuantum(int q) {
    quantum = q;
}

void Scheduler::run() {
    thisQuantum = 0;

    while (hasProcesses()) {
        advance();
        simulationTime++;
    }
}

bool Scheduler::hasProcesses() {
    return ((unscheduled.size() > 0) || (ready.size() > 0) || (running.size() > 0));
}

void Scheduler::contextSwitch() {
    if (ready.front().hasResponseOccured() == false) {
        ready.front().responseHasOccured();
        float diff = simulationTime - ready.front().getArrival() + contextTime;
        ready.front().setResponse(diff);
    }

    running[0].incrementContext();
    ready.push_back(running[0]);
    running.pop_back();

    for (int i = 0; i < ready.size(); i++) {
        ready[i].incrementWaiting(CONTEXT_SWITCH_TIME);
    }

    contextTime += CONTEXT_SWITCH_TIME;
    running.push_front(ready.front());
    ready.pop_front();
}

void Scheduler::sortAndAggregate() {
    float sumBurst = 0, sumWait = 0, sumTurn = 0, sumResp = 0;

    while (terminated.size() > 0) {
        int lowPIDIndex = SIMULATION_NUMERICAL_BOUND;
        int lowPID = SIMULATION_NUMERICAL_BOUND;
        
        for (int i = 0; i < terminated.size(); i++) {
            if (terminated[i].getPID() < lowPID) {
                lowPIDIndex = i;
                lowPID = terminated[i].getPID();
            }
        }

        terminated.moveToBack(lowPIDIndex);
        results.push_back(terminated.back());
        terminated.pop_back();

        sumBurst += results.back().getCPUBurst();
        sumWait += results.back().getWaiting();
        sumTurn += results.back().getTurnaround();
        sumResp += results.back().getResponse();
    }

    avgBurst = sumBurst / results.size();
    avgWait = sumWait / results.size();
    avgTurn = sumTurn / results.size();
    avgResp = sumResp / results.size();
}

void Scheduler::printResults() {
    sortAndAggregate();
    string header;

    switch (algorithm) {
        case 0:
            header = "First Come, First Serve (FCFS)";
            break;
        case 1:
            header = "Shortest Run Time First (SRTF)";
            break;
        case 2:
            header = "Round Robin (RR)";
            break;
    }

    cout << "*****************************************************************" << endl
    << "           Scheduling Algorithm: " << header << endl;

    if (algorithm==2) {
        cout << "             No. of tasks: " << results.size()
                  << ", Quantum: " << quantum << endl;
    }

    cout << "*****************************************************************\n" << endl;
    cout << setw(4) << "pid" << setw(8) << "arrival" << setw(9) << "CPUburst" << setw(7) << "finish"
    << setw(8) << "waiting" << setw(11) << "turnaround" << setw(9) << "response" << setw(9) << "context" << endl;
    
    for (int i = 0; i < results.size(); ++i) {
        cout << results[i].results();
        cout << endl;
    }

    cout << "Average CPU burst time = " << avgBurst << ", Average waiting time = " << avgWait << endl << "Average turn around time = " << avgTurn
    << ", Average response time = " << avgResp << endl << "Total No. of Context Switches Performed = " << sumContext << endl;
    
}