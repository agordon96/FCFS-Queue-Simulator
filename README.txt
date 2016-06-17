//////////////////////////A2.C\\\\\\\\\\\\\\\\\\\\\\\\\\
Aaron Gordon
0884023
CIS*3110 Operating Systems

//HOW TO USE\\
The program functions like a shell script in that ./simcpu will bring you
to the simcpu script, then you can simply type < INPUT FILE to work the program.
Typing any of the following modes will do its function if inputted before the <,
and any errors in formatting will be checked and displayed (although most are accounted
for such as different orderings):

-d for detailed, provides info on each thread
-v for verbose, provides info on when threads are ready, run and terminate
-r X for round robin scheduling, if X is 0 or below then it will default to FCFS

//FUNCTIONALITY\\
Currently, all functions related to FCFS are working perfectly, but when using rr,
there are differences and large heaps of code built to making it work, but it doesn't
quite work properly. Considering this was the code I had done at the time of A2, I 
couldn't work on it more now, although now with my extra knowledge of programming
and this course, I would add a rrQueue instead of a readyQueue when quantum is greater than
0.