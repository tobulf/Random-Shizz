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


using namespace std;
// normal length for a SMS is 160 bytes.
#define MEM_SIZE 160
// defining semaphores:
#define SEM_NAME "/SMSsem5"



struct sms {
	char number[11];
	char body[160];
	int to_read;
};


int main(int argc, char *argv[]) {
	int shmid;
	key_t key;
	char *shm;
	char *pointer;

	key = 7020;
	// opening a semaphore and a shared memory:
	
	sem_t *sem = sem_open(SEM_NAME, 0);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}
	shmid = shmget(key, MEM_SIZE, 0666);

	if (shmid < 0) {

		perror("shmget");
		exit(1);
	}

	shm = static_cast<char*>(shmat(shmid, NULL, 0));

	if (shm == (char *)-1) {
		perror("shmget");
		exit(1);
	}
	
	for (;;) {
		sem_wait(sem);

		for (pointer = shm; *pointer != 0; pointer++) {
			cout << pointer;
		}
		cout << "" << endl;
		*shm = '*';
		
		sem_post(sem);
	}

	return 0;
}
