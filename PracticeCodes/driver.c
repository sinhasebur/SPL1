#include <stdio.h>
#include <string.h>
#include "des.h"
#include "2des.h"

#define endl printf("\n");



int main(){
    char a[8]={'a','b','c','d','a','b','c','d'};
    char b[8],c[8];
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};

    char ciphertext[8];

    encrypt(a,key,b);

    decrypt(b,key,c);

    for(int i=0;i<8;i++) printf("%c",c[i]);
    endl
}
