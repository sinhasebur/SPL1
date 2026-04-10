#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "encryption.h"
#include "modes.h"
#include "authenticationModes.h"
#include "attacks.h"

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
#define gcm 6

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
    int ivbits;
    unsigned char iv[16];

    int ivState,keyState; // 0 is default, 1 is random, -1 is specific
    unsigned char authenticationTag[16];
} command;


void setDefaultValues(struct values* command);
void printError(char * name);
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


void printError(char * x){
    printf("Incorrect Command : %s" , x); endl endl
    printf("Please Follow the following standard"); endl
    printf("./cryptool enc/dec/break -alg  -mode -in -out -key -iv"); endl;
    printf("./cryptool -attack"); endl;
    endl;
    printf("Attacks: patternLeak kpta mitm"); endl;
    endl;
    exit(1);
}


int writeCommand(int comm, struct values* command, char* x){
    //printf(" %s",x); endl;

    if(comm==1){ //algo
        if(!strcmp(x, "AES") || !strcmp(x, "aes")) command->encryp=AES;
        else if(!strcmp(x, "DES") || !strcmp(x, "des") ) command->encryp=DES;
        else if(!strcmp(x, "2DES") || !strcmp(x, "2des") ) command->encryp=_2DES;
        else if(!strcmp(x, "3DES") || !strcmp(x, "3des") ) command->encryp=_3DES;
        else{}
        return 1;
    }
    else if( (comm==0)){ //enc 1,  decrypt 2 , attack 5
        
        if(command->task!=-1){ 
            printError("Multiple Functions given at once");
        }
        else{
            if(!strcmp(x, "enc"))
                command->task=1;
            else if(!strcmp(x, "dec")){
                command->task=2;
            }
            else if(!strcmp(x, "break")){
                command->task=5;
            }
        }
        return 1;
    }
    else if(comm==3){ //input
        command->inputfilename=x;
        return 1;
    }
    else if(comm==4){ // output
        command->outputfilename=x;
        return 1;
    }
    else if(comm==6){ //mode
        if(!strcmp(x, "ecb") || !strcmp(x, "ECB")) command->mode=ecb;
        else if(!strcmp(x, "CBC") || !strcmp(x, "cbc")) command->mode=cbc;
        else if(!strcmp(x, "OFB") || !strcmp(x, "ofb")) command->mode=ofb;
        else if(!strcmp(x, "CFB") || !strcmp(x, "cfb")) command->mode=cfb;
        else if(!strcmp(x, "counter") || !strcmp(x, "Counter")) command->mode=counter;
        else if(!strcmp(x, "GCM") || !strcmp(x, "gcm")) {command->mode=gcm; command->ivbits=96;} 
        else{
            printError("Incorrect Mode name");
        } 
        return 1;
    }
    else if(comm==7){ //key
        if(!strcmp(x, "random")){
            if(command->encryp==AES) command->keybits=128;
            else if(command->encryp==_2DES) command->keybits=128;
            else if(command->encryp==_3DES) command->keybits=192;
            else if(command->encryp==DES) command->keybits=64; 
            
            int keyspace=command->keybits;
            generateRandomKey(command->keybits,keyspace, command->key);
            printf("Generated pseudorandom key"); endl
            command->keyState=1;
        }
        else{
            hextoBits(x,command->key);
            //printf("%d",strlen(x));
            command->keybits=strlen(x)*4;
            command->keyState=-1;
        }
        return 1;
    }
    else if(comm==8){  //iv
  
        if(command->mode==ecb){
            printError("ECB mode does not require iv");
        }
        if(!strcmp(x, "random")){

            if(command->task==2){
                printError("Random used for Decrytion");
            }

            if(command->encryp==AES) command->ivbits=128;
            else if(command->encryp==_2DES) command->ivbits=64;
            else if(command->encryp==_3DES) command->ivbits=64;
            else if(command->encryp==DES) command->ivbits=64; 
            
            int ivSize=(command->ivbits+7)/8;
            int intIV[128];
            char hexIV[32];

            generateRandomKey(ivSize,ivSize, intIV);

            bitstoHex(intIV,hexIV,ivSize);
            hextoBytes(hexIV,command->iv);

            printf("Generated pseudorandom iv"); endl
            command->ivState=1;
        }
        else{
            hextoBytes(x,command->iv);
            //printf("%d",strlen(x));
            command->ivbits=strlen(x)*4;
            command->keyState=-1;
        }
        return 1;
    }
    else if(comm==5){ //attack

        if (command->task!=comm){
            printf("Incorrect use of attack");
        }
        
        if(!strcmp(x, "patternLeak") || !strcmp(x, "pl") || !strcmp(x, "PL")){ command->attackType=1; } 
        else if(!strcmp(x, "knownPlaintextAttack") || !strcmp(x, "KPTA") || !strcmp(x, "kpta")) { 
            command->attackType=1; 
            //command->encryp=DES; 
        }
        else if(!strcmp(x, "meetInTheMiddle") || !strcmp(x, "MITM") || !strcmp(x, "mitm")) {
            command->attackType=3; 
            //command->encryp=_2DES;
        }
        else{ 
            printError("Incorrect Attack Name");
        } 
        
        return 1;
    }

}


