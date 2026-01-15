#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "des.h"
#include "2des.h"
#include "3des.h"
#include "modes.h"


#define endl printf("\n");

#define headerSize 54


void patternLeak(){
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
    //int key2[128]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0};

    //unsigned char iv[8]={'a','b','c','d','e','f','g','h'};


    FILE *text  = fopen("input", "rb");

    if(!text){
        printf("Input file not found"); endl
        exit(1);
    }
    unsigned char headerBMP[headerSize];

    for(int i=0;i<headerSize;i++){
        char x= fgetc(text);
        headerBMP[i]=x;
    }
    fseek(text, 0, SEEK_SET);
    fclose(text);

    ecb_encrypt(64,key, "input", "des");


    FILE *entext  = fopen("encrypted", "rb");
    FILE *patternLeak= fopen("patternLeak", "wb");

    if(!entext){
        printf("Output file not found"); endl
        exit(1);
    }

    if(!patternLeak){
        printf("sad");endl
    }

    int n=0,i;

    while((i=fgetc(entext))!=EOF){
        n++;
    }
    fseek(entext, 0, SEEK_SET);
    
    unsigned char* stream = malloc (sizeof(unsigned char)* n);

    unsigned char x;
    for(int i=0;i<n;i++){
        x=fgetc(entext);
        if(i<headerSize){
            fputc(headerBMP[i], patternLeak);
        }
        else{
            fputc(x, patternLeak);
        }
    }

    fclose(entext);
    fclose(patternLeak);

}
