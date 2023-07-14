#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bch_codec.h"

//clang -o testBCH BCHtest.c bch_codec.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./testBCH



int cmpfunc(const void *a, const void *b)
{
    return (*(unsigned int *)a - *(unsigned int *)b);
}

int monte_carlo(int m, int t, unsigned int prim_poly, int ntrials)
{
    struct bch_control *bch = init_bch(m, t, prim_poly);
    printf("initialized m=%u n=%u t=%u ecc_bits=%u ecc_bytes=%u\n", bch->m, bch->n, bch->t, bch->ecc_bits, bch->ecc_bytes);

    int N = (1 << bch->m) - 1;
    int msgBits = N - bch->ecc_bits;

    printf("running %d trials of BCH decoding with up to %d errors\n", ntrials, t);
    uint8_t *data = (uint8_t *)malloc(msgBits / 8 * sizeof(uint8_t));
    uint8_t *dataClean = (uint8_t *)malloc(msgBits / 8 * sizeof(uint8_t));
    printf("data.size()=%d\n", msgBits / 8);

    for (int trial = 0; trial < ntrials; ++trial)
    {
        // make a random message
        for (size_t k = 0; k < msgBits / 8; ++k)
            data[k] = rand() & 0xFF;
        memcpy(dataClean, data, msgBits / 8);

        // encode it
        uint8_t *ecc = (uint8_t *)calloc(bch->ecc_bytes, sizeof(uint8_t));
        encode_bch(bch, data, msgBits / 8, ecc);

        // introduce up to t errors
        int nerrs = rand() % (t + 1);
        unsigned int *errLocIn = (unsigned int *)malloc(nerrs * sizeof(unsigned int));
        for (int k = 0; k < nerrs; ++k)
        {
            int i = rand() % (msgBits);
            int byteIndex = i >> 3;
            int bitIndex = i & 7;
            data[byteIndex] ^= (1 << bitIndex);
            errLocIn[k] = i;
        }

        // decode and make sure the right errors were corrected
        unsigned int *errLocOut = (unsigned int *)malloc(t * sizeof(unsigned int));
        int nerrFound = decode_bch(bch, data, msgBits / 8, ecc, NULL, NULL, errLocOut);
        if (nerrFound != nerrs)
        {
            fprintf(stderr, "decode_bch return value=%d expected %d\n", nerrFound, nerrs);
            if (nerrFound < 0)
                fprintf(stderr, "%s\n", strerror(-nerrFound));
            return 1;
        }
        qsort(errLocIn, nerrs, sizeof(unsigned int), cmpfunc);
        qsort(errLocOut, nerrFound, sizeof(unsigned int), cmpfunc);
        if (memcmp(errLocIn, errLocOut, nerrs * sizeof(unsigned int)) != 0)
        {
            fprintf(stderr, "Input Errors!= Found Errors !!!\n");
            return 1;
        }

        correct_bch(bch, data, msgBits / 8, errLocOut, nerrFound);

        if (memcmp(dataClean, data, msgBits / 8) != 0)
        {
            fprintf(stderr, "data not corrected\n");
            return 1;
        }

        free(ecc);
        free(errLocIn);
        free(errLocOut);
    }

    printf("now testing bitwise functions\n");

    data = (uint8_t *)realloc(data, msgBits * sizeof(uint8_t));
    dataClean = (uint8_t *)realloc(dataClean, msgBits * sizeof(uint8_t));
    for (int trial = 0; trial < ntrials; ++trial)
    {
        for (size_t k = 0; k < msgBits; ++k)
            data[k] = rand() & 1;
        memcpy(dataClean, data, msgBits);

        uint8_t *ecc = (uint8_t *)calloc(bch->ecc_bits, sizeof(uint8_t));
        encodebits_bch(bch, data, ecc);

        // introduce up to t errors
        int nerrs = rand() % (t + 1);
        unsigned int *errLocIn = (unsigned int *)malloc(nerrs * sizeof(unsigned int));
        for (int k = 0; k < nerrs; ++k)
        {
            int i = rand() % msgBits;
            data[i] ^= 1;
            errLocIn[k] = i;
        }

        // decode and make sure the right errors were corrected
        unsigned int *errLocOut = (unsigned int *)malloc(t * sizeof(unsigned int));
        int nerrFound = decodebits_bch(bch, data, ecc, errLocOut);
        if (nerrFound != nerrs)
        {
            fprintf(stderr, "decodebits_bch return value=%d expected %d\n", nerrFound, nerrs);
            if (nerrFound < 0)
                fprintf(stderr, "%s\n", strerror(-nerrFound));
            return 1;
        }
        qsort(errLocIn, nerrs, sizeof(unsigned int), cmpfunc);
        qsort(errLocOut, nerrFound, sizeof(unsigned int), cmpfunc);
        if (memcmp(errLocIn, errLocOut, nerrs * sizeof(unsigned int)) != 0)
        {
            fprintf(stderr, "Errors Location mismatch:\n");
            fprintf(stderr, "input errors @ ");
            for (size_t k = 0; k < nerrs; ++k)
                fprintf(stderr, "%u ", errLocIn[k]);
            fprintf(stderr, "\nfound errors @ ");
            for (size_t k = 0; k < nerrFound; ++k)
                fprintf(stderr, "%u ", errLocOut[k]);
            fprintf(stderr, "\n");
        }

        correctbits_bch(bch, data, errLocOut, nerrFound);
        if (memcmp(dataClean, data, msgBits) != 0)
        {
            fprintf(stderr, "data not corrected\n");
            return 1;
        }

        free(ecc);
        free(errLocIn);
        free(errLocOut);
    }

    free(data);
    free(dataClean);
    free_bch(bch);
    return 0;
}

