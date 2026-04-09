#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "des.h"
#include "2des.h"
#include "3des.h"
#include "modes.h"
#include "mitm.h"
#include "patternLeak.h"
#include "desKPTA.h"



#define endl printf("\n");

#define default 0;

//encryptions
#define DES 1 
#define _2DES 2
#define _3DES 3
#define AES 4

//modes 
#define ecb 1
#define cbc 2
#define cfb 3
#define ofb 4
#define counter 5

//authentication modes 
#define gcm 6;

//attacks
#define MeetInTheMiddle 1
#define KPTA 2
#define PatternLeak 3

struct values{
    int mode;
    int encryp;
    int task; 
    int attackType;

    char* inputfilename, *outputfilename;
    int key[192];
    int keybits;
    unsigned char iv[16];
} command;


void setDefaultValues(struct values* command);
void printError();
void parse(struct values* command, int argc , char* argv[]);
int commandType(char* x);
int writeCommand(int comm, struct values* command, char* x);
void checkValidity(struct values* command);
void execute (struct values* command);




int main(int argc , char* argv[]){
    
    clock_t start, end;
    start = clock();

    
    setDefaultValues(&command);

    parse(&command, argc, argv);
    checkValidity(&command);

    execute(&command);
    

    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);

    endl;
    printf("Time taken is: %f sec.", time_taken); endl;
}


void printError(){
    endl
    printf("Incorrect Command"); endl;
    printf("Please follow the following standard"); endl;
    printf("./driver -enc - dec  -mode -in -out -key -iv"); endl;
    printf("./driver -attack"); endl;
    endl;
    printf("Attacks: patternLeak kpta mitm"); endl;
    endl;
    exit(1);
}


int writeCommand(int comm, struct values* command, char* x){
    //printf(" %s",x); endl;

    if(comm==0){ //default
        if(!strcmp(x, "AES") || !strcmp(x, "aes")) command->encryp=AES;
        else if(!strcmp(x, "DES") || !strcmp(x, "des") ) command->encryp=DES;
        else if(!strcmp(x, "2DES") || !strcmp(x, "2des") ) command->encryp=_2DES;
        else if(!strcmp(x, "3DES") || !strcmp(x, "3des") ) command->encryp=_3DES;
        else{}
    }
    else if( (comm==1) || comm==2 || comm==5){ //enc 1,  decrypt 2 , attack 5
        if(command->task!=-1){
            printf("multiple functions at once"); endl 
            printError();
        }
        else{
            command->task=comm;
            if(comm==5){
                if(!strcmp(x, "patternLeak") || !strcmp(x, "pl") || !strcmp(x, "PL")){ command->attackType=1; } 
                else if(!strcmp(x, "knownPlaintextAttack") || !strcmp(x, "KPTA") || !strcmp(x, "kpta")) { command->attackType=1; command->encryp=DES; }
                else if(!strcmp(x, "meetInTheMiddle") || !strcmp(x, "MITM") || !strcmp(x, "mitm")) {command->attackType=3; command->encryp=_2DES;}
                else{ 
                    printf("incorrect attack name");
                    printError();
                } 
            }
        }
    }
    else if(comm==3){
        command->inputfilename=x;
    }
    else if(comm==4){
        command->outputfilename=x;
    }
    else if(comm=6){
        if(!strcmp(x, "ecb") || !strcmp(x, "ECB")) command->mode=ecb;
        else if(!strcmp(x, "CBC") || !strcmp(x, "cbc")) command->mode=cbc;
        else if(!strcmp(x, "OFB") || !strcmp(x, "ofb")) command->mode=ofb;
        else if(!strcmp(x, "CFB") || !strcmp(x, "cfb")) command->mode=cfb;
        else if(!strcmp(x, "counter") || !strcmp(x, "Counter")) command->mode=counter;
        else if(!strcmp(x, "GCM") || !strcmp(x, "gcm")) {command->mode=gcm;} 
        else{
            printf("Incorrect mode name"); endl
            printError();
        } 
    }
}


void parse(struct values* command, int argc , char* argv[]){
    
    int comm;
    
    for(int i=0; i<argc; i++){
        if(argv[i][0]=='-'){
            comm = commandType(argv[i]);
            // endl
            // printf("%d", comm);

            if(!writeCommand(comm, command, argv[i+1])){
                printError();
            }
            //i++;
            //printf("nice"); endl
        }
        // if(!writeCommand(0, command, argv[i])){
        //     printError();
        // }
    }
}


int commandType(char* x){
    //printf("%s",x);

    if(!strcmp(x, "-enc")) return 1;
    if(!strcmp(x, "-dec")) return 2;
    if(!strcmp(x, "-in")) return 3;
    if(!strcmp(x, "-out")) return 4;
    if(!strcmp(x,"-attack")) return 5;
    if(!strcmp(x,"-mode")) return 6;   

    printf("Incorrect argument");endl
    printError();
}



void setDefaultValues(struct values* command){
    command->mode=-1;
    command->encryp=1;
    command->task=-1;
    command->attackType=0;

    char* in="input";
    command->inputfilename=in;
    
    char* out="output";
    command->outputfilename=out;

    
    int defaultkey[196]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
    memcpy(command->key,defaultkey,192*sizeof(int));

    command->keybits=-1;
    
    unsigned char defaultIV[16]= {'a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h'};
    memcpy(command->iv,defaultIV,16*sizeof(unsigned char));

}


void checkValidity (struct values* command){
    if(command->task==-1 ){
        printf("Please have proper arguments"); endl
        printError();
    }
    
    // if(command->attackType!=0){
    //     if(command->attackType==1 && enc)
    // }
}

char * enc(int i){
    if(i==1) return "des";
    if(i==2) return "2des";
    if(i==3) return "3des";
    if(i==4) return "aes";
}

int blockSize(int i){
    if(i==AES) return 128;
    else return 64;
}

void execute(struct values* command){
    
    int block=blockSize(command->encryp);
    char* encr= enc(command->encryp);
    
    if(command->task==1){ //encrypt
        if(command->mode==ecb) {ecb_encrypt(block, command->key, command->inputfilename, encr);}
        else if(command->mode==cbc){ cbc_encrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_encrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_encrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_encrypt(block, command->key, command->inputfilename, command->iv, encr);}
    }
    else if(command->task==2){ //decrypt
        if(command->mode==ecb) {ecb_decrypt(block, command->key, command->inputfilename, encr);}
        else if(command->mode==cbc){ cbc_decrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_decrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_decrypt(block, command->key, command->inputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_decrypt(block, command->key, command->inputfilename, command->iv, encr);}
    }
    else if(command->task==5){
        if(command->attackType==1) {}
    }
}