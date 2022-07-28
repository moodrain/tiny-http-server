#include <string.h>
#include <stdio.h>
#include <uriparser/Uri.h>

#include "helper.h"


void wrapStrFromPtr(char* str, int len, const char* head, const char* tail) {
    for (int i = 0; head != tail; head++) {
        str[i++] = *head;
    }  
    str[len - 1] = '\0';
}

int getQueryIntByKey(char* reqBuf, char* key) {
    char* head = strchr(reqBuf, ' ') + 1;
    char* tail = strchr(head, ' ');
    int len = tail - head + 1;
    char uri[len];
    wrapStrFromPtr(uri, len, head, tail);
    UriUriA uriA;
    UriQueryListA* queryList;
    int itemCount, result;
    const char* error;
    if (uriParseSingleUriA(&uriA, uri, &error) == URI_SUCCESS) {
        if (uriDissectQueryMallocA(&queryList, &itemCount, uriA.query.first, uriA.query.afterLast) == URI_SUCCESS) {
            while(itemCount--) {
                if (strcmp(queryList->key, key) == 0) {
                    result = atoi(queryList->value);
                    break;
                }
                queryList = queryList->next;
            }
            uriFreeQueryListA(queryList);
        }
    }
    return result;
}

int __calcFibTCO(int n, int x, int y) {
  if (n == 0)
    return x;
  if (n == 1)
    return y;
  return __calcFibTCO(n - 1, y, x + y);
}

int calcFibonacci(int n) {
  return __calcFibTCO(n, 0, 1);
}