#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SHA_BLOK 32

typedef struct{
	uint8_t data[64];
	uint32_t length;
	uint64_t duzinaBit;
	uint32_t state[8];
} SHA256_CTX;

void shainit(SHA256_CTX *ctx);
void shatransform(SHA256_CTX *ctx, const uint8_t data[]);
void shaupdate(SHA256_CTX *ctx, const uint8_t *data, size_t length);
void shafinal(SHA256_CTX *ctx, uint8_t *hash);
void shaprint(uint8_t hash[]);
int checkWeight(uint8_t *hash, int tezina);
void mine(const char *data, int tezina);

#endif

