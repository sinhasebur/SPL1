#ifndef mitm
#define mitm


typedef struct {
    __uint64_t X;
    __uint64_t key;
} middleEntry;

void swapEntry(middleEntry* a, middleEntry* b);
int partition(middleEntry* a, int low, int high);
void quicksort(middleEntry* a, int low , int high);
void MITM();

#endif