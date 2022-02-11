// Turner Gravin 
// ID: 5576429
// CSE Account: gravi022@umn.edu
// Assignment 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void getInput(int[], int);

int main(int argc, char *argv[]){

    int process_num = atoi(argv[1]);
    int ppid = atoi(argv[2]);

    union sigval sivalue;
    sivalue.sival_int = getpid();

    if(sigqueue(ppid, SIGRTMIN+process_num, sivalue) < 0){
        fprintf(stderr, "sigqueue failed: %s\n", strerror(-1));
        exit(-1);
    }

    sigset_t wait_set;
    sigset_t usr_set;
    sigemptyset(&wait_set);
    sigemptyset(&usr_set);
    sigaddset(&usr_set, SIGUSR1);
    sigaddset(&wait_set, SIGUSR1);

    sigprocmask (SIG_BLOCK, &wait_set, NULL); //block the signal before it arrives

    int sig; 
    sigwait(&usr_set, &sig); //wait to recieve SIGUSR1 back from the aggregator

    sigprocmask (SIG_UNBLOCK, &wait_set, NULL); //unblock the set

    //Reading Input
    int input[100];
    getInput(input, process_num); //call to populate the input array

    int i = 0;
    while(1){

        if(input[i] == -1){ //we are done sending the data
            sivalue.sival_int = (getpid() * (-1));
            
            if(sigqueue(ppid, SIGRTMIN+process_num, sivalue) < 0){
                printf("ERROR: ABORTING\n");
                exit(-1);
            }  
            exit(0);
        }else{  //there is more data to send!
            sivalue.sival_int = input[i];

            if(sigqueue(ppid, SIGRTMIN+process_num, sivalue) < 0){
                printf("ERROR: ABORTING\n");
                exit(-1);
            }   

            sigprocmask (SIG_BLOCK, &wait_set, NULL);
            
            sigwait(&usr_set, &sig);
            sigprocmask (SIG_UNBLOCK, &wait_set, NULL); //unblock the set

            i++;
        }
        
    }

    return 0;
}


void getInput(int input[], int process_num){

    FILE *fp;
    char temp[40];
    char temp2[10] = "data";
    char temp3[10] = ".txt";
    sprintf(temp, "%d", process_num);
    strcat(temp2, temp);
    strcat(temp2, temp3);
    
    fp = fopen(temp2, "r");
    
    char data[10];
    int i = 0;
    
    while (fgets(data, 10, fp) != NULL) {
        input[i] = atoi(data);
        i++;
    }
    input[i] = -1; //set the last element to -1 so we know that this is all of the data to be sent

    fclose(fp);
    return;
}