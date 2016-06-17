/*A2.c

I completed every element of the assignment except for the round robin scheduling, which I briefly worked on
an array with rrArray later on in the code. If it helps my marking, I had a bunch of code where the code
simply adds a new elements to my readyQueue and treated the entire process like a round robin scheduling
system, but segfaults forced me to not bother and work on the million other assignments I have to deal with.

As for using it, simcpu is already entered and -d -v < input_file can be input in any order. Any mistakes
with syntax will simply not run the program.

Also, the code is parsed for RR scheduling. If I did not have so many problems, then it would've been
fully implemented.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Burst{
	int burstNo;
	int cpuTime;
	int ioTime;
}Burst;

typedef struct Thread{
	Burst burst[32];
	int arrivalTime;
	int numBurst;
	int serviceTime;
	int ioTime;
	int finishTime;
	int threadNo;
}Thread;

typedef struct Process{
	Thread thread[32];
	int numThread;
	int procNo;
}Process;

int main(){
	FILE *file;
	Process proc[32];
	char *fileName = calloc(256, sizeof(char));
	char *input = calloc(256, sizeof(char));
	char *token = calloc(256, sizeof(char));
	double turnaround, totalTime, idleTime;
	int readyQueue[128][4];//0 is the process no, 1 is the thread no, 2 is the burst no and 3 is the time the burst was entered into the queue
	int isDetail, isFile, isRobin, isVerbose, numElements, p, t, b, i, j, k, x, y, z;//meta numbers
	int threadSwitch, procSwitch, numProc, bursts, isCalculated;//calculation numbers
	long quantum;

	while(1){
		strcpy(fileName, "");
		readyQueue[0][0] = 0;
		readyQueue[0][1] = 0;
		readyQueue[0][2] = 0;
		readyQueue[0][3] = 99999;
		isDetail = isVerbose = isRobin = isFile = quantum = turnaround = numProc = threadSwitch 
			= procSwitch = totalTime = numElements = p = t = b = z = bursts = idleTime = 0;
		printf("simcpu ");
	
		gets(input);
		if(strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0)
			break;
		
		token = strtok(input, " ");
		while(token){
			if(strcmp(token, "-d") == 0)
				isDetail = 1;
			else if(strcmp(token, "-v") == 0)
				isVerbose = 1;
			else if(strcmp(token, "-r") == 0)
				isRobin = 1;
			else if(strcmp(token, "<") == 0)
				isFile = 1;
			else if(isFile)
				strcpy(fileName, token);
			else if(isRobin)
				quantum = strtol(token, NULL, 10);
				
			token = strtok(NULL, " ");
		}
		
		file = fopen(fileName, "r");
		if(file == 0){
			isFile = 0;
		}else{//if a file is read, then all of the information is processed into the ready queue as follows
			fscanf(file, "%d %d %d", &numProc, &threadSwitch, &procSwitch);
			for(i = 0; i < numProc; i++){//processes are systematically entered into the array of structs
				fscanf(file, "%d %d", &proc[i].procNo, &proc[i].numThread);
				for(j = 0; j < proc[i].numThread; j++){//threads are entered into the array of thread structs for each process
					fscanf(file, "%d %d %d", &proc[i].thread[j].threadNo, &proc[i].thread[j].arrivalTime
						, &proc[i].thread[j].numBurst);
					proc[i].thread[j].ioTime = 0;
					proc[i].thread[j].serviceTime = 0;
					numElements++;

					for(x = 0; x < numElements; x++){//adds the information into the ready queue and shifts it accordingly
					//as a side note, the ready queue entered here is only the threads and processes, individual bursts are calculated at runtime later
						if(readyQueue[x][3] > proc[i].thread[j].arrivalTime){
							for(y = numElements; y > x; y--){//the shift for statement
								readyQueue[y][0] = readyQueue[y - 1][0];
								readyQueue[y][1] = readyQueue[y - 1][1];
								readyQueue[y][2] = readyQueue[y - 1][2];
								readyQueue[y][3] = readyQueue[y - 1][3];
							}
							readyQueue[x][0] = i;
							readyQueue[x][1] = j;
							readyQueue[x][2] = 0;
							readyQueue[x][3] = proc[i].thread[j].arrivalTime;
							break;
						}
					}
					
					for(k = 0; k < (proc[i].thread[j].numBurst - 1); k++){//each burst is entered into the data struct for the array of bursts in its thread
						fscanf(file, "%d %d %d", &proc[i].thread[j].burst[k].burstNo
							, &proc[i].thread[j].burst[k].cpuTime, &proc[i].thread[j].burst[k].ioTime);
						proc[i].thread[j].serviceTime += proc[i].thread[j].burst[k].cpuTime;
						proc[i].thread[j].ioTime += proc[i].thread[j].burst[k].ioTime;
						bursts++;
					}//the following is repeated because the final burst does not include the ioTime information
					fscanf(file, "%d %d", &proc[i].thread[j].burst[k].burstNo
						, &proc[i].thread[j].burst[k].cpuTime);
					proc[i].thread[j].serviceTime += proc[i].thread[j].burst[k].cpuTime;
					proc[i].thread[j].burst[k].ioTime = 0;
					bursts++;
				}
			}
			
			totalTime = readyQueue[0][3];
			x = -1;
			while(1){//continues until the end of every thread is found, completes the ready queue with arrival times
				x++;
				if(x >= bursts)
					break;
				p = readyQueue[x][0];
				t = readyQueue[x][1];
				b = readyQueue[x][2];
				
				isCalculated = 0;
				if(proc[p].thread[t].burst[b].ioTime != 0){
					for(i = 0; i < numElements; i++){
						if(proc[p].thread[t].burst[b].cpuTime > quantum && quantum > 0){
							for(j = 0; j < numElements; j++){
								if(readyQueue[j][3] > (totalTime + quantum + proc[p].thread[t].burst[b].ioTime)){
									for(y = numElements; y > j; y--){
										readyQueue[y][0] = readyQueue[y - 1][0];
										readyQueue[y][1] = readyQueue[y - 1][1];
										readyQueue[y][2] = readyQueue[y - 1][2];
										readyQueue[y][3] = readyQueue[y - 1][3];
									}
									break;
								}
							}
							proc[p].thread[t].burst[b].cpuTime -= quantum;
							totalTime += (quantum + proc[p].thread[t].burst[b].ioTime);
							proc[p].thread[t].burst[b].ioTime = 1;
							bursts++;
							isCalculated = 1;
							break;
						}else if(readyQueue[i][3] > (proc[p].thread[t].burst[b].cpuTime + proc[p].thread[t].burst[b].ioTime + totalTime)){
							for(y = numElements; y > i; y--){
								readyQueue[y][0] = readyQueue[y - 1][0];
								readyQueue[y][1] = readyQueue[y - 1][1];
								readyQueue[y][2] = readyQueue[y - 1][2];
								readyQueue[y][3] = readyQueue[y - 1][3];
							}
							b++;
							totalTime += proc[p].thread[t].burst[b].cpuTime;
							isCalculated = 1;
							break;
						}
					}
					if(!isCalculated){
						totalTime += proc[p].thread[t].burst[b].cpuTime;
						b++;
					}
					readyQueue[i][0] = p;
					readyQueue[i][1] = t;
					readyQueue[i][2] = b;
					numElements++;
					
					if(x == 0){
						x = x;//placeholder logic
					}else if(p == readyQueue[x - 1][0]){//same process thread = shorter time
						totalTime += 3;
						idleTime += 3;
					}else if(p != readyQueue[x - 1][0]){//new process thread = longer time
						totalTime += 7;
						idleTime += 7;
					}
					readyQueue[i][3] = proc[p].thread[t].burst[b].ioTime + totalTime;
				}else{
					totalTime += proc[p].thread[t].burst[b].cpuTime;
					if(x == 0){
						x = x;//placeholder logic
					}else if(p == readyQueue[x - 1][0]){//same process thread = shorter time
						totalTime += 3;
						idleTime += 3;
					}else if(p != readyQueue[x - 1][0]){//new process thread = longer time
						totalTime += 7;
						idleTime += 7;
					}
				}
				
				proc[p].thread[t].finishTime = totalTime;
			}
		}
		
		if(isFile){
			x = 0;
			for(i = 0; i < numProc; i++){
				for(j = 0; j < proc[i].numThread; j++){
					turnaround += (proc[i].thread[j].finishTime - proc[i].thread[j].arrivalTime);
					x++;
				}
			}
			turnaround = turnaround / x;
		
			if(quantum != 0){
				printf("Round Robin Scheduling (quantum = %ld time units)\n", quantum);
			}else{
				printf("FCFS Scheduling\n");
			}
			printf("Total Time required is %.0f time units\n", totalTime);
			printf("Average Turnaround Time is %.1f\n", turnaround);
			printf("CPU Utilization is %.1f%%\n", ((totalTime - idleTime) / totalTime) * 100);
			
			if(isDetail){
				for(i = 1; i <= numProc; i++){
					for(j = 1; j <= proc[i - 1].numThread; j++){
						printf("Thread %d of Process %d:\n", j, i);
						printf("\tarrival time: %d\n", proc[i - 1].thread[j - 1].arrivalTime);
						printf("\tservice time: %d units, I/O time: %d units, turnaround time: %d units, finish time: %d units\n"
							, proc[i - 1].thread[j - 1].serviceTime
							, proc[i - 1].thread[j - 1].ioTime
							, proc[i - 1].thread[j - 1].finishTime - proc[i - 1].thread[j - 1].arrivalTime
							, proc[i - 1].thread[j - 1].finishTime);
					}
				}
			}
			
			if(isVerbose){
				for(i = 1; i <= numProc; i++){
					for(j = 1; j <= proc[i - 1].numThread; j++){
						printf("At time %d: Thread %d of Process %d moves from new to ready\n"
							, proc[i - 1].thread[j - 1].arrivalTime, j, i);
						printf("At time %d: Thread %d of Process %d moves from ready to running\n"
							, proc[i - 1].thread[j - 1].arrivalTime + proc[i - 1].thread[j - 1].burst[0].ioTime, j, i);
						printf("At time %d: Thread %d of Process %d moves from running to terminated\n"
							, proc[i - 1].thread[j - 1].finishTime, j, i);
					}
				}
			}
		}else{
			printf("No input file detected or selected file failed to parse.\n");
		}
	}
	
	free(fileName);
	free(input);
	free(token);
}