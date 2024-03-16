# README

**Name:** Justin Hoang  
**Student Id:** 110069730  
**Class:** CSCI 3453  
**Lab:** 2  
**Due Date:** March 25, 2024  

## Description of Program
A C++ program that simulates a CPU scheduler, which selects a process from a ready queue and exectues the process by a given scheduling algorithm.  
The simulation displays activities and evalates performance such as average waiting time, turnaround time, and response time. This program includes  
three different scheduling algorithms, FCFS (First Come First Served), SRTF (Shortest Response Time First), and RR (Round Robin) implemented with  
a time quantum. PCB (Process Control Blocks) have been manually created, as well as a ready-queue, which dynamically resizes if needed. 

## Program Status
Program is mostly working.  

Performance for SRTF and RR is being slightly miscalculated.

## Source Files
main.cpp  
Scheduler.cpp  
Scheduler.h  
PCB.cpp  
PCB.h  
Queue.cpp  
Queue.h  
Readfile.cpp  
Readfile.h  

## How to build and run program
1. Unzip the package
2. Enter the command `make` in the Terminal or CSE Grid
3. Run the executable `./myScheduler` by entering the command `./myScheduler <input_file> <scheduler_type> <time_quantum>`
4. Note that the `time_quantum` argument is only needed if using Round Robin scheduling
5. Remove all output files by entering the command `rm ./myScheduler main.o PCB.o Scheduler.o Readfile.o`