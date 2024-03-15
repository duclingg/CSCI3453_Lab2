#ifndef QUEUE_H
#define QUEUE_h

#include "PCB.h"

class Queue {
    private:
        PCB* resize_arr(int, int);
        PCB* queue;
        int size;

    public:
        Queue();
        ~Queue();

        int length();
        bool empty();
        void sort();
        void push(PCB&);
        PCB& pop(PCB*, int);
        PCB& remove(int, PCB*, int);
        PCB* get(int);
};

#endif