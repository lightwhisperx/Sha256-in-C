#include "sha256.h"


#define ROTDESNO(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#define CH(x,y,z)  (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x)     (ROTDESNO(x,2) ^ ROTDESNO(x,13) ^ ROTDESNO(x,22))
#define EP1(x)     (ROTDESNO(x,6) ^ ROTDESNO(x,11) ^ ROTDESNO(x,25))
#define SIG0(x)    (ROTDESNO(x,7) ^ ROTDESNO(x,18) ^ ((x) >> 3))
#define SIG1(x)    (ROTDESNO(x,17) ^ ROTDESNO(x,19) ^ ((x) >> 10))

static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void shainit(SHA256_CTX *ctx) {
    ctx->length = 0;
    ctx->duzinaBit = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

void shatransform(SHA256_CTX *ctx, const uint8_t data[]) {
    uint32_t a, b, c, d, e, f, g, h, t1, t2, m[64];

    for (uint32_t i = 0, j = 0; i < 16; ++i, j += 4) {
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    }

    for (uint32_t i = 16; i < 64; ++i) {
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
    }

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (uint32_t i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

void shaupdate(SHA256_CTX *ctx, const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        ctx->data[ctx->length] = data[i];
        ctx->length++;
        if (ctx->length == 64) {
            shatransform(ctx, ctx->data);
            ctx->duzinaBit += 512;
            ctx->length = 0;
        }
    }
}

void shafinal(SHA256_CTX *ctx, uint8_t *hash) {
    uint32_t i = ctx->length;

    if (ctx->length < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56) {
            ctx->data[i++] = 0x00;
        }
    }
    else {
        ctx->data[i++] = 0x80;
        while (i < 64) {
            ctx->data[i++] = 0x00;
        }
        shatransform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    ctx->duzinaBit += ctx->length * 8;
    ctx->data[56] = (ctx->duzinaBit >> 56) & 0xff;
    ctx->data[57] = (ctx->duzinaBit >> 48) & 0xff;
    ctx->data[58] = (ctx->duzinaBit >> 40) & 0xff;
    ctx->data[59] = (ctx->duzinaBit >> 32) & 0xff;
    ctx->data[60] = (ctx->duzinaBit >> 24) & 0xff;
    ctx->data[61] = (ctx->duzinaBit >> 16) & 0xff;
    ctx->data[62] = (ctx->duzinaBit >> 8) & 0xff;
    ctx->data[63] = ctx->duzinaBit & 0xff;
    shatransform(ctx, ctx->data);

    for (i = 0; i < 4; ++i) {
        for (uint32_t j = 0; j < 8; ++j) {
            hash[i + j * 4] = (ctx->state[j] >> (24 - i * 8)) & 0x000000ff ;
        }
    }
}

void shaPrint(uint8_t hash[]) {
    for (int i = 0; i < 32; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}
