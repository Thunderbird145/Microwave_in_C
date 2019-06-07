#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

enum op {SYNCH, DOOR, LIGHT, EMIT, KEYPAD, RUN, TIME};

int main()
{
    key_t key;
	int shm_id;
	int *shm;
	int emitstat;
	int os = 0;
	
	key=0x520260A;
	//key = ftok("/home/kosuke/IPU", 'a' );
	printf("key= %x\n", key);
	shm_id = shmget(key, sizeof(int)*10, 0666);
	
	printf(" shm_id = %d\n", shm_id);
	shm = shmat(shm_id,0,0);
	printf("waiting for GO.\n");
	while ( *(shm+SYNCH) == 0 );

	printf("light application started.\n");
	printf("%d\n", *(shm+LIGHT));	
	while(1) {
		if (*(shm+EMIT) == 1 || *(shm+DOOR) == 1) {
         		if (*(shm+LIGHT) == 0) {
            			*(shm+LIGHT) = 1;
            			printf("Light is on.\n");
         		}
      		} else {
         		if (*(shm+LIGHT) == 1) {
            			*(shm+LIGHT) = 0;
            			printf("Light is off.\n");
         		}
      		}
	}
}
