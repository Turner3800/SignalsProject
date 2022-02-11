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

void stats(int);

int processes[5]; //array for storing the Process ID's of the producers
int recieved[5000];
int signal_numbers[5000];
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
int count5 = 0;
int tracker = 0;

void rt_handler(int signal, siginfo_t *info, void *arg __attribute__ ((__unused__))) //function declaration used from the example file "realtime3.c" on Canvas
{
    int val = info->si_value.sival_int;
    processes[signal - SIGRTMIN - 1] = val;  //initialize the PID's

    return;
}

void round_handler(int signal, siginfo_t *info, void *arg __attribute__ ((__unused__))) //function declaration used from the example file "realtime3.c" on Canvas
{
    int val = info->si_value.sival_int; //extract the integer value

    if(val < 0){
        processes[signal - SIGRTMIN - 1] = val;  //set the process to negative PID (this means it is done)
    }else{
        recieved[tracker] = val;    //keep track of the values
        signal_numbers[tracker] = signal;  //keep track of the signals that sent the values
        tracker++;

        if(signal == 35){
            count1++;
        }
        if(signal == 36){
            count2++;
        }
        if(signal == 37){
            count3++;
        }
        if(signal == 38){
            count4++;
        }
        if(signal == 39){
            count5++;
        }
    }
    
    return;
}

int main(int argc, char *argv[]){

    if(argc != 2){ //error checking
        printf("Invalid number of arguments.\n");
        return -1;
    }
    int num_producers = atoi(argv[1]);
    if(num_producers < 1 || num_producers > 5){ 
        printf("Invalid Input: Please enter a number from 1-5\n");
        return -1;
    }//end error checking

    //intialize signal set
    sigset_t wait_set;
    sigset_t track_set;
    sigemptyset(&wait_set);
    
    for(int i = 1; i < num_producers + 1; i++){ //add the signals to be blocked into the set
        sigaddset(&wait_set, SIGRTMIN + i);
    }

    //set up the action handler
    struct sigaction action;
    action.sa_flags = SA_SIGINFO;
    action.sa_sigaction = rt_handler;
    sigemptyset(&action.sa_mask);

    //assign the action handlers
    sigaction(SIGRTMIN+1, &action, NULL);
    sigaction(SIGRTMIN+2, &action, NULL);
    sigaction(SIGRTMIN+3, &action, NULL);
    sigaction(SIGRTMIN+4, &action, NULL);
    sigaction(SIGRTMIN+5, &action, NULL);

    sigfillset(&track_set);
    sigdelset(&track_set, SIGRTMIN+1);

    //block incoming signals while we wait to recieve all of the initial SIGRTMIN signals
    sigprocmask (SIG_BLOCK, &wait_set, NULL);

    for(int i = 1; i < num_producers+ 1; i++){ //create child processes

        int pid = getpid();
        int num = fork();
        if(num == 0){//child
            char temp[5];
            char temp2[5];
            sprintf(temp, "%d", i);
            sprintf(temp2, "%d", pid);
            execl("./producer", "producer", temp, temp2, NULL);
        }
    }

    //wait to recieve all of the required signals
    int sig;
    for(int i = 1; i < num_producers + 1; i++){
        sigsuspend(&track_set);
        sigdelset(&track_set, SIGRTMIN+i+1); //add the next signal in the set to be waited on
    }
    sigprocmask (SIG_UNBLOCK, &wait_set, NULL);

    for(int i = 1; i < num_producers + 1; i++){ //send SIGUSR1 to each of the producers
        kill(processes[i-1], SIGUSR1);
    }

    struct sigaction action2;
    action2.sa_flags = SA_SIGINFO;
    action2.sa_sigaction = round_handler;
    sigemptyset(&action.sa_mask);

    sigaction(SIGRTMIN+1, &action2, NULL); //set the new signal handler
    sigaction(SIGRTMIN+2, &action2, NULL);
    sigaction(SIGRTMIN+3, &action2, NULL);
    sigaction(SIGRTMIN+4, &action2, NULL);
    sigaction(SIGRTMIN+5, &action2, NULL);

    while(1){

        sigprocmask (SIG_BLOCK, &wait_set, NULL); //block the signals again

        sigfillset(&track_set);       //reset the tracking set of signals
        sigdelset(&track_set, SIGRTMIN+1);

        int done_count = 0;
        for(int i = 1; i < num_producers + 1; i++){ //wait for all signals to arrive again
            if(processes[i-1] >= 0){
                sigsuspend(&track_set);
                sigdelset(&track_set, SIGRTMIN+i+1); 
            }else{
                sigdelset(&track_set, SIGRTMIN+i+1); 
                done_count++;
            }
            if(done_count == num_producers){ //got all of the data!
                stats(num_producers);
                exit(0); //finished!
            }
        }

        for(int i = 1; i < num_producers + 1; i++){ //send SIGUSR1 to each of the producers
            if(processes[i-1] > 0){
                kill(processes[i-1], SIGUSR1);
            }
        }

    }

    return 0;
}

void stats(int num_producers){

    FILE *fp;
    fp = fopen("log.txt", "w"); //open log file for writing

    for(int i = 0; i < tracker; i++){  //print to the log file
        char temp[50] = "Signal: ";
        char temp2[20];
        char temp3[20];
        
        sprintf(temp2, "%d", signal_numbers[i]);
        sprintf(temp3, "%d", recieved[i]);
        
        strcat(temp, temp2);
        strcat(temp, " value: ");
        strcat(temp, temp3);
        strcat(temp, "\n");
        fputs(temp, fp);
    }
    
    fclose(fp);

    //calculate the statistics here
    int sum = 0, average = 0, min = 0, max = 0, total = tracker;

    min = recieved[0];
    max = recieved[0];
    for(int i = 0; i < tracker; i++){
        sum += recieved[i];

        if(recieved[i] < min){
            min = recieved[i];
        }
        if(recieved[i] > max){
            max = recieved[i];
        }
    }

    average = sum / total;

    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    printf("Average: %d\n", average);
    printf("Total Values recieved: %d\n", total);

    printf("Total Values from Producer 1: %d\n", count1);

    if(num_producers > 1){
        printf("Total Values from Producer 2: %d\n", count2);
    }
    if(num_producers > 2){
        printf("Total Values from Producer 3: %d\n", count3);
    }
    if(num_producers > 3){
        printf("Total Values from Producer 4: %d\n", count4);
    }
    if(num_producers > 4){
        printf("Total Values from Producer 5: %d\n", count5);
    }
    
    return;
}