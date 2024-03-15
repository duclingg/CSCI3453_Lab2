#include "PCB.h"

PCB::PCB() {
    PID = 0;
    ARRIVAL_TIME = 0;
    CPU_BURST = 0;
    og_CPU_burst = 0;
    time_of_completion = 0;
    waiting_time = 0;
    turnaround_time = 0;
    response_time = 0;
    context_switch_num = 0;
    waiting = false;
    finished = false;
    preempted = false;
}

PCB::PCB(int* p_data) {
    PID = p_data[0];
	ARRIVAL_TIME = p_data[1];
	CPU_BURST = p_data[2];
	og_CPU_burst = CPU_BURST;
	time_of_completion = 0;
	waiting_time = 0;
	turnaround_time = 0;
	response_time = 0;
	context_switch_num = 0;
	waiting = false;
	finished = false;
	preempted = false;
}

int PCB::getPID() {
    return PID;
}

int PCB::getArrivalTime() {
    return ARRIVAL_TIME;
}

int PCB::getCPUBurst() {
    return CPU_BURST;
}

void PCB::setCPUBurst(int burst) {
    CPU_BURST = burst;
}

void PCB::progress() {
    --CPU_BURST;
}