#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>



using namespace std;

#define MEM_SIZE 160
#define SEM_NAME "/SMSsem5"






int main(int argc, char *argv[]) {
	int shmid;
	key_t key;
	char *shm; 
	char *pointer;
	char message[] = "";

	key = 7020;


	sem_t *sem = sem_open(SEM_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	shmid = shmget(key, MEM_SIZE, IPC_CREAT | 0666);

	if (shmid < 0){
		perror("shmget");
		exit(1);
	}


	shm = static_cast<char*>(shmat(shmid, NULL, 0));
	

	if (shm == (char *) -1){
		perror("shmget");
		exit(1);
	}
	for (;;) {
		cout << "What is your message?" << endl;
		cin >> message;
		sem_wait(sem);
		memcpy(shm, message, strlen(message));
		sem_post(sem);

		pointer = shm;

		pointer += strlen(message);

		*pointer = 0;

		while (*shm != '*') {
			usleep(1);
		}

	}



	return 0;
}
