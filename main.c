#include <math.h>
#include "sha256.h"

#define BUFFER 1024

int main(int argc, char *argv[]) {
    SHA256_CTX ctx;
    uint8_t hash[32];
    char choice[10];

    printf("1. String hash\n");
    printf("2. SHA256sum fajla\n");
    printf("3. BitCoin miner\n");
    printf("> ");
    fgets(choice, 10, stdin);

    choice[strcspn(choice, "\n")] = 0;

    if (strcmp(choice, "1") == 0) {
        char input[2048];
        printf("Enter a string to hash: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        shainit(&ctx);
        shaupdate(&ctx, (uint8_t *)input, strlen(input));
        shafinal(&ctx, hash);

        printf("Hashed string: ");
        shaPrint(hash);
    }
    else if (strcmp(choice, "2") == 0) {
        char path[1024];
        printf("Enter file path: ");
        fgets(path, sizeof(path), stdin);
        path[strcspn(path, "\n")] = 0;

        FILE *f = fopen(path, "rb");
        if (!f) {
            fprintf(stderr, "File error!");
            return 1;
        }

        shainit(&ctx);

        uint8_t buffer[BUFFER];
        size_t bytes;
        while ((bytes = fread(buffer, 1, BUFFER, f)) > 0) {
            shaupdate(&ctx, (uint8_t *)buffer, bytes);
        }

        fclose(f);

        shafinal(&ctx, hash);

        printf("SHA256sum of the file is: ");
        shaPrint(hash);
    }
    else if (strcmp(choice, "3") == 0) {
        char data[1024];
        char temp[10];
        int weight;

        printf("Enter data block: ");
        fgets(temp, sizeof(temp), stdin);
        data[strcspn(data, "\n")] = 0;

        printf("Enter weight: ");
        fgets(temp, sizeof(temp), stdin);
        weight = atoi(temp);

        mine(data, weight);
    }
    else {
        fprintf(stderr, "Pogresan izbor");
    }

    return 0;
}
