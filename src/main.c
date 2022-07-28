#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <signal.h>
#include <stdnoreturn.h>
#include <sys/socket.h>
#include <sys/syscall.h>

#include "libs/helper.h"

atomic_int threadCounter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void renewThread(void *arg) {  
    printf("Renew Thread\n");
    int* acSocket = (int*) arg;
    close(*acSocket);
    pthread_mutex_lock(&mutex);
    atomic_fetch_sub(&threadCounter, 1);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
} 

noreturn void* handleConn(void* arg) {
    acceptParams* ap = (acceptParams*) arg;
    int acSocket;
    const long tid = syscall(SYS_gettid);
    printf("Thread %ld Ready\n", tid);
    while(1) {
        pthread_cleanup_push(renewThread, &acSocket);
        if ((acSocket = accept(ap->serverFd, (sockaddr*) ap->addr, (socklen_t*) ap->addrLen)) < 0) {
            perror("In accept");
            pthread_exit(NULL);
        }
        char reqBuf[HTTP_REQ_BUF];
        memset(reqBuf, 0, HTTP_REQ_BUF);
        int receivedBytes = read(acSocket, reqBuf, HTTP_REQ_BUF);
        if (receivedBytes > 0) {
            char resBuf[HTTP_RES_BUF];
            pthread_mutex_lock(&mutex);
            const int num = getQueryIntByKey(reqBuf, "num");
            pthread_mutex_unlock(&mutex);
            if ((rand() % 100) < ERR_PERCENT) {
                pthread_exit(NULL);
            }
            int result = calcFibonacci(num);
            sprintf(resBuf, "HTTP/1.1 200 OK\r\n\r\n%d\r\n", result);
            write(acSocket, resBuf, strlen(resBuf));
            const long tid = syscall(SYS_gettid);
            printf("Thread %ld handle request's number: %d -> %d\n", tid, num, result);
        }
        close(acSocket);        
        pthread_cleanup_pop(0);
    }
}

int main() {

    time_t seed;
    srand(time(&seed));

    int serverFd; 
    sockaddr_in address;
    int addrLen = sizeof(address);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket creation");
        exit(-1);
    }

    memset(&address, 0, addrLen);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(serverFd, (sockaddr*) &address, sizeof(address)) < 0) {
        perror("In bind");
        exit(-1);
    }
    if (listen(serverFd, MAX_LISTEN_CONN) < 0) {
        perror("In listen");
        exit(-1);
    }

    int acSocket;
    while(1) {
        pthread_mutex_lock(&mutex);
        while (threadCounter >= THREAD) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        pthread_t threadId;
        acceptParams ap = { serverFd, (sockaddr*) &address, (socklen_t*) &addrLen };
        pthread_create(&threadId, NULL, handleConn, &ap);
        atomic_fetch_add(&threadCounter, 1);
        printf("Thread Created: No.%d\n", threadCounter);
    }
    return 0;
}
