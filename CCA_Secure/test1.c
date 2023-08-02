
//clang -o test1 test1.c SeedGen.c BerParameter.c bch.c read_files.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//       ./seedtest

// control command e 相同字段全选
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include "bch.h"
#include "config.h"
#include "SeedGen.h"
#include "read_files.h"
#include "BerParameter.h"
#include "read_files.h"
#include "calculation.h"

#define N N_VALUE
#define M M_VALUE
#define T T_VALUE
#define L L_VALUE
#define K K_VALUE


//
//int main() {
//
//    seed();
//    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CCA_Secure/Seeds/seed.txt";
//
//    unsigned int seed = read_seed_from_file(seed_file_path);
//    printf("Seed from file: %u\n", seed);
//    //srand(seed);
//    srand(1234);
//    //KEM.Kgen
//    //generate A Z^(n x m)_2;
//    unsigned char *vectorA = generateBernoulliVector(10);
//
//    printf("vector A:\n");
//
//    print_vector(vectorA, 10);
//    //the vectorAs are different
//
//
//}

//unsigned char* generate_Rvector(const unsigned char* message, int num, int m) {
//
//    if (m <= 128 || message == NULL || num <= 0) {
//        printf("Error: Invalid parameters\n");
//        return NULL;
//    }
//
//    unsigned char *result = (unsigned char*)calloc((m + 7) / 8, sizeof(unsigned char));
//
//
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//
//    unsigned char hash_input[SHA256_DIGEST_LENGTH];
//
//    memcpy(hash_input, message, num / 8);
//    printf("111111\n");
//    int result_length = 0; // 以位为单位的长度
//    while (result_length < m) {
//        SHA256(hash_input, num, hash);
//
//        // 复制所需的位到结果中
//        int to_copy = m - result_length > 128 ? 128 : m - result_length;
//        for (int i = 0; i < to_copy; i++) {
//            if (hash[i / 8] & (1 << (7 - i % 8))) {
//                result[result_length / 8] |= 1 << (7 - result_length % 8);
//            }
//            result_length++;
//        }
//
//        // 如果还需要更多结果，继续处理剩下的128位
//        if (result_length < m) {
//            memcpy(hash_input, hash + 16, 16); // 后128位
//            num = 16; // 后128位的长度
//        }printf("2222222\n");
//    }
//
//    // 将结果除以255.0，并与T进行比较
//    unsigned char *vector_r = (unsigned char*)malloc(m);
//    for (int i = 0; i < m; i++) {
//        double value = (double)(result[i / 8] >> (7 - i % 8) & 1) / 255.0;
//        vector_r[i] = value < T ? 1 : 0;
//    }
//
//    // 打印结果
//    printf("Result:\n");
//    for (int i = 0; i < m; i++) {
//        printf("%d ", vector_r[i]);
//    }
//    printf("\n");
//
//    free(result);
//    return vector_r;
//}

unsigned char* generate_Rvector(const unsigned char* message, int num_bits, int m) {
    if (m <= 0 || num_bits <= 0 || message == NULL) {
        printf("Error: Invalid parameters\n");
        return NULL;
    }

    unsigned char *r = (unsigned char*)calloc(m, sizeof(unsigned char));
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int result_length = 0;
    int iterations = 0;

    size_t num_bytes = num_bits / 8;

    // 复制原始消息，以便我们可以对其进行修改
    unsigned char* temp_message = (unsigned char*)malloc(num_bytes);
    memcpy(temp_message, message, num_bytes);

    while (result_length < m) {
        // 哈希消息
        SHA256(temp_message, num_bytes, hash);

        for (int i = 0; i < 32 && result_length < m; i++) {
            unsigned int value = hash[i];
            double comparison_value = (double)value / 255.0;
            r[result_length] = comparison_value < T ? 1 : 0;
            result_length++;
        }

        // 使用完整的64字节哈希值作为下一次迭代的输入
        memcpy(temp_message, hash, SHA256_DIGEST_LENGTH);
        num_bytes = SHA256_DIGEST_LENGTH;
        iterations++;

        // 如果迭代次数过多，可能存在问题
        if (iterations > 10000) {
            printf("Error: Too many iterations\n");
            free(temp_message);
            free(r);
            return NULL;
        }
    }

    free(temp_message);

    return r;
}

int main() {
    
    int m_length = K; // Specify the desired m length in bits = 256 bits
//    unsigned char* m = generate_binary_message(m_length);
//
    unsigned char *m = (unsigned char *)malloc(m_length * sizeof(unsigned char));
    generate_binary_vector(m, m_length);
    print_vector(m, m_length);

    unsigned char* vector_r = generate_Rvector(m, m_length, M);
    
    printf("r:\n");
    print_vector(vector_r, M);
    // 你可以按需使用vector_r
    free(vector_r);
    return 0;
}
