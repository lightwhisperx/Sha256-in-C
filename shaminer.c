#include "sha256.h"

int checkWeight(uint8_t *hash, int weight) {
    for (int i = 0; i < weight; i++) {
        int byteIndex = i / 2;
        int nibble = (i % 2 == 0) ? (hash[byteIndex] >> 4) : (hash[byteIndex] & 0x0F);
        if (nibble != 0) return 0;
    }
    return 1;
}

void mine(const char *data, int weight) {
    SHA256_CTX ctx;
    uint8_t hash[32];
    uint32_t nonce = 0;
    char combined[4096];

    printf("Mining weight: %d\n", weight);

    while (1) {
        snprintf(combined, sizeof(combined), "%s%u", data, nonce);

        shainit(&ctx);
        shaupdate(&ctx, (uint8_t *)combined, strlen(combined));
        shafinal(&ctx, hash);

        if (proveriTezinu(hash, weight)) {
            printf("-------------------------------------\n");
            printf("Bitcoin found successfuly!\n");
            printf("Nonce: %u\n", nonce);
            printf("Hash: ");
            ispis(hash);
            printf("-------------------------------------");
            break;
        }

        if (nonce % 100000 == 0) {
            printf("Current nonce: %u\n", nonce);
        }

        printf("Tested hash: ");
        shaprint(hash);

        nonce++;
    }
}