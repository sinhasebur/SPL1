#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "2des.h"
#include "modes.h"
#include "conversions.h"

#define endl printf("\n");

#define keybits 20
#define keyspace (1U<<keybits)


typedef struct {
    __uint64_t X;
    __uint64_t key;
} middleEntry;


void swapEntry(middleEntry* a, middleEntry* b);
int  partition(middleEntry* a, int low, int high);
void quicksort(middleEntry* a, int low , int high);


void MITM(char* inFile, char* KnownText, char* outFile , int mode, char* iv) 
{    

    FILE *entext = fopen(inFile, "rb");

    if(!entext){
        printf("Input file \"%s\" not found", inFile); endl
        exit(1);
    }
    
    unsigned char firstCipherBlock[8];

    for(int i = 0; i < 8; i++){
        char x = fgetc(entext);
        firstCipherBlock[i] = x;
    }

    int guessedKey[64] = {0};

    middleEntry *givesKey2 = malloc(sizeof(middleEntry) * keyspace);

    for(long long i=0;i<keyspace;i++){
        memset(guessedKey, 0, sizeof(guessedKey));
        for(long long j=0;j<keybits;j++){
            guessedKey[j]=(i>>(keybits-1-j))&1;
        }
        unsigned char comp[8];
        DES_Decrypt(firstCipherBlock,guessedKey, comp ); 
        givesKey2[i].key=i;
        
        __uint64_t pX=0;

        for(int p=0;p<8;p++){
            pX=(pX<<8) |comp[p];
        }
        
        givesKey2[i].X=pX;
    }

    middleEntry* givesKey1=malloc(sizeof(middleEntry)*keyspace);

    for(long long i=0;i<keyspace;i++){
        memset(guessedKey, 0, sizeof(guessedKey));
        for(long long j=0;j<keybits;j++){
            guessedKey[j]=(i>>(keybits-1-j))&1;
        }
        unsigned char comp[8];
        DES_Encrypt(KnownText,guessedKey, comp );
        givesKey1[i].key=i;
        
        __uint64_t pX=0;

        for(int p=0;p<8;p++){
            pX = (pX<<8) | comp[p];
        }
        
        givesKey1[i].X=pX;
    }

    quicksort(givesKey1, 0, keyspace - 1);
    quicksort(givesKey2, 0, keyspace - 1);

    __uint64_t k1, k2;
    int found=0;

    int finalGuessedKey[128]={0};


    for(int i=0;i<keyspace;i++){
        __uint64_t x= givesKey1[i].X;
        int l=0, r=keyspace-1;
        
        while(l<=r){

            long long mid= l +(r-l)/2;
            
            if (givesKey2[mid].X==x) {
                k2=givesKey2[mid].key;
                k1=givesKey1[i].key;
                printf(".");endl; 

                int guessedKey1[64]={0};
                int guessedKey2[64]={0};

                for(int i=keybits-1; i >= 0; i-- ){
                    guessedKey1[i]=k1&1;
                    k1>>=1;
                }

                for(int i=keybits-1; i>=0;i-- ){
                    guessedKey2[i]=k2&1;
                    k2>>=1;
                }

                unsigned char cmp[8];
                _2DES_Decrypt(firstCipherBlock,guessedKey1, guessedKey2, cmp);
                if(memcmp(cmp, KnownText, 8)==0) { 
                    printf("Found both keys, decrypting now"); endl
                    printf("The total key was "); 
                    found=1;  

                    for(int j=0;j<64;j++) finalGuessedKey[j]=guessedKey1[j];
                    for(int j=0;j<64;j++) finalGuessedKey[64+j]=guessedKey2[j];
                    
                    char finalHexKey[257];
                    bitstoHex(finalGuessedKey, finalHexKey, 128);

                    printf("%s", finalHexKey);endl
                }
                else {printf("Error"); endl}
                break;
            }
            else if (givesKey2[mid].X>x) {
                r=mid-1;
            }
            else{
                l=mid+1;
            }
        }

        if(found==1){
            break;
        }

    }

    free(givesKey1);
    free(givesKey2);

    if(mode==1){
        ecb_decrypt(64, finalGuessedKey, inFile, outFile, "2des");
    }
    else if(mode==2){
        cbc_decrypt(64, finalGuessedKey, inFile, outFile,iv, "2des");
    }
    else if(mode==3){
        cfb_decrypt(64, finalGuessedKey, inFile, outFile,iv, "2des");
    }
    else if(mode==4){
        ofb_decrypt(64, finalGuessedKey, inFile, outFile,iv, "2des");
    }
    else if(mode==5){
        counter_decrypt(64, finalGuessedKey, inFile, outFile,iv, "2des");
    }
    else{
        printf("Incorrect Mode sent to mitm attack");
        exit(1);
    }
    

    fclose(entext);

}

void swapEntry(middleEntry* a, middleEntry* b){
    middleEntry temp=*a;
    *a=*b;
    *b=temp;
}


void quicksort(middleEntry* a, int low , int high){
    if(low<high){
        int pivotIndex= partition(a, low,high);

        quicksort(a, low, pivotIndex-1);
        quicksort(a, pivotIndex+1, high);
    }
}

int partition(middleEntry a[], int low, int high) {
    __uint64_t pivot=a[high].X;
    int i =low-1;

    for (int j=low; j<high; j++) {
        if (a[j].X <pivot) {
            i++;
            swapEntry(&a[i], &a[j]);
        }
    }
    swapEntry(&a[i + 1], &a[high]);
    return i + 1;
}

