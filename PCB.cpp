#include "PCB.h"

using namespace std;

void PCB::incrementWaiting(float inc) {
    waiting += inc;
}

void PCB::incrementRunning(float inc) {
    timeRemaining -= inc;
}

void PCB::done(float time) {
    finish = time;
    turnaround = finish - arrival;
}

string PCB::results() {
    ostringstream output;
    output << setw(4) << pid << setw(8) << arrival << setw(9) << CPUBurst << setw(7) << finish 
    << setw(8) << waiting << setw(11) << turnaround << setw(9) << response << setw(9) << context;
    return output.str();
}