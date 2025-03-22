#include <pthread.h>
#include <stdio.h>


void *printThread(){
	printf("%d",1);
	return NULL;
}

void *printThread2(){
	printf("%d",2);
	return NULL;
}

int main(){
	pthread_t thread_1, thread_2;
	pthread_create(&thread_1, NULL, printThread, NULL);
	pthread_create(&thread_2, NULL, printThread2, NULL);
	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	printf("\nThread Executed");
	return 1;
}

