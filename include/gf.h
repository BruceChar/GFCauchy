#ifndef _RAID_H
#define _RAID_H
#endif // _RAID_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

uint8_t GF8Add(uint8_t a, uint8_t b);
uint8_t GF8Multiply(uint8_t a, uint8_t b);
uint8_t GF8Invert(uint8_t a);
uint8_t *CauchyMat(uint8_t k, uint8_t m);
uint8_t *ToBitMatrix(uint8_t k, uint8_t m, uint8_t *mat);
uint8_t *uIdMatrix(uint8_t k);
uint8_t *uToBitMatrix(uint8_t m, uint8_t k, uint8_t w, uint8_t *mat);
uint8_t *uGenDecodeMatrix(uint8_t k, uint8_t *erased, uint8_t esize, uint8_t *cmat);
uint8_t *uGenInvertMatrix(uint8_t k, uint8_t *erased, uint8_t esize, uint8_t *cmat);
void BitMat(uint8_t a, uint8_t bM[]);
