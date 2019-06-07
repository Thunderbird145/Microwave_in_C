#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_SEC 6039
#define KEY 0x520260A

enum op {SYNCH, DOOR, LIGHT, EMIT, KEYPAD, RUN, TIME};

int *shm;
int shm_id;

int main() {
	key_t key = KEY;
	shm_id = shmget(key, sizeof(int)*10, 0666);
	shm = shmat(shm_id,0,0);
	int time = *(shm+TIME);
	printf(" shm_id = %d\n", shm_id);
	printf("waiting for GO.\n");
	while ( *(shm+SYNCH) == 0 );
	printf("timer application started.\n");

	while(1) {
		while(*(shm+RUN) == 1 && *(shm+TIME) > 0) {
			printf("");
			if(*(shm+DOOR) == 0) { // door is closed
				printf("Time: %d \n", *(shm+TIME));
				sleep(1);
				*(shm+TIME) = *(shm+TIME) - 1;
				if(*(shm+TIME) == 0) {
					*(shm+RUN) = 0;
					printf("cooking done. \n");
				}
			} else {
				printf("WARNING: DOOR IS OPEN. \n");
				*(shm+RUN) = 0;
			}
		}
	}
}
