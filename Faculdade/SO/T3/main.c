#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct {
	short priority;
	float f_cpu_timer;
	float IO_timer;
	float s_cpu_timer;
	float exec_time_clock;	
}process_t;

typedef struct {

}process_queue_t;

typedef struct {

}IO_queue_t;

static long global_time = 0.0;


void cpu_execution(process_t ready_process){
	

	global_time += ready_process.f_cpu_timer;
	printf("Finished process job at %ld time\n", global_time);

}

void scheduling(process_t*** processQueue){
	process_t** processQueue_aux = *processQueue;


	// Escalonamento;
	process_t* next_process = processQueue_aux[0];
	cpu_execution(*next_process);

}

int main(){
	
	process_t** processQueue;
	processQueue = malloc(sizeof(process_t) * 10);
	processQueue[0] = malloc(sizeof(process_t));


	processQueue[0]->f_cpu_timer = 3.0;
	scheduling(&processQueue);


	free(processQueue[0]);
	free(processQueue);

	return 0;
}







