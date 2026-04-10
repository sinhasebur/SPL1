#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption.h"
#include "modes.h"


#define endl printf("\n");

#define HEADERSIZE 54


void patternLeak(int * key, char* inputFilename, char* outputFilename, int mode, char* iv , int encryp)
{
    
    FILE *text = fopen(inputFilename, "rb");

    if(!text){
        printf("Input file not found, please have %s", inputFilename); endl
        exit(1);
    }

    unsigned char headerBMP[HEADERSIZE];

    for(int i = 0; i < HEADERSIZE; i++){
        char x = fgetc(text);
        headerBMP[i] = x;
    }

    fseek(text, 0, SEEK_SET);
    fclose(text);

    //ecb_encrypt(128, key2, "input", "aes");

    if(mode==1){
        printf("Showing pattern Leak in ECB\n");
        ecb_encrypt(getBlockSize(encryp),key,inputFilename,"Encrypted", encNumber(encryp));
    }
    else{
        printf("Other modes than ECB do not leak pattern\n");
        if(mode==2){
            cbc_encrypt(getBlockSize(encryp),key,inputFilename, "Encrypted",iv, encNumber(encryp));
        }
        else if(mode==3){
            cfb_encrypt(getBlockSize(encryp),key,inputFilename, "Encrypted",iv, encNumber(encryp));
        }
        else if(mode==4){
            ofb_encrypt(getBlockSize(encryp),key,inputFilename, "Encrypted",iv, encNumber(encryp));
        }
        else if(mode==5){
            counter_encrypt(getBlockSize(encryp),key,inputFilename, "Encrypted",iv, encNumber(encryp));
        }
    }
    
    FILE *entext      = fopen("Encrypted", "rb");
    FILE *patternLeak = fopen(outputFilename, "wb");

    if(!entext){
        printf("Output file not found"); endl
        exit(1);
    }

    if(!patternLeak){
        printf("outputfilename error");endl
    }

    int n=0,i;

    while((i = fgetc(entext)) != EOF){
        n++;
    }
    fseek(entext, 0, SEEK_SET);
    
    unsigned char* stream = malloc(sizeof(unsigned char)* n);

    unsigned char x;
    for (int i = 0; i < n; i++) {
        x = fgetc(entext);
        if(i < HEADERSIZE){
            fputc(headerBMP[i], patternLeak);
        }
        else {
            fputc(x, patternLeak);
        }
    }

    fclose(entext);
    fclose(patternLeak);

}
