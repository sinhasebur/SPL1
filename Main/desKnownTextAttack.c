#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "modes.h"


#define endl printf("\n");
#define keybits 18
#define keyspace (1U<<keybits)



void desKnownTextAttack(){
    int key[64] = {1 ,1 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };


    FILE *text  = fopen("input", "rb");

    if(!text){
        printf("Input file not found"); endl
        exit(1);
    }
    unsigned char KnownText[8];

    for(int i=0;i<8;i++){
        char x= fgetc(text);
        KnownText[i]=x;
    }
    fseek(text, 0, SEEK_SET);
    fclose(text);

    ecb_encrypt(64,key, "input", "des");


    FILE *entext  = fopen("encrypted", "rb");

    if(!entext){
        printf("Output file not found"); endl
        exit(1);
    }
    
    unsigned char firstCipherBlock[8];

    for(int i=0;i<8;i++){
        char x= fgetc(entext);
        firstCipherBlock[i]=x;
    }
    int guessedKey[64]={0};

    int found=0;

    for(long long i=0;i<keyspace;i++){
        memset(guessedKey, 0, sizeof(guessedKey));
        for(long long j=0;j<keybits;j++){
            guessedKey[j]=(i>>(keybits-1-j))&1;
        }
        unsigned char comp[8];
        DES_Decrypt(firstCipherBlock,guessedKey, comp );
        if(memcmp(comp, KnownText,8)==0){
            printf("Encryption Broken"); endl
            found=1;
            break;
        }
    }

    if(found==1) ecb_decrypt(64, guessedKey, "encrypted", "des");
    
    fclose(entext);
    
}
