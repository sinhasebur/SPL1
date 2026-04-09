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

#include "conversions.h"
#include "randomKey.h"




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
void printInfo(struct values* command);



int main(int argc , char* argv[]){
    
    clock_t start, end;
    start = clock();

    
    setDefaultValues(&command);

    parse(&command, argc, argv);
    checkValidity(&command);

    execute(&command);

    printInfo(&command);
    

    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);

    endl;
    printf("Time taken is: %f sec.", time_taken); endl;
}


void printError(){
    endl
    printf("Incorrect Command"); endl;
    printf("Please follow the following standard"); endl;
    printf("./cryptool -enc - dec  -mode -in -out -key -iv"); endl;
    printf("./cryptool -attack"); endl;
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
    else if(comm==6){
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
    else if(comm==7){ //key
        if(!strcmp(x, "random")){

            if(command->task==2){
                printf("Please do not use random for decryption"); endl printError();
            }
            
            if(command->encryp==AES) command->keybits=128;
            else if(command->encryp==_2DES) command->keybits=128;
            else if(command->encryp==_3DES) command->keybits=192;
            else if(command->encryp==DES) command->keybits=64; 
            
            int keyspace=command->keybits;
            generateRandomKey(command->keybits,keyspace, command->key);
            printf("Generated pseudorandom key"); endl
        }
        else{
            hextoBits(x,command->key);
            //printf("%d",strlen(x));
            command->keybits=strlen(x)*4;
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
        else{
            writeCommand(0, command, argv[i]);
        }
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
    if(!strcmp(x,"-key")) return 7;   

    printf("Incorrect argument");endl
    printError();
}



void setDefaultValues(struct values* command){
    command->mode=-1;
    command->encryp=-1;
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
    
    if(command->encryp==-1 ){
        printf("Please have encryption type"); endl
        printError();
    }
    

    if(command->keybits!=-1){
        if(command->encryp==AES && command->keybits!=128){
            printf("Incorrect keytype, AES-128 uses 128 bit key"); endl
            printError();
        }
        else if(command->encryp==DES && command->keybits!=64){
            printf("Incorrect keytype, DES uses 64 bit key"); endl
            printError();
        }
        else if(command->encryp==_2DES && command->keybits!=128){
            printf("Incorrect keytype, 2DES uses 128 bit key"); endl
            printError();
        }
        else if(command->encryp==_3DES && command->keybits!=192){
            printf("Incorrect keytype, 3DES uses 192 bit key"); endl
            printError();
        }
        else{

        }
    }



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
        if(command->mode==ecb) {ecb_encrypt(block, command->key, command->inputfilename,command->outputfilename, encr);}
        else if(command->mode==cbc){ cbc_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_encrypt(block, command->key, command->inputfilename, command->outputfilename, command->iv, encr);}
    }
    else if(command->task==2){ //decrypt
        if(command->mode==ecb) {ecb_decrypt(block, command->key, command->inputfilename,command->outputfilename, encr);}
        else if(command->mode==cbc){ cbc_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
    }
    else if(command->task==5){
        if(command->attackType==1) {}
    }
}



void printInfo(struct values* command){
    if(command->keybits!=-1){
        printf("Used key is : ");
        char hex[193];
        bitstoHex(command->key,hex, command->keybits);
        printf("%s",hex);
        endl 
    }
}