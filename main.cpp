#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "Queue.h"
#include "PCB.h"

using namespace std;

string PROCESS_FILE;

// parse data from txt file, used to construct PCB
int get_processes(PCB* processes) {
	int p = 0;
	int p_data[3];
	string current_line;
	ifstream infile(PROCESS_FILE);

	if (infile.good()) {
		while (getline(infile, current_line)) {
			int p_data_count = 0;

			for (int i = 0; i < current_line.size(); ++i) {
				if (current_line[i] != ' ') {
					string num = "";
					while (current_line[i] != ' ' && current_line[i] != '\0') {
						num += current_line[i];
						i++;
					}

					if (p_data_count < 3) {
						p_data[p_data_count] = stoi(num);
						p_data_count++;
					}
				}
			}

			PCB pcb(p_data);

			processes[p] = pcb;
			p++;
		}
	}
	else {
		infile.close();
		return 1;
	}

	infile.close();
	return 0;
}

// prints result of scheduling algorithm
void print_results(PCB* p, int num_processes, int mode, int quantum) {
    string algo = "";
    int num_astr = 21;

    if (mode == 0) {
        algo = "FCFS";
    } else if (mode == 1) {
        algo = "SRTF";
    } else {
        algo = "RR";
        num_astr = 18;
    }

    cout << string(71, '*') << endl << string(num_astr, '*') << " Scheduling Algorithm: " << algo;

    if (mode == 2) {
        num_astr--;
    }

    cout << string(num_astr, '*') << endl;

    if (mode == 2) {
        num_astr++;
        cout << string(num_astr, '*') << "     (No. of Task = " << num_processes << " Quantum = " << quantum << ")     " << string(num_astr--, '*') << endl;
    }

    cout << string(71, '*') << endl << endl << string(106, '-') << endl;

    cout << "| PID " << setw(9) << "| Arrival " << setw(11) << "| CPU Burst " << setw(8) << "| Finish " << setw(15) << "| Waiting Time " << setw(12) << "| Turnaround " 
    << setw(15) << "| Response Time" << setw(24) << "| No. of Context Switch |";

    cout << endl << string(106, '-') << endl;

    float CPU_BURST_sum = 0;
    float waiting_time_sum = 0;
    float turnaround_time_sum = 0;
    float response_time_sum = 0;
    int context_switch_sum = 0;

    for (int i = 0; i < num_processes; i++) {
        cout << fixed << setprecision(1) << "| " << setw(4)  << left << p[i].getPID() << "| " << setw(8)  << left << p[i].getArrivalTime() << "| " 
        << setw(10) << left << p[i].og_CPU_burst << "| " << setw(7)  << left << p[i].completion_time << "| " << setw(13) << left << p[i].waiting_time << "| " 
        << setw(11) << left << p[i].turnaround_time << "| " << setw(14) << left << p[i].response_time << "| " << setw(21) << left << p[i].context_switch_num << " |" << endl;

        CPU_BURST_sum += p[i].og_CPU_burst;
        waiting_time_sum += p[i].waiting_time;
        turnaround_time_sum += p[i].turnaround_time;
        response_time_sum += p[i].response_time;
        context_switch_sum += p[i].context_switch_num;
    }

    cout << string(106, '-') << endl << endl;

    float avg_CPU_BURST = CPU_BURST_sum / num_processes, avg_waiting_time = waiting_time_sum / num_processes, 
    avg_turnaround_time = turnaround_time_sum / num_processes, avg_response_time = response_time_sum / num_processes;

    cout << fixed << setprecision(3) << "Average CPU Burst Time = " << avg_CPU_BURST << " ms\n" << "Average Waiting Time = " << avg_waiting_time << " ms\n" 
    << "Average Turnaround Time = " << avg_turnaround_time << " ms\n" << "Average Response Time = " << avg_response_time << " ms\n" 
    << "Total No. of Context Switches = " << context_switch_sum << endl;
}

// helper method for schedule()
bool time_in_range(int clock, int arrival_time, int cswitches) {
    bool in_range = false;

    for (int i = 0; i <= cswitches; i++) {
        if (clock - (0.5 * i) == arrival_time) {
            in_range = true;
            break;
        }
    }

    return in_range;
}


