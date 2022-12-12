#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "a2_helper.h"

// Declaring a semaphore
int sem_id;

void P(int sem_id, int sem_no)
{
    struct sembuf op = {sem_no, -1, 0};
    semop(sem_id, &op, 1);
}

void V(int sem_id, int sem_no)
{
    struct sembuf op = {sem_no, +1, 0};
    semop(sem_id, &op, 1);
}

void P9Threads(int *threadNumber)
{
    int threadNumberInt = *threadNumber;
    // thread 4 starts before thread 2
    if (threadNumberInt == 2) {
        P(sem_id, 0);
    }
    // Block P9.3 for Ex 2.5
    if (threadNumberInt == 3) P(sem_id, 5);
    info(BEGIN, 9, *threadNumber);
    if (threadNumberInt == 4) {
        V(sem_id, 0);
    }
    // thread 4 ends after thread 2
    if (threadNumberInt == 4) {
        P(sem_id, 1);
    }
    info(END, 9, *threadNumber);
    if (threadNumberInt == 3) V(sem_id, 6);
    if (threadNumberInt == 2) {
        V(sem_id, 1);
    }
}

void P7Threads(int *threadNumber)
{
    int threadNumberInt = *threadNumber;
    if (threadNumberInt == 11) {
        P(sem_id, 3);
        P(sem_id, 3);
        P(sem_id, 3);
    } else {
        P(sem_id, 2);
    }
    info(BEGIN, 7, *threadNumber);
    V(sem_id, 3);
    if (threadNumberInt != 11) P(sem_id, 4);
    info(END, 7, *threadNumber);
    if (threadNumberInt == 11) {
        semctl(sem_id, 4, SETVAL, 99);
    }
    V(sem_id, 2);
}

void P2Threads(int *threadNumber)
{
    int threadNumberInt = *threadNumber;
    if (threadNumberInt == 4) P(sem_id, 6);
    info(BEGIN, 2, *threadNumber);
    info(END, 2, *threadNumber);
    if (threadNumberInt == 1) V(sem_id, 5);
}

int main() {
    // Initializing 3 semaphores
    sem_id = semget(IPC_PRIVATE, 7, IPC_CREAT | 0600);
    // The processes
    int p2, p3, p4, p5, p6, p7, p8, p9;
    init();
    info(BEGIN, 1, 0);
    // P1 exists
    p2 = fork();
    if (p2 == 0) { // child
        // P2 created
        info(BEGIN, 2, 0);
        /* CREATING 5 THREADS */
        pthread_t threads[5];
        int arg[5];
        // Initializing the next 2 semaphores
        semctl(sem_id, 5, SETVAL, 0);
        semctl(sem_id, 6, SETVAL, 0);
        // Creating the threads
        for (int i = 1; i <= 5; i++) {
            arg[i - 1] = i;
            pthread_create(&threads[i-1], NULL, (void*(*)(void *))P2Threads, &arg[i-1]);
        }
        // Main threads wait for all other to finish
        for(int i = 1; i <= 5; i++) {
            arg[i-1] = i;
            pthread_join(threads[i-1], (void**)&arg[i-1]);
        }
        info(END, 2, 0);
    }
    else { // parent
        p3 = fork();
        if (p3 == 0) { // child
            // P3 created
            info(BEGIN, 3, 0);
            p5 = fork();
            if (p5 == 0) { // child
                // P5 created
                info(BEGIN, 5, 0);
                info(END, 5, 0);
            }
            else { // parent
                p6 = fork();
                if (p6 == 0) { // child
                    // P6 created
                    info(BEGIN, 6, 0);
                    p8 = fork();
                    if (p8 == 0) { // child
                        // P8 was created
                        info(BEGIN, 8, 0);
                        p9 = fork();
                        if (p9 == 0) { // child
                            // P9 was created
                            info(BEGIN, 9, 0);
                            /* CREATING 4 THREADS */
                            pthread_t threads[4];
                            int arg[4];
                            // Initializing the first 2 sempahores with 0
                            semctl(sem_id, 0, SETVAL, 0);
                            semctl(sem_id, 1, SETVAL, 0);
                            // Creating the threads
                            for (int i = 1; i <= 4; i++) {
                                arg[i - 1] = i;
                                pthread_create(&threads[i-1], NULL, (void*(*)(void *))P9Threads, &arg[i-1]);
                            }
                            // Main threads wait for all other to finish
                            for(int i = 1; i <= 4; i++) {
                                arg[i-1] = i;
                                pthread_join(threads[i-1], (void**)&arg[i-1]);
                            }
                            info(END, 9, 0);
                        }
                        else { // parent
                            waitpid(p9, NULL, 0);
                            info(END, 8, 0);
                        }
                    }
                    else { // parent
                        waitpid(p8, NULL, 0);
                        info(END, 6, 0);
                    }
                }
                else { // parent
                    waitpid(p5, NULL, 0);
                    waitpid(p6, NULL, 0);
                    info(END, 3, 0);
                }
            }
        }
        else { // parent
            p4 = fork();
            if (p4 == 0) { // child
                // P4 created
                info(BEGIN, 4, 0);
                p7 = fork();
                if (p7 == 0) { // child
                    // P7 created
                    info(BEGIN, 7, 0);
                    /* CREATING 46 THREADS */
                    pthread_t threads[46];
                    int arg[46];
                    // Initializing the next 3 sempahores
                    semctl(sem_id, 2, SETVAL, 3);
                    semctl(sem_id, 3, SETVAL, 0);
                    semctl(sem_id, 4, SETVAL, 0);
                    // Creating the threads
                    for (int i = 1; i <= 46; i++) {
                        arg[i - 1] = i;
                        pthread_create(&threads[i-1], NULL, (void*(*)(void *))P7Threads, &arg[i-1]);
                    }
                    // Main thread waits for all others to end
                    for (int i = 1; i <= 46; i++) {
                        arg[i-1] = i;
                        pthread_join(threads[i-1], (void**)&arg[i-1]);
                    }
                    info(END, 7, 0);
                }
                else { // parent
                    waitpid(p7, NULL, 0);
                    info(END, 4, 0);
                }
            }
            else { // parent
                waitpid(p2, NULL, 0);
                waitpid(p3, NULL, 0);
                waitpid(p4, NULL, 0);
                info(END, 1, 0);
            }
        }
    }
    return 0;
}