#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "gf.h"

void PintAddList()
{
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            printf("%4d ", GF8Add(i, j));
        }
        printf("\n");
    }
}
void TestAdd(uint8_t a, uint8_t b)
{
    uint8_t sum = GF8Add(a, b);
    printf("%3d + %3d = %3d\n", a, b, sum);
    printf("%3d - %3d = %3d\n", sum, a, GF8Add(sum, a));
    printf("%3d - %3d = %3d\n", sum, b, GF8Add(sum, b));
    
}

uint8_t TestMultiply(uint8_t a, uint8_t b)
{
    uint8_t pro = GF8Multiply(a, b);
    printf("%3d * %3d = %3d\n", a, b, pro);
    return pro;
}

uint8_t TestInvert(uint8_t a)
{
    uint8_t inv = GF8Invert(a);
    printf("%3d^-1 = %3d\n", a, inv);
    printf("%3d * %3d = %3d\n", a, inv, GF8Multiply(a, inv));
    return inv;
}
void TestCMat()
{
	struct timeval ba,bb,ea,eb;
    uint8_t *mat, *bmat, *Bmat;
    srand((unsigned) time(NULL));
    for (int i = 0; i < 1; i++) {
        int k = rand()%6 + 8;
        int m = 12 - k;
        printf("(k,m)=(%d, %d)\n", k, m);
        mat = CauchyMat(m, k);
        if (mat != NULL) {
            for (int j = 0; j < k*m; j++) {
                if (j != 0 && j%k == 0) printf("\n");
                printf("0x%02x ", mat[j]);
            }
            printf("\n");
			gettimeofday(&ba, NULL);
            bmat = ToBitMatrix(k, m, mat);
			gettimeofday(&ea, NULL);
			for (int j = 0; j < 8*k*m; j++) {
                if (j !=0 && j%8 == 0) printf(" ");
                if (j !=0 && j%(8*k) == 0) printf("\n");
                printf("%02x ", bmat[j]);
            }
            printf("\n");
            for (int j = 0; j < m; j++) {
                int c = k*8;
                uint8_t bitMask = 1;
                for (int l = 0; l < 8; l++) {
                    for (int n = 0; n < c; n++) {
                        if (n != 0 && n%8 == 0) printf("  ");
                        uint8_t tmp = *(bmat + c*j + n);
                        printf("%d ", tmp&bitMask?1:0);
                    }
                    bitMask <<= 1;
                    printf("\n");
                }
                printf("\n");
            }
            free(bmat);
			gettimeofday(&bb, NULL);
			Bmat = uToBitMatrix(m, k, 8, mat);
			gettimeofday(&eb, NULL);
			printf("uint bmat:\n");
			for (int i = 0; i < m*8; i++) {
				for (int j = 0; j < k*8; j++) {
					if(j != 0 && j%8 == 0) printf("  ");
					printf("%d ", *(Bmat+i*k*8+j));
				}
				printf("\n");
				if ((i+1)%8 == 0) printf("\n");
			}
			free(Bmat);
        }
        free(mat);
        printf("\n");
    }
    float tia, tib;
    tia = (ea.tv_sec-ba.tv_sec)*1000000+ea.tv_usec-ba.tv_usec;
    tib = (eb.tv_sec-bb.tv_sec)*1000000+eb.tv_usec-bb.tv_usec;
	//printf("Time :\nA-bit:%lf\nB-uint8:%lf", (double)(ea-ba)/CLOCKS_PER_SEC, (double)(eb-bb)/CLOCKS_PER_SEC);
	printf("Time :\nA-bit:%lf\nB-uint8:%lf", tia, tib);
}
static void PrintMat(int m, int k, uint8_t *mat)
{
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; ++j) {
            printf("%3d ", *(mat+i*k+j));
        }
        printf("\n");
    }
}
void TestDecodeMat(uint8_t m, uint8_t k, uint8_t *erased, uint8_t n)
{   
    struct timeval start, end;
    gettimeofday(&start, NULL);
    uint8_t *cmat = CauchyMat(m, k);
    uint8_t *cdmat = uGenDecodeMatrix(k, erased, n, cmat);
    printf("Code Mat:%p\n", cdmat);
    PrintMat(k, k, cdmat);
    uint8_t *dmat = uGenInvertMatrix(k, erased, n, cmat);
    printf("Decode Mat:%p\n", dmat);
    PrintMat(k, k, dmat);
    uint8_t *mat = (uint8_t *)malloc(sizeof(uint8_t)*k*k);
    for (int i = 0; i < k; ++i) {
        int ind = i*k;
        for (int j = 0; j < k; ++j) {
            uint8_t sum = 0;
            for (int l = 0; l < k; ++l) {
                int a = *(cdmat+ind+l), b = *(dmat+l*k+j);
                uint8_t tmp = GF8Multiply(a, b);
                sum = GF8Add(tmp, sum);
            }
            *(mat+i*k+j) = sum;
        }
    }
    gettimeofday(&end, NULL);
    printf("Result Mat:\n");
    PrintMat(k, k, mat);
    float time = (end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec;
    printf("Time: %f\n", time);
    free(cmat);
    free(cdmat);
    free(dmat);
}
int main()
{
    srand((unsigned) time(NULL));
    uint8_t erased[] = {0, 2, 5};
    TestDecodeMat(3, 9, erased, 3);
    //GenMat();
    //FindBestBitMat();
    
    TestCMat();
    #if 0
    uint8_t a[10], b[10];
    for (int i = 0; i < 10; i++) {
        a[i] = rand()%0xff;
        b[i] = rand()%0xff;
        printf("ADD:\n");
        TestAdd(a[i], b[i]);
        printf("MULTIPLY:\n");
        TestMultiply(a[i], b[i]);
        printf("INVERT:\n");
        TestInvert(a[i]);
        TestInvert(b[i]);
    }
    TestMultiply(3,0);
    TestInvert(0);
    #endif
    return 0;
}
