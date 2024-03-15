#include "Queue.h"
#include "PCB.h"
#include <cstring>

int HEAD = 0;
int TAIL = 1;
int INCREASE = 1;
int DECREASE = -1;

Queue::Queue() {
    queue = new PCB[0];
    size = 0;
}

Queue::~Queue() {
    delete[] queue;
}

// sort
void Queue::sort() {
    PCB temp;
    bool swapped = true;
    int index = 0;
    
    while (swapped) {
        swapped = false;
        index++;

        for (int i = 0; i < size - index; i++) {
            if (queue[i].getCPUBurst() > queue[i+1].getCPUBurst()) {
                temp = queue[i];
                queue[i] = queue[i+1];
                queue[i+1] = temp;
                swapped = true;
            }
        }
    }
}

// push into queue
void Queue::push(PCB& p) {
    queue = resize_arr(INCREASE, TAIL);
    queue[size-1] = p;
}

// pop from queue
PCB& Queue::pop(PCB* p, int num_processes) {
    PCB pcb = queue[0];
    
    for (int i = 0; i < num_processes; i++) {
        if (queue[0].getPID() == p[i].getPID()) {
            p[i].setCPUBurst(queue[0].getCPUBurst());
            p[i].finished = queue[0].finished;
            p[i].context_switch_num = queue[0].context_switch_num;
            p[i].response_time = queue[0].response_time;
            p[i].completion_time = queue[0].completion_time;
            p[i].turnaround_time = queue[0].turnaround_time;
            p[i].waiting = queue[0].waiting;
            p[i].waiting_time = queue[0].waiting_time;
            break;
        }
    }

    queue = resize_arr(DECREASE, HEAD);

    return pcb;
}

// remove from queue
PCB& Queue::remove(int pos, PCB* p, int num_processes) {
    PCB pcb = queue[pos];

    for (int i = 0; i < num_processes; i++) {
        if (queue[pos].getPID() == p[i].getPID()) {
            p[i].setCPUBurst(queue[pos].getCPUBurst());
			p[i].finished = queue[pos].finished;
			p[i].context_switch_num = queue[pos].context_switch_num;
			p[i].response_time = queue[pos].response_time;
			p[i].completion_time = queue[pos].completion_time;
			p[i].turnaround_time = queue[pos].turnaround_time;
			p[i].waiting = queue[pos].waiting;
			p[i].waiting_time = queue[pos].waiting_time;
			break;
        }
    }
}

PCB* Queue::resize_arr(int mode, int from) {
    size += mode;
    PCB* new_queue = new PCB[size];

    for (int i = 0; i < size; i++) {
        if ((mode == INCREASE && from == TAIL) || (mode == DECREASE && from == TAIL)) {
            new_queue[i] = queue[i];
        } else {
            new_queue[i] = queue[i+1];
        }
    }

    return new_queue;
}

// gets items in queue
PCB* Queue::get(int i) {
    return &queue[i];
}

// length of queue (num of elements)
int Queue::length() {
    return size;
}

bool Queue::empty() {
    bool empty = false;
    
    if (size == 0) {
        empty = true;
    }

    return empty;
}