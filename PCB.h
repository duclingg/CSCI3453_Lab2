#ifndef PCB_H
#define PCH_H

class PCB {
    private:
        int PID;
        int ARRIVAL_TIME;
        int CPU_BURST;

    public:
        PCB();
        PCB(int*);

        int getPID();
        int getArrivalTime();
        int getCPUBurst();
        void setCPUBurst(int);
        void progress();

        int context_switch_num;
        int og_CPU_burst;
        bool waiting;
        bool finished;
        bool preempted;
        float completion_time;
        float waiting_time;
        float turnaround_time;
        float response_time;
};

#endif