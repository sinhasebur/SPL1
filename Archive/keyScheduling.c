#include <stdio.h>
#include <memory.h>

void permutedChoice1(int *x, int *y);
void permutedChoice2(int *x, int *y);

void lcs(int *x); // left circular shift

int main()
{
    int key[64] = {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
    int permuted1key[56];
    permutedChoice1(key, permuted1key);

    int keys[16][48];

    for (int i = 1; i <= 16; i++){
        if (i == 1 || i == 4 || i == 9 || i == 16){

            lcs(permuted1key);


            lcs(permuted1key);
        }
        else{

            lcs(permuted1key);
        }
        int temp[48];
        permutedChoice2(permuted1key, temp);

        memcpy(keys[i - 1], temp, 48 * sizeof(int));
    }

    for (int i = 0; i < 16; i++){
        printf("Key Number %d:\n", i);
        for (int j = 0; j < 48; j++){
            printf("%d ", keys[i][j]);
        }
        printf("\n");
    }
}

void lcs(int *x)
{
    int place = x[55];
    x[55] = 0;
    int temp;
    for (int i = 54; i >= 0; i--){
        temp = x[i];
        x[i] = place;
        place = temp;
    }
}

void permutedChoice1(int *x, int *y)
{

    int p1[56] = {57, 49, 41, 33, 25, 17, 9, 63, 55, 47, 39, 31, 23, 15,
                  1, 58, 50, 42, 34, 26, 18, 7, 62, 54, 46, 38, 30, 22,
                  10, 2, 59, 51, 43, 35, 27, 14, 6, 61, 53, 45, 37, 29,
                  19, 11, 3, 60, 52, 44, 36, 21, 13, 5, 28, 20, 12, 4};

    for (int i = 0; i < 56; i++){
        y[i] = x[p1[i-1]];
        printf("%d ",y[i]);
    }
}

void permutedChoice2(int *x, int *y)
{

    int temp[48];

    int p2[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32};

    for (int i = 0; i < 48; i++){
        y[i] = x[p2[i - 1]];
    }
}
