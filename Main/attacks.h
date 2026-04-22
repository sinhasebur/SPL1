#ifndef attack
#define attack

#include "patternLeak.h"
#include "desKPTA.h"
#include "patternLeak.h"

//#include ""
void patternLeak(int * key, char* inputFilename, char* outputFilename, int mode, char* iv , int encryp);
void deskpta(unsigned char* knownText,char* inFilename, char* outFilename, int mode, unsigned char* iv);
void patternLeak(int * key, char* inputFilename, char* outputFilename, int mode, char* iv , int encryp);

#endif