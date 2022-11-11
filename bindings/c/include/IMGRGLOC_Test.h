#pragma once

//test data transfer between c++ and target languages
extern "C"{
    //get a pre-allocated block of memory, size = n
    char* IMGRGLOC_TEST_get_random_bytes(int n);

    //fill a host-allocated block of memory, size = n
    void IMGRGLOC_TEST_random_bytes(char* buf, int n);

    //allocate a block of memory, then release it
    char* IMGRGLOC_TEST_allocate_random_string(int n);
    void IMGRGLOC_TEST_free_string(char* s);
}