#include<pthread.h>
#include"openfile.h"


void create_thread(){

}


int main(int argc, char* argv[]){
	pthread_t prime_thread;
	char* filename = argv[1];
	int arraySize, *numberArray;

	struct ThreadArgs args = {filename, &arraySize, &numberArray};



	pthread_create(&prime_thread, NULL, openFileAndStore, (void*)args);
	

}
