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

void printError();

int main(int argc , char* argv[]){
    
    clock_t start, end;
    start = clock();

    int mode=0; //1 is ecb, 2 is cbc, 3 is cfb, 4 is ofb 
    int encryp=0; // 1 is des, 2 is 2des, 3 is 3des ,4 is aes
    int task=0;

    if(argc==2){
        if(strcmp(argv[1],"--mitm")==0){
            MITM();
        }
        else if(strcmp(argv[1],"--patternLeak")==0){
            patternLeak();
        }
        else if (strcmp(argv[1],"--kpta")==0){
            desKnownTextAttack();
        }
        else{
            printError();
        }
    }
    else if(argc==4){
        
        if(strcmp(argv[1],"--encrypt")==0) task=1;
        else if(strcmp(argv[1],"--encrypt+decrypt")==0) task=3;
        else if(strcmp(argv[1],"--decrypt")==0) task=2;

        if((strcmp(argv[2],"--ecb")==0||strcmp(argv[3],"--ecb")==0)&&!mode) mode=1;
        if((strcmp(argv[2],"--cbc")==0||strcmp(argv[3],"--cbc")==0)&&!mode) mode=2;
        if((strcmp(argv[2],"--cfb")==0||strcmp(argv[3],"--cfb")==0)&&!mode) mode=3;
        if((strcmp(argv[2],"--ofb")==0||strcmp(argv[3],"--ofb")==0)&&!mode) mode=4;
        
        if((strcmp(argv[2],"--des")==0||strcmp(argv[3],"--des")==0)&&!encryp) encryp=1;
        if((strcmp(argv[2],"--2des")==0||strcmp(argv[3],"--2des")==0)&&!encryp) encryp=2;
        if((strcmp(argv[2],"--3des")==0||strcmp(argv[3],"--3des")==0)&&!encryp) encryp=3;
        if((strcmp(argv[2],"--aes")==0||strcmp(argv[3],"--aes")==0)&&!encryp) encryp=4;
        
        if(mode==0 || encryp==0 || task==0) printError();

        else{
            unsigned char iv[8]={'a','b','c','d','e','f','g','h'};
            unsigned char iv2[16]={'a','b','c','d','e','f','g','h','a','b','c','d','e','f','g','h'};
            
            int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
            int key2[128]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0};
            int key3[192]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
            
            if(task==1){
                if(mode==1){
                    if(encryp==1) ecb_encrypt(64,key, "input", "des");
                    else if(encryp==3) ecb_encrypt(64,key3, "input", "3des");
                    else if(encryp==4) ecb_encrypt(128,key2, "input", "aes");
                    else ecb_encrypt(64,key2, "input", "2des");
                }
                else if(mode==2){
                    if(encryp==1) cbc_encrypt(64,key, "input",iv, "des");
                    else if(encryp==3) cbc_encrypt(64,key3, "input",iv, "3des");
                    else if(encryp==4) cbc_encrypt(128,key2, "input",iv2, "aes");
                    else cbc_encrypt(64,key2, "input",iv, "2des");
                }
                else if(mode==3){
                    if(encryp==1) cfb_encrypt(64,key, "input",iv, "des");
                    else if(encryp==3) cfb_encrypt(64,key3, "input",iv, "3des");
                    else if(encryp==4) cfb_encrypt(128,key2, "input",iv2, "aes");
                    else cfb_encrypt(64,key2, "input",iv, "2des");
                }
                else if(mode==4){
                    if(encryp==1) ofb_encrypt(64,key, "input",iv, "des");
                    else if(encryp==3) ofb_encrypt(64,key3, "input",iv, "3des");
                    else if(encryp==4) ofb_encrypt(128,key2, "input",iv2, "aes");
                    else ofb_encrypt(64,key2, "input",iv, "2des");
                }
            }
            else if(task==2){
                if(mode==1){
                    if(encryp==1) ecb_decrypt(64,key, "encrypted", "des");
                    else if(encryp==3) ecb_decrypt(64,key3, "encrypted", "3des");
                    else if(encryp==4) ecb_decrypt(128,key2, "input", "aes");
                    else ecb_decrypt(64,key2, "encrypted", "2des");
                }
                else if(mode==2){
                    if(encryp==1) cbc_decrypt(64,key, "encrypted",iv, "des");
                    else if(encryp==3) cbc_decrypt(64,key3, "encrypted",iv, "3des");
                    else if(encryp==4) cbc_decrypt(128,key2, "input",iv2, "aes");
                    else cbc_decrypt(64,key2, "encrypted",iv, "2des");
                }
                else if(mode==3){
                    if(encryp==1) cfb_decrypt(64,key, "encrypted",iv, "des");
                    else if(encryp==3) cfb_decrypt(64,key3, "encrypted",iv, "3des");
                    else if(encryp==4) cfb_decrypt(128,key2, "input",iv2, "aes");
                    else cfb_decrypt(64,key2, "encrypted",iv, "2des");
                }
                else if(mode==4){
                    if(encryp==1) ofb_decrypt(64,key, "encrypted",iv, "des");
                    else if(encryp==3) ofb_decrypt(64,key3, "encrypted",iv, "3des");
                    else if(encryp==4) ofb_encrypt(128,key2, "input", iv2,"aes");
                    else ofb_decrypt(64,key2, "encrypted",iv, "2des");
                }
            }
            else if(task==3){
                if(mode==1){
                    if(encryp==1){
                        ecb_encrypt(64,key, "input", "des");
                        ecb_decrypt(64,key, "encrypted", "des");
                    } 
                    else if(encryp==3){
                        ecb_encrypt(64,key3, "input", "3des");
                        ecb_decrypt(64,key3, "encrypted", "3des");
                    }
                    else if(encryp==4) {
                        ecb_encrypt(128,key2, "input", "aes");
                        ecb_decrypt(128,key2, "encrypted", "aes");
                    } 
                    else {
                        ecb_encrypt(64,key2, "input", "2des");
                        ecb_decrypt(64,key2, "encrypted", "2des");
                    }

                }
                else if(mode==2){
                    if(encryp==1){
                        cbc_encrypt(64,key, "input",iv, "des");
                        cbc_decrypt(64,key, "encrypted",iv, "des");
                    }
                    else if(encryp==3){
                      cbc_encrypt(64,key3, "input",iv, "3des");
                      cbc_decrypt(64,key3, "encrypted",iv, "3des");  
                    }
                    else if(encryp==4) {
                        cbc_encrypt(128,key2, "input",iv2, "aes");
                        cbc_decrypt(128,key2, "encrypted",iv2, "aes");
                    }
                    else{
                        cbc_encrypt(64,key2, "input",iv, "2des");
                        cbc_decrypt(64,key2, "encrypted",iv, "2des");
                    } 
                }
                else if(mode==3){
                    if(encryp==1){
                        cfb_encrypt(64,key, "input",iv, "des");
                        cfb_decrypt(64,key, "encrypted",iv, "des");
                    }
                    else if(encryp==3){
                      cfb_encrypt(64,key3, "input",iv, "3des");
                      cfb_decrypt(64,key3, "encrypted",iv, "3des");  
                    }
                    else if(encryp==4) {
                        cfb_encrypt(128,key2, "input",iv2, "aes");
                        cfb_decrypt(128,key2, "encrypted",iv2, "aes");
                    }
                    else{
                        cfb_encrypt(64,key2, "input",iv, "2des");
                        cfb_decrypt(64,key2, "encrypted",iv, "2des");
                    } 
                }
                else if(mode==4){
                    if(encryp==1){
                        ofb_encrypt(64,key, "input",iv, "des");
                        ofb_decrypt(64,key, "encrypted",iv, "des");
                    }
                    else if(encryp==3){
                      ofb_encrypt(64,key3, "input",iv, "3des");
                      ofb_decrypt(64,key3, "encrypted",iv, "3des");  
                    }
                    else if(encryp==4) {
                        ofb_encrypt(128,key2, "input", iv2, "aes");
                        ofb_decrypt(128,key2, "encrypted",iv2, "aes");
                    }
                    else{
                        ofb_encrypt(64,key2, "input",iv, "2des");
                        ofb_decrypt(64,key2, "encrypted",iv, "2des");
                    } 
                }
            }

        }
    
    
    
    
    }
    else{
        printError();
    }

    end = clock();

    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);

    endl;
    printf("Time taken is: %f sec.", time_taken); endl;
}


void printError(){
    endl
    printf("Incorrect Command"); endl;
    printf("Please follow the following standard"); endl;
    printf("./driver --(encrypt | decrypt | encrypt+decrypt ) --mode --encryption"); endl;
    printf("./driver --attack"); endl;
    endl;
    printf("Attacks: --patternLeak --kpta --mitm"); endl;
    endl;
    exit(1);
}