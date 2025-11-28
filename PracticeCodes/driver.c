#include <stdio.h>
#include <string.h>
#include "des.h"

#define endl printf("\n");



int main(){
    char a[8]={'a','b','c','d','a','b','c','d'};
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    char ciphertext[8];

    encrypt(a,key);

    decrypt(a,key);

    for(int i=0;i<8;i++) printf("%c",a[i]);

    endl
}
