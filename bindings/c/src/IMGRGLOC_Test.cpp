#include "IMGRGLOC_Test.h"
#include <random>

//get a pre-allocated block of memory, size = n
char* IMGRGLOC_TEST_get_random_bytes(int n){
    static char x[1000];
    static std::default_random_engine rd;
    std::uniform_int_distribution<int> d('a','z');
    for(int i=0;i<n && i<sizeof(x); i++){
        x[i] = d(rd);
    }
    return &x[0];
}

//fill a host-allocated block of memory, size = n
void IMGRGLOC_TEST_random_bytes(char* buf, int n){
    static char x[1000];
    static std::default_random_engine rd;
    std::uniform_int_distribution<int> d('a','z');
    for(int i=0; i<n && i<sizeof(x); i++)
    {
        buf[i] = d(rd);
    }
}

//allocate a block of memory, then release it
char* IMGRGLOC_TEST_allocate_random_string(int n){
    static std::default_random_engine rd;
    std::uniform_int_distribution<int> d('a','z');

    auto x = new char[n];
    for(int i=0; i<n-1; i++){
        x[i] = d(rd);
    }
    x[n-1] = '\0';

    return x;
}

void IMGRGLOC_TEST_free_string(char* s){
    delete[] s;
}