int bit_testvectors()
{
    // from mktestvectors.m
    unsigned int prim_poly = 37;
    const int N = 31;
    const int K = 21;
    const int ntrials = 21;
    int m = 5;
    int t = 2;
    //char msgbits[ntrials][K+1] = {"100000000000000000000", "010000000000000000000", "001000000000000000000", "000100000000000000000", "000010000000000000000", "000001000000000000000", "000000100000000000000", "000000010000000000000", "000000001000000000000", "000000000100000000000", "000000000010000000000", "000000000001000000000", "000000000000100000000", "000000000000010000000", "000000000000001000000", "000000000000000100000", "000000000000000010000", "000000000000000001000", "000000000000000000100", "000000000000000000010", "000000000000000000001", };
    //char eccbits[ntrials][N-K+1] = {"1110110100", "0111011010", "0011101101", "1111000010", "0111100001", "1101000100", "0110100010", "0011010001", "1111011100", "0111101110", "0011110111", "1111001111", "1001010011", "1010011101", "1011111010", "0101111101", "1100001010", "0110000101", "1101110110", "0110111011", "1101101001", };
    char msgbits[ntrials][K + 1] = {"100000000000000000000", "010000000000000000000", "001000000000000000000", "100100000000000000000", "010010000000000000000", "101001000000000000000", "110100100000000000000", "011010010000000000000", "101101001000000000000", "110110100100000000000", "111011010010000000000", "011101101001000000000", "001110110100100000000", "000111011010010000000", "000011101101001000000", "000001110110100100000", "000000111011010010000", "000000011101101001000", "000000001110110100100", "000000000111011010010", "000000000011101101001", };
    char eccbits[ntrials][N - K + 1] = {"1110110100", "0111011010", "0011101101", "0001110110", "0000111011", "0000011101", "0000001110", "0000000111", "0000000011", "0000000001", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", "0000000000", };

    struct bch_control *bch = init_bch(m, t, prim_poly);
    int fails = 0;
    for (int trial = 0; trial < ntrials; ++trial)
    {
        char ecc[N - K + 1];
        memset(ecc, 0, sizeof(ecc));
        encodebits_bch(bch, (const uint8_t *)msgbits[trial], (uint8_t *)ecc);

        for (int i = 0; i < N - K; ++i)
            ecc[i] |= '0';
        printf("message=%s ecc=%s", msgbits[trial], ecc);
        if (strcmp(eccbits[trial], ecc) != 0)
        {
            printf(" WANTED %s", eccbits[trial]);
            ++fails;
        }

        unsigned int errLoc[t];
        int nerr = decodebits_bch(bch, (const uint8_t *)msgbits[trial], (const uint8_t *)eccbits[trial], errLoc);
        if (nerr)
        {
            printf(" nerr=%d", nerr);
        }

        printf("\n");
    }
    free_bch(bch);
    return fails;
}

int main(int argc, char **argv)
{
    bit_testvectors();
    int m = 9;
    int t = 5;
    unsigned int p = 0; // zero indicates to use the appropriate element from prim_poly_tab
    int ntrials = 1000;
    if (argc > 1)
        m = atoi(argv[1]);
    if (argc > 2)
        t = atoi(argv[2]);
    if (argc > 3)
        p = atoi(argv[3]);
    if (argc > 4)
        ntrials = atoi(argv[4]);
    return monte_carlo(m, t, p, ntrials);
}
