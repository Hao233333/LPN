

//clang -o testErrors testErrors.c bch_codec.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./testErrors
//
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdint.h>
//#include <time.h>
//#include "bch_codec.h"
//
//#define INFO_SIZE 64 // k
//#define ERROR_T 10 // t
//
//int calculate_m(int n, int t) {
//    int m = 0;
//    while ((1 << m) - 1 < n + m * t) {
//        m++;
//    }
//    return m;
//}
//
//int main()
//{
//    int m = calculate_m(INFO_SIZE, ERROR_T);
//    int n = (1 << m) - 1; // n
//    int i;
//
//    printf("m=%d, n=%d\n", m, n);
//
//    struct bch_control *bch = init_bch(m, ERROR_T, 0);
//    if (!bch) {
//        printf("Cannot initialize BCH.\n");
//        return 1;
//    }
//
//    srand(time(NULL));
//
//    // Allocate memory for original data, encoded data, and error positions
//    uint8_t *data = malloc(sizeof(uint8_t) * INFO_SIZE);
//    uint8_t *encoded = malloc(sizeof(uint8_t) * n);
//    unsigned int *errloc = malloc(sizeof(unsigned int) * ERROR_T);
//
//    // Randomly generate original data
//    for (i = 0; i < INFO_SIZE; i++) {
//        data[i] = rand() & 1;
//    }
//
//    // Encode original data
//    encode_bch(bch, data, INFO_SIZE, encoded);
//
//    // Introduce errors at t random positions
//    for (i = 0; i < ERROR_T; i++) {
//        int pos = rand() % n;
//        encoded[pos] ^= 1;
//        errloc[i] = pos;
//    }
//
//    // Decode and correct errors
//    uint8_t *decoded = malloc(sizeof(uint8_t) * n);
//    int nerr = decode_bch(bch, encoded, n, NULL, decoded, NULL, errloc);
//
//    // Check if the number of corrected errors matches t
//    if (nerr != ERROR_T) {
//        printf("Number of corrected errors does not match t.\n");
//        return 1;
//    }
//
//    // Check if the corrected message matches the original message
//    for (i = 0; i < INFO_SIZE; i++) {
//        if (data[i] != decoded[i]) {
//            printf("Decoded message does not match original message.\n");
//            return 1;
//        }
//    }
//
//    printf("Decoded message matches original message.\n");
//
//    free_bch(bch);
//    free(data);
//    free(encoded);
//    free(errloc);
//    free(decoded);
//
//    return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bch_codec.h"
#include <time.h>





#define BCH_N 127
#define BCH_K 57
#define BCH_M 7
#define BCH_T 10
//
//void print_bits(uint8_t *arr, size_t size)
//{
//    for (int i = 0; i < size; i++)
//    {
//        for (int j = 7; j >= 0; j--)
//        {
//            printf("%d ", (arr[i] & (1 << j)) ? 1 : 0);
//        }
//        printf(" ");
//    }
//    printf("\n");
//}
//
//int main()
//{
//    struct bch_control *bch = init_bch(BCH_M, BCH_T, 0);
//    if (!bch)
//    {
//        printf("Failed to initialize BCH.\n");
//        return 1;
//    }
//
//    uint8_t data[BCH_K / 8] = {0};
//    uint8_t ecc[bch->ecc_bytes];
//
//    for (int i = 0; i < sizeof(data); i++)
//    {
//        data[i] = rand() % 256;
//    }
//
//    printf("BCH_N=%d, BCH_K=%d, BCH_M=%d, BCH_T=%d\n", BCH_N, BCH_K, BCH_M, BCH_T);
//    printf("Original data bits: \n");
//    print_bits(data, sizeof(data));
//
//    encode_bch(bch, data, sizeof(data), ecc);
//
//    printf("ECC: \n");
//    print_bits(ecc, bch->ecc_bytes);
//
//    uint8_t *encoded_data = (uint8_t *)malloc((BCH_N + 7) / 8);
//    memcpy(encoded_data, data, sizeof(data));
//    memcpy(encoded_data + sizeof(data), ecc, bch->ecc_bytes);
//
//    printf("Full encoded data (data + ecc): \n");
//    print_bits(encoded_data, (BCH_N + 7) / 8);
//
//    for (int i = 0; i < BCH_T; i++)
//    {
//        int error_bit_pos = rand() % BCH_N;
//        encoded_data[error_bit_pos / 8] ^= 1 << (error_bit_pos % 8);
//    }
//
//    printf("Data bits after introducing errors: \n");
//    print_bits(encoded_data, (BCH_N + 7) / 8);
//
//    unsigned int errloc[BCH_T];
//    int num_errors = decode_bch(bch, encoded_data, (BCH_N + 7) / 8, NULL, NULL, NULL, errloc);
//
//    printf("Number of errors detected: %d\n", num_errors);
//    if (num_errors == -1)
//    {
//        printf("Decoding failed.\n");
//        free_bch(bch);
//        free(encoded_data);
//        return 1;
//    }
//
//    printf("Errors at positions: \n");
//    for (int i = 0; i < num_errors; i++)
//    {
//        printf("%d ", errloc[i]);
//    }
//    printf("\n");
//
//    for (int i = 0; i < num_errors; i++)
//    {
//        encoded_data[errloc[i] / 8] ^= 1 << (errloc[i] % 8);
//    }
//
//    printf("Data bits after error correction: \n");
//    print_bits(encoded_data, (BCH_N + 7) / 8);
//
//    memcpy(data, encoded_data, sizeof(data));
//
//    printf("Decoded data bits: \n");
//    print_bits(data, sizeof(data));
//
//    printf("Original data bits: \n");
//    print_bits(data, sizeof(data));
//
//
//
//    free_bch(bch);
//    free(encoded_data);
//
//    return 0;
//}

