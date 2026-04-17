#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "modes.h"


#define endl printf("\n");
#define keybits 20
#define keyspace (1U<<keybits)
#include "conversions.h"


void deskpta(unsigned char* knownText,char* inFilename, char* outFilename, int mode, unsigned char* iv) {
    //int key[64] = {1 ,1 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };

    FILE *entext  = fopen(inFilename, "rb");

    if(!entext){
        printf("input file \"%s\" not found", inFilename); endl
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
        if(memcmp(comp, knownText,8)==0){
            char hexKey[129];
            bitstoHex(guessedKey,hexKey, 64);
            printf("Encryption Broken, the key was \"%s\"", hexKey); endl
            found=1;
            break;
        }
    }

    if(found==1){
        
        if(mode==1)      ecb_decrypt(64, guessedKey, inFilename, "des", outFilename);
        else if(mode==2) cbc_decrypt(64, guessedKey, inFilename, outFilename,iv, "des");
        else if(mode==3) cfb_decrypt(64, guessedKey, inFilename, outFilename,iv, "des");
        else if(mode==4) ofb_decrypt(64, guessedKey, inFilename, outFilename,iv, "des");
        else if(mode==5) counter_decrypt(64, guessedKey, inFilename, outFilename,iv, "des");
        else{            
            printf("Incorrect Mode sent to mitm attack");
            exit(1);
        }

    }else{
        printf("Key not found, did the key use 20 keyspace?\n");
    }
    
    fclose(entext);
    
}
