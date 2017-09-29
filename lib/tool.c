#include <stdio.h>
#include <stdlib.h>
#include "raid.h"


void GenMat(uint8_t *mat, uint8_t size)
{
    uint8_t imat[size];
    for (int i = 0; i < size; i++) {
        imat[i] = GF8Invert(mat[i]);
        printf("%02x, ", imat[i]);
    }
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < size; j++) {
            printf("0x%02x, ", GF8Invert(GF8Add(i, imat[j])));
        }
        printf("\n");
    }
}
void FindBestBitMat()
{
    uint8_t bmats[256][8];
    int bitCount[256] = {0};
    for (int i = 0; i < 256; i++) {
        BitMat(i, bmats[i]);
        uint8_t bitMask = 1;
        //printf("%02x :", i);
        for (int j = 0; j < 8; j++) {
            for(int l = 0; l < 8; l++) {
                int bit = bmats[i][l]&bitMask?1:0;
                bitCount[i] += bit;
                //printf("%d ", bit);
            }
            //printf("\n    ");
            bitMask <<= 1;
        }
        //printf("Weight: %d\n\n", bitCount[i]);
    }
    int mats[6][80] = {-1};
    for (int w = 24; w < 30; w++) {
        int c = 0;
        for (int i = 0; i < 256; i++) {
            if (bitCount[i] < w) {
                mats[w-24][c++] = i;
                printf("[%02x:%d] ", mats[w-24][c-1], bitCount[i]);
            }
        }
        printf("\nnumeber of weight<%d: %d\n\n", w, c);
    }
    uint8_t alp[16][16] = {0};
    uint8_t cn[16] = {0};
    for (int i = 0; i < 76; i++) {
        uint8_t s = mats[5][i];
        uint8_t t = s&15;
        if (!(t^1)) {alp[1][cn[1]] = s; cn[1] += 1;}
        if (!(t^2)) {alp[2][cn[2]] = s; cn[2] += 1;}
        if (!(t^3)) {alp[3][cn[3]] = s; cn[3] += 1;}
        if (!(t^4)) {alp[4][cn[4]] = s; cn[4] += 1;}
        if (!(t^5)) {alp[5][cn[5]] = s; cn[5] += 1;}
        if (!(t^6)) {alp[6][cn[6]] = s; cn[6] += 1;}
        if (!(t^7)) {alp[7][cn[7]] = s; cn[7] += 1;}
        if (!(t^8)) {alp[8][cn[8]] = s; cn[8] += 1;}
        if (!(t^9)) {alp[9][cn[9]] = s; cn[9] += 1;}
        if (!(t^10)) {alp[10][cn[10]] = s; cn[10] += 1;}
        if (!(t^11)) {alp[11][cn[11]] = s; cn[11] += 1;}
        if (!(t^12)) {alp[12][cn[12]] = s; cn[12] += 1;}
        if (!(t^13)) {alp[13][cn[13]] = s; cn[13] += 1;}
        if (!(t^14)) {alp[14][cn[14]] = s; cn[14] += 1;}
        if (!(t^15)) {alp[15][cn[15]] = s; cn[15] += 1;}
    }
    for (int i = 0; i < 16; i++) {
        printf("Alp end with %02x:\n\t", i);
        for (uint8_t j = 0; j < 16; j++) {
            #if 0
            uint8_t s = alp[i][j]>>4;
            uint8_t d = (alp[i][j]-alp[i][j-1])>>4;
            uint8_t n = j;
            for (int k = 0; k < d-1; k++) {
                if ((s^n) == 0) {
                    printf("%02x ", alp[i][j]);
                    break;
                } else {
                    n++;
                    printf("-- ");
                }
            }
            #endif
            if (alp[i][j] != 0)
            printf("%02x ", alp[i][j]);
        }
        printf("\n\n");
    }

}