void print_bits(uint8_t *arr, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 7; j >= 0; j--)
        {
            printf("%d ", (arr[i] & (1 << j)) ? 1 : 0);
        }
        printf(" ");
    }
    printf("\n");
}

int main()
{
    struct bch_control *bch = init_bch(BCH_M, BCH_T, 0);
    if (!bch)
    {
        printf("Failed to initialize BCH.\n");
        return 1;
    }

    srand(time(NULL)); // Initialize random number generator with current time

    uint8_t data[BCH_K / 8] = {0};
    uint8_t ecc[bch->ecc_bytes];

    for (int i = 0; i < sizeof(data); i++)
    {
        data[i] = rand() % 256;
    }

    printf("BCH_N=%d, BCH_K=%d, BCH_M=%d, BCH_T=%d\n", BCH_N, BCH_K, BCH_M, BCH_T);
    printf("Original data bits: \n");
    print_bits(data, sizeof(data));

    encode_bch(bch, data, sizeof(data), ecc);

    printf("ECC: \n");
    print_bits(ecc, bch->ecc_bytes);

    uint8_t *encoded_data = (uint8_t *)malloc((BCH_N + 7) / 8);
    memcpy(encoded_data, data, sizeof(data));
    memcpy(encoded_data + sizeof(data), ecc, bch->ecc_bytes);

    printf("Full encoded data (data + ecc): \n");
    print_bits(encoded_data, (BCH_N + 7) / 8);

    int num_errors = 1; // Change this to the desired number of errors

    printf("Data bits after introducing errors: \n");
    print_bits(encoded_data, (BCH_N + 7) / 8);

    unsigned int errloc[BCH_T];
    int num_err_detected = 0;

    for (int i = 0; i < num_errors; i++)
    {
        int error_bit_pos = rand() % BCH_N;

        if (encoded_data[error_bit_pos / 8] & (1 << (error_bit_pos % 8)))
        {
            encoded_data[error_bit_pos / 8] &= ~(1 << (error_bit_pos % 8));
        }
        else
        {
            encoded_data[error_bit_pos / 8] |= (1 << (error_bit_pos % 8));
        }

        num_err_detected = decode_bch(bch, encoded_data, (BCH_N + 7) / 8, NULL, NULL, NULL, errloc);

        if (num_err_detected != -1 && num_err_detected > 0)
        {
            printf("Number of errors detected: %d\n", num_err_detected);
        }
    }

    printf("Data bits after error correction: \n");
    print_bits(encoded_data, (BCH_N + 7) / 8);

    uint8_t decoded_data[BCH_K / 8];
    memset(decoded_data, 0, sizeof(decoded_data));
    memcpy(decoded_data, encoded_data, sizeof(decoded_data));

    printf("Decoded data bits: \n");
    print_bits(decoded_data, sizeof(decoded_data));
    printf("Original data bits: \n");
    print_bits(data, sizeof(data));
    
    
    free_bch(bch);
    free(encoded_data);

    return 0;
}
