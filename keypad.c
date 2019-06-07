#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
enum op {SYNCH, DOOR, LIGHT, EMIT, KEYPAD, RUN, COOKTIME};

int displayMenu();
int normCook();
int foodMenu();
int *shm;
int shm_id;
int doorstat;
int oc;
int cookTime;
//function sleep declaration

int main() {
    key_t key;
    

    key=0x520260A;
    //key = ftok("/home/kosuke/IPU", 'a' );
    printf("key= %x\n", key);
    shm_id = shmget(key, sizeof(int)*10, 0666);

    printf(" shm_id = %d\n", shm_id);
    shm = shmat(shm_id,0,0);
    printf("waiting for GO.\n");
    while ( *(shm+SYNCH) == 0 );

    // start application

    int option = 0;

    while(option == 1 || option == 2 || option == 3 || option == 0) {
        option = displayMenu();
        if(option == 1) {
		*(shm+RUN) = 0;	
		setTime();
        }
	if(option == 2) {
		*(shm+RUN) = 1;
        }
        if(option == 3) {
		*(shm+RUN) = 0;
        }
	if (!(option == 1 || option == 2 || option == 3 || option == 0)) {
		option = 0;
	}
    }
    printf(":POWERING OFF: \n");
}//end main


int displayMenu() {
    int choice = 0;
    printf("Welcome to Microwave 9000! \n");
    printf("Please select an option: \n");
    printf("1. set cook time \n");
    printf("2. start \n");
    printf("3. stop \n");
    printf("4. power off \n");
    scanf("%d", &choice);
    return choice;

}

int setTime() {
    printf("How long to cook for?");
    scanf("%d", &cookTime);
    *(shm+COOKTIME) = cookTime;
}

//Disabled
/*int foodMenu(){
    int foodchoice = 0;
    printf("Choose a food option ");
    printf("1.Popcorn");
    printf("2.Pork");
    printf("3.Chicken");
    scanf("%d", &foodchoice);
    return foodchoice;
}*/
