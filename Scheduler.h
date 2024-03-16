#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "queue.h"
#include "PCB.h"

class Scheduler {
private:
    float CONTEXT_SWITCH_TIME;
    float SIMULATION_INCREMENT;
    float SIMULATION_NUMERICAL_BOUND;
    Queue<PCB> unscheduled, ready, running, waiting, terminated, results;

    class srtfSwapTuple {
    private:
        int index;
        int pid;
    
    public:
        srtfSwapTuple(int p, int i): index(i), pid(p) {}
        int getIndex() { return index; }
        int getPID() { return pid; }
    };

    int algorithm;
    int quantum;
    int thisQuantum;
    float simulationTime;
    float contextTime;

    void advance(); 
    void fcfs();
    void srtf();
    void rrobin();

    void checkUnscheduled();
    void runningToTerminated();
    void readyToRun();
    srtfSwapTuple srtfSwapTest(int, float);
    void advanceThisQuantum();

    void sortAndAggregate();
    float avgBurst;
    float avgWait;
    float avgTurn;
    float avgResp;
    int sumContext;

public:
    void loadUnscheduledPCBs(Queue<PCB>&);
    Scheduler(int a, int q):
        algorithm(a),
        quantum(q),
        CONTEXT_SWITCH_TIME(0.5),
        SIMULATION_INCREMENT(1),
        SIMULATION_NUMERICAL_BOUND(999999) {
            simulationTime = 0;
            contextTime = 0;
            
            if (algorithm != 2) {
                quantum = 1;
            }

            thisQuantum = 0;
            sumContext = 0;
        };

    void setAlgorithm(int);
    void setQuantum(int);
    void run();
    bool hasProcesses();
    void contextSwitch();
    void printResults();
};

#endif