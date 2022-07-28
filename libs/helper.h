#ifndef _LIB_HELPER
#define _LIB_HELPER

#include <sys/socket.h>

#define THREAD 4
#define PORT 8080
#define MAX_LISTEN_CONN 128
#define ERR_PERCENT 20
#define HTTP_REQ_BUF 1024
#define HTTP_RES_BUF 1024

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

typedef struct {
  int serverFd;
  sockaddr* addr;
  socklen_t* addrLen;
} acceptParams;


void wrapStrFromPtr(char* str, int len, const char* head, const char* tail);
int getQueryIntByKey(char* uri, char* key);
int calcFibonacci(int num);


#endif