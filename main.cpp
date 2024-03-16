#include "scheduler.h"
#include "PCB.h"
#include "queue.h"
#include "readfile.h"
#include <iostream>
#include <cstdlib>

int main(int argc, const char * argv[]) {
	int q;

	if (argc > 3) {
		q = atoi(argv[3]);
	} else {
		q = 1;
	}

	Queue<PCB> fileLoad = read_file(argv[1]);
	Scheduler myScheduler(atoi(argv[2]), q);
	myScheduler.loadUnscheduledPCBs(fileLoad);
	myScheduler.run();
	myScheduler.printResults();

	return 0;
}