void parse(struct values* command, int argc , char* argv[]){
    
    int comm;
    
    writeCommand(0, command, argv[1]);

    for(int i=2; i<argc; i=i+2){
        if(argv[i][0]=='-'){
            comm = commandType(argv[i]);

            if(!writeCommand(comm, command, argv[i+1])){
                printError("Error reading command");
            }
        
        }
        else{
            printError("Incorrect flag use");
        }
    }
}


int commandType(char* x){
    //printf("%s",x);

    if(!strcmp(x, "-alg") || !strcmp(x, "-algo")) return 1;
    if(!strcmp(x, "-dec")) return 2;
    if(!strcmp(x, "-in")) return 3;
    if(!strcmp(x, "-out")) return 4;
    if(!strcmp(x,"-attack")) return 5;
    if(!strcmp(x,"-mode")) return 6;
    if(!strcmp(x,"-key")) return 7;
    if(!strcmp(x,"-iv")) return 8;

    printError("Incorrect Flag used");
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
    command->ivbits=-1;
    
    unsigned char defaultIV[16]= {'a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h'};
    memcpy(command->iv,defaultIV,16*sizeof(unsigned char));

    command->keyState=0; command->ivState=0;
}


void checkValidity (struct values* command){
    if(command->task==-1 ){
        printError("Please have proper arguments");
    }
    
    if(command->encryp==-1 ){
        printError("Please have encryption type");
    }
    

    if(command->keybits!=-1){

        if(command->task==2 && command->keyState==1){
            printError("do not use random for decryption");
        }

        if(command->encryp==AES && command->keybits!=128){
            printError("Incorrect keytype, AES-128 uses 128 bit key");
        }
        else if(command->encryp==DES && command->keybits!=64){
            printError("Incorrect keytype, DES uses 64 bit key");
        }
        else if(command->encryp==_2DES && command->keybits!=128){
            printError("Incorrect keytype, 2DES uses 128 bit key");
        }
        else if(command->encryp==_3DES && command->keybits!=192){
            printError("Incorrect keytype, 3DES uses 192 bit key");
        }
        else{

        }
    }

    if(command->ivbits!=-1){
        
        if(command->task==2 && command->ivState==1){
            printError("Random used in decryption, not valid");
        }
        
        if(command->mode==1 && (command->ivState!=0)){
            printError("ECB mode does not require IV");
        }
        
        if( (command->encryp==AES && command->ivbits!=128 ) && command->mode!=gcm){
            printError("Incorrect iv, AES-128 uses 128 bit iv");
        }
        else if(command->mode==gcm && command->ivState==-1 && command->ivbits!=96){
            printError("AES_GCM uses 96 bit iv");
        }
        else if(command->encryp==DES && command->ivbits!=64){
            printError("Incorrect keytype, DES uses 64 bit iv");
        }
        else if(command->encryp==_2DES && command->ivbits!=64){
            printError("Incorrect keytype, 2DES uses 64 bit iv");
        }
        else if(command->encryp==_3DES && command->ivbits!=64){
            printError("Incorrect keytype, 3DES uses 64 bit iv");
        }
        else{

        }
    }
    
    if(command->task==5){
        if(command->attackType==1){ //pl
            if(command->mode==-1){
                command->mode=1;
            }
        }
    }

    if(command->mode==gcm){
        if(command->encryp!=AES){
            printError("GCM is only for AES");
        }
    }
}



void execute(struct values* command){
    
    int block=getBlockSize(command->encryp);
    char* encr= encNumber(command->encryp);
    
    if(command->task==1){ //encrypt
        if(command->mode==ecb) {ecb_encrypt(block, command->key, command->inputfilename,command->outputfilename, encr);}
        else if(command->mode==cbc){ cbc_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_encrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_encrypt(block, command->key, command->inputfilename, command->outputfilename, command->iv, encr);}
        else if(command->mode==gcm){
            gcm_encrypt(command->key,command->inputfilename, command->iv, command->outputfilename, command->authenticationTag );
        }
    }
    else if(command->task==2){ //decrypt
        if(command->mode==ecb) {ecb_decrypt(block, command->key, command->inputfilename,command->outputfilename, encr);}
        else if(command->mode==cbc){ cbc_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==ofb){ ofb_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==cfb){ cfb_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==counter){ counter_decrypt(block, command->key, command->inputfilename,command->outputfilename, command->iv, encr);}
        else if(command->mode==gcm){
            gcm_decrypt(command->key,command->inputfilename, command->iv, command->outputfilename, command->authenticationTag );
        }
    }
    else if(command->task==5){
        if(command->attackType==1) {
            patternLeak(command->key, command->inputfilename, command->outputfilename,command->mode, command->iv,command->encryp);
        }
    }
}



void printInfo(struct values* command){
    if(command->keyState==1){
        printf("Used key is : ");
        char hex[193];
        bitstoHex(command->key,hex, command->keybits);
        printf("%s",hex);
        endl 
    }
    else if(command->keyState==0){
        printf("Used default key");endl
    }
    else{

    }

    if(command->ivState==1 ){
        printf("Used iv is : "); 
        char hex[33];
        bytetoHex(command->iv,hex,((command->ivbits)+7)/8);
        
        printf("%s",hex);
        endl 
    }
    else if(command->ivState==0){
        printf("Used default iv");endl
    }


    if(command->mode==gcm){
        char hexTag[33];
        
        bytetoHex(command->authenticationTag,hexTag,16);
        
        //printf("%s",hexTag);
        endl
        printf("Authentication Tag is : %s", hexTag);endl
    }
}

