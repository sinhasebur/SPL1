#include <stdio.h>
#include <time.h>

__uint64_t random_state;

__uint64_t seed(){
    return (__uint64_t)time(NULL);
}


int random01(){
    //from https://en.wikipedia.org/wiki/Xorshift 
    __uint64_t x = random_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    random_state = x;

    return x&1;
}


void generateRandomKey(int keysize, int keyspace, int* key){
    
    random_state=seed();
    for(int i=0;i<keyspace;i++){
        key[i]=random01();
    }
    for(int i=keyspace;i<keysize;i++){
        key[i]=0;
    }
}
