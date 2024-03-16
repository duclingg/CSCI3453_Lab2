#ifndef PCB_H
#define PCB_H

#include <string>
#include <iomanip>
#include <sstream>

class PCB {
private:
    int pid;
    float arrival;
    float CPUBurst;

    float finish;
    float timeRemaining;
    float waiting;
    float turnaround;
    float response;
    bool responseOccured;
    int context;

public:
    PCB(){};
    PCB(int p, float a, float c): pid(p), arrival(a), CPUBurst(c) {
        finish = 0.0;
        timeRemaining = CPUBurst;
        waiting = 0.0;
        turnaround = 0.0;
        response = 0.0;
        context = 0;
        responseOccured = false;
    };

    PCB(PCB& copy) {
        pid = copy.getPID();
        arrival = copy.getArrival();
        CPUBurst = copy.getCPUBurst();
        finish = copy.getFinish();
        timeRemaining = copy.getRemaining();
        waiting = copy.getWaiting();
        turnaround = copy.getTurnaround();
        response = copy.getResponse();
        context = copy.getContext();
        responseOccured = copy.hasResponseOccured();
    }

    // getters for basic record data
    int getPID() { return pid; }
    float getArrival() { return arrival; }
    float getCPUBurst() { return CPUBurst; }
    float getFinish() { return finish; }
    float getRemaining() { return timeRemaining; }
    float getResponse() { return response; }
    bool hasResponseOccured() { return responseOccured; }
    float getWaiting() { return waiting; }
    float getTurnaround() { return turnaround; }
    int getContext() { return context; }
    
    // data recording functions
    void setArrival(float a) { arrival = a; }
    void setResponse(float r) { response = r; }
    void responseHasOccured() { responseOccured = true; }
    void incrementContext() { context++; };
    void incrementWaiting(float);
    void incrementRunning(float);
    void done(float);
    
    // output
    std::string results();
};

#endif