#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "2des.h"
#include "modes.h"


#define endl printf("\n");

#define keybits 16
#define keyspace (1U<<keybits)


typedef struct {
    __uint64_t X;
    __uint16_t key;
} middleEntry;


void swapEntry(middleEntry* a, middleEntry* b);
int partition(middleEntry* a, int high, int low);
void quicksort(middleEntry* a, int high , int low);


int main(){
    
    int key2[128]={1 ,1 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0, 0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };

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

    ecb_encrypt(64,key2, "input", "2des");


    FILE *entext  = fopen("encrypted", "rb");

    FILE *mid1  = fopen("middle1", "rb");

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

    middleEntry* givesKey2=malloc(sizeof(middleEntry)*keyspace);

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
            pX=(pX<<8) |comp[p];
        }
        
        givesKey1[i].X=pX;
    }

    quicksort(givesKey1,0, keyspace-1);
    quicksort(givesKey2,0, keyspace-1);

    __uint16_t k1, k2;
    int found=0;

    int finalGuessedKey[128]={0};


    for(int i=0;i<keyspace;i++){
        __uint64_t x= givesKey1[i].X;
        int l=0, r=keyspace-1;
        
        while(l<=r){
            long long mid= l +(r-l)/2;
            if(givesKey2[mid].X==x){
                k2=givesKey2[mid].key;
                k1=givesKey1[i].key;
                printf(".");endl; 

                int guessedKey1[64]={0};
                int guessedKey2[64]={0};
                for(int i=keybits-1; i>=0;i-- ){
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
                    printf("Found both keys"); endl 
                    found=1;  
                    for(int j=0;j<64;j++) finalGuessedKey[j]=guessedKey1[j];
                    for(int j=0;j<64;j++) finalGuessedKey[64+j]=guessedKey2[j];
                }
                else {printf("Error"); endl}
                break;
            }
            else if(givesKey2[mid].X>x){
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

    if(found==1){
    }

    ecb_decrypt(64, finalGuessedKey, "encrypted", "2des");

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