// This method implements simulations of FIFO, SRTF and RR (quantum > 0) CPU scheduling algorithms
void schedule(PCB* p, int num_processes, int mode, int quantum) {
	Queue ready_queue; // FIFO queue, but with the option to sort for SRTF simulations
	float time = 0;         // system clock
	int p_index = 0;          // represents the next process in PROCESS_LIST to get added to the queue
	int p_count = num_processes; // represents the number of processes that have not been finished
	int last_process_pid = 1;
	int context_switch_count = 0;
	int iterations = quantum;
	bool rr_exception = false; 

	while (p_count != 0) {
		if (time_in_range(time, p[p_index].getArrivalTime(), context_switch_count)) {
			PCB pcb;
			int end_index = ready_queue.length() - 1;

			if (mode == 2 && !rr_exception && ready_queue.length() > 1 && quantum == iterations) {
				pcb = ready_queue.remove(end_index, p, num_processes);
			}

			p[p_index].waiting = true;
			ready_queue.push(p[p_index]);
			p_index++;

			if (mode == 1)
				ready_queue.sort();
			else if (mode == 2) {
				if (rr_exception) {
					PCB temp = ready_queue.pop(p, num_processes);
					ready_queue.push(temp);
					rr_exception = false;
				}
				else {
					if (end_index + 1 > 1 && quantum == iterations)
						ready_queue.push(pcb);
				}
			}
		}
		else {
			if (mode == 2) {
				if (last_process_pid == ready_queue.get(0)->getPID() && iterations == quantum)
					rr_exception = true;
			}
		}

		if (!ready_queue.empty()) {
			if (ready_queue.get(0)->waiting == true) {
				if (mode == 1 || mode == 2) {
					if (last_process_pid != ready_queue.get(0)->getPID()) {
						for (int i = 0; i < ready_queue.length(); ++i) {
							if (ready_queue.get(i)->getPID() == last_process_pid) {
								++ready_queue.get(i)->context_switch_num;

								// Identify a preempted process and add 0.5 to the clock
								if (!ready_queue.get(i)->finished) {
									ready_queue.get(i)->waiting = true;
									ready_queue.get(i)->preempted = true;
									time = time + 0.5;
									context_switch_count++;
								}
								break;
							}
						}
					}
				}

				if (!ready_queue.get(0)->preempted)
					ready_queue.get(0)->response_time = time - ready_queue.get(0)->getArrivalTime();

				ready_queue.get(0)->waiting = false;
			}

			if (mode == 1 || mode == 2)
				last_process_pid = ready_queue.get(0)->getPID();

			// Decrement the CPU Burst field of the current process's PCB
			ready_queue.get(0)->progress();
			if (mode == 2)
				iterations--;

			if (ready_queue.get(0)->getCPUBurst() == 0) {
				ready_queue.get(0)->completion_time = time + 1;
				ready_queue.get(0)->turnaround_time = time + 1 - ready_queue.get(0)->getArrivalTime();
				ready_queue.get(0)->finished = true;

				// Adjust total waiting time of the process to include 0.5 ms for each
				// context switch it had encountered while waiting
				if (context_switch_count > 0)
					ready_queue.get(0)->waiting_time += context_switch_count * 0.5;

				ready_queue.pop(p, num_processes);

				if (mode == 2)
					iterations = quantum;

				--p_count;
			}
			else {
				if (mode == 2 && iterations == 0) {
					PCB temp = ready_queue.pop(p, num_processes);
					ready_queue.push(temp);
					iterations = quantum;
				}
			}
		}

		if (mode == 1)
			ready_queue.sort();

		for (int i = 0; i < ready_queue.length(); ++i) {
			if (!ready_queue.get(i)->finished && ready_queue.get(i)->waiting)
				++ready_queue.get(i)->waiting_time;
		}

		++time;
	}

	print_results(p, num_processes, mode, quantum);
}

int get_num_processes() {
	int num_processes = 0;
	string current_line;
	ifstream infile(PROCESS_FILE);

	while(getline(infile, current_line)) {
		num_processes++;
	}

	infile.close();

	return num_processes;
}

int main(int argc, char* argv[]) {
	PROCESS_FILE = string(argv[1]);
	int num_processes = get_num_processes();
	int algo = atoi(argv[2]);
	int time_quantum = 0;
	PCB* PROCESS_LIST;

	if (algo == 2) {
		if (argv[3] != nullptr) {
			time_quantum = atoi(argv[3]);
		} else {
			cout << "Must specify a time quantum to run Round Robin Scheduler." << endl;
			time_quantum = -1;
		}
	}

	if (time_quantum > -1) {
		PROCESS_LIST = new PCB[num_processes];

		if (get_processes(PROCESS_LIST) == 0) {
			schedule(PROCESS_LIST, num_processes, algo, time_quantum);
		} else {
			cout << "No file named \"" << PROCESS_FILE << "\" found." << endl;
		}

		delete[] PROCESS_LIST;
	}

	return 0;
}