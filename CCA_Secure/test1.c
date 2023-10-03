
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


//unsigned char* generate_Rvector(const unsigned char* message, int num_bits, int m) {
//    if (m <= 0 || num_bits <= 0 || message == NULL) {
//        printf("Error: Invalid parameters\n");
//        return NULL;
//    }
//
//    unsigned char *r = (unsigned char*)calloc(m, sizeof(unsigned char));
//    unsigned char hash[SHA256_DIGEST_LENGTH];
//    int result_length = 0;
//    int iterations = 0;
//
//    size_t num_bytes = num_bits / 8;
//
//    // 复制原始消息，以便我们可以对其进行修改
//    unsigned char* temp_message = (unsigned char*)malloc(num_bytes);
//    memcpy(temp_message, message, num_bytes);
//
//    while (result_length < m) {
//        // 哈希消息
//        SHA256(temp_message, num_bytes, hash);
//
//        for (int i = 0; i < 32 && result_length < m; i++) {
//            unsigned int value = hash[i];
//            double comparison_value = (double)value / 255.0;
//            r[result_length] = comparison_value < T ? 1 : 0;
//            result_length++;
//        }
//
//        // 使用完整的64字节哈希值作为下一次迭代的输入
//        memcpy(temp_message, hash, SHA256_DIGEST_LENGTH);
//        num_bytes = SHA256_DIGEST_LENGTH;
//        iterations++;
//
//        // 如果迭代次数过多，可能存在问题
//        if (iterations > 10000) {
//            printf("Error: Too many iterations\n");
//            free(temp_message);
//            free(r);
//            return NULL;
//        }
//    }
//
//    free(temp_message);
//
//    return r;
//}

//int main() {
//
//    int m_length = K; // Specify the desired m length in bits = 256 bits
////    unsigned char* m = generate_binary_message(m_length);
////
//    unsigned char *m = (unsigned char *)malloc(m_length * sizeof(unsigned char));
//    generate_binary_vector(m, m_length);
//    print_vector(m, m_length);
//
//    unsigned char* vector_r = generate_Rvector(m, m_length, M);
//
//    printf("r:\n");
//    print_vector(vector_r, M);
//    // 你可以按需使用vector_r
//    free(vector_r);
//    return 0;
//}



unsigned char* generate_binary_message(int v) {
    int num_bytes = v / 8 + (v % 8 != 0);  // Calculate the number of bytes needed

    unsigned char* message = (unsigned char*)malloc(num_bytes);
    if (message == NULL) {
        fprintf(stderr, "Error allocating memory for message.\n");
        return NULL;
    }

    if (RAND_bytes(message, num_bytes) != 1) {
        fprintf(stderr, "Error generating random message.\n");
        free(message); // Free the memory in case of an error
        return NULL;
    } else {
        
        print_binary(message, num_bytes);
        return message;
    }
}



void check_length(unsigned char array[], int expected_length) {
    int count = 0;

    for (int i = 0; i < expected_length; i++) {
        // 增加计数值
        count++;
    }

    printf("Actual length: %d\n", count);
    printf("Expected length: %d\n", expected_length);
    
    assert(count == expected_length);
}




int main() {
    
    seed();
    
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CCA_Secure/Seeds/seed.txt";
    
    unsigned int Seed = read_seed_from_file(seed_file_path);
    printf("Seed from file: %u\n", Seed);
    srand(Seed);
    
    //generate Ber E Z^(m x L)_t;
    unsigned char **E = generateBernoulliMatrix(M,L);
    
    printf("Matrix E:\n");
    //print_matrix(M, L, E);
    
    //ET  L x M
    unsigned char **ET = generateTransposeMatrix(E, M, L);
    printf("Matrix ET:\n");
    //print_matrix(L, M, ET);
    

   
    
    //KEM.Ecap(Pk)
    int m_length = K; // Specify the desired m length in bits = 256 bits
    //    unsigned char* m = generate_binary_message(m_length);
    //
    unsigned char *m = (unsigned char *)malloc(m_length * sizeof(unsigned char));
    generate_binary_vector(m, m_length);
    
    unsigned char *vector_r = generate_Rvector(m, m_length, M);
    
    printf("r:\n");
    print_vector(vector_r, M);
    
    unsigned char *ETr = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (ETr == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    //E^T * r
    matrix_vector_multiplication(ET, L, M, vector_r, ETr);
    printf("ETr:\n");
    print_vector(ETr, L);
    
   

    // Release E
    for (int i = 0; i < M; i++) {
        free(E[i]);
    }
    // Release ET
    for (int i = 0; i < L; i++) {
        free(ET[i]);
    }
    free(E);
    free(ET);
    free(m);
    free(vector_r);
    free(ETr);

    
}
