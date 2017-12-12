
#define SHMOBJ_PATH "/inbox"
#define SEMOBJ_PATH "/inbox_semaphore"

struct sms {
	char number[11];
	char body[160];
	int to_read;
};

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>



#define QUEUE_LEN 5


struct sms msg_queue[] = {
	{ "3490000000", "Hello!" },
	{ "3490000001", "Ready?" },
	{ "3490000002", "Please call me" },
	{ "3490000003", "I'm waiting" },
	{ "", "..." },
};

void send(struct sms * out_ptr, struct sms * msg) {
	memcpy(out_ptr->number, msg->number, sizeof(msg->number));
	memcpy(out_ptr->body, msg->body, sizeof(msg->body));
	out_ptr->to_read = 1;
}
int main() {
	int shm_fd = shm_open(SHMOBJ_PATH, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("Error while opening inbox memory segment\n");
		exit(EXIT_FAILURE);
	}
	size_t msg_size = sizeof(struct sms);
	ftruncate(shm_fd, msg_size);
	printf("Shared inbox size: %ld\n", msg_size);
	struct sms * out_ptr = (struct sms *) mmap(NULL, msg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (out_ptr == NULL) {
		printf("Access to inbox memory segment failed\n");
		exit(EXIT_FAILURE);
	}
	sem_t *sem_id = sem_open(SEMOBJ_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);

		for (int id = 0; id<QUEUE_LEN; id++) {
			sem_wait(sem_id);
			while (out_ptr->to_read) {
				printf("Waiting for prev SMS(%d) to be read\n", id);
				sem_post(sem_id);
				sleep(1);
				sem_wait(sem_id);
			}
			send(out_ptr, &msg_queue[id]);
			sem_post(sem_id);
			printf("SMS [%d] sent\n", id);
		}

	munmap(out_ptr, msg_size);
	sem_close(sem_id);
	/* include this only when you’re sure that the dispatcher will no longer
	execute
	*/
	return 0;
}