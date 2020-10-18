#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct page{

    int pid;
    int Rbit;         
    char* page;
    int timestamp;
};


struct WS{

    char* page;
    int timestamp;
    int pid;
};

typedef struct WS WorkingSet;
typedef struct WS* WSptr;

typedef struct page Page;
typedef struct page* Pageptr;