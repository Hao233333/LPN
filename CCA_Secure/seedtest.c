
//clang -o seedtest seedtest.c SeedGen.c BerParameter.c bch.c read_files.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//       ./seedtest

// control command e 相同字段全选
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include "bch.h"
#include "SeedGen.h"
#include "config.h"
#include "read_files.h"
#include "BerParameter.h"
#include "read_files.h"
#include "calculation.h"

#define N N_VALUE
#define M M_VALUE
//#define T T_VALUE
#define L L_VALUE
#define K K_VALUE


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

unsigned int sha256t_hash32(unsigned char *data, size_t len) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(data, len, hash);
    
    // Use the first 32 bits of the hash as the result
    unsigned int hash32 = *(unsigned int *)hash;
    return hash32;
}

void check_length(unsigned char* vector, int expected_length) {
    assert(vector != NULL);  // Check that the vector is not NULL

    // Calculate the length of the vector
    int actual_length = strlen((char*)vector);
    printf("Actual length: %d\n", actual_length);
        printf("Expected length: %d\n", expected_length);
    // If the actual length is not the same as the expected length, assert will terminate the program
    assert(actual_length == expected_length);
}




int main() {
    
    seed();
    
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CCA_Secure/Seeds/seed.txt";
    
    unsigned int Seed = read_seed_from_file(seed_file_path);
    printf("Seed from file: %u\n", Seed);
    srand(Seed);
    
    //KEM.Kgen
    //generate A Z^(n x m)_2;
    unsigned char **matrixA = generateBinaryMatrix(N, M);
    
    // print matrix
    printf("Matrix A:\n");
    //print_matrix(N, M, matrixA);
    
    //generate S Z^(n x l)_2;
    unsigned char **matrixS = generateBinaryMatrix(N, L);
    
    // print matrix
    printf("Matrix S:\n");
    //print_matrix(N, L, matrixS);
    
    
    //generate Ber E Z^(m x L)_t;
    unsigned char **E = generateBernoulliMatrix(M,L);
    
    printf("Matrix E:\n");
     //print_matrix(M, L, E);
    
    //ET  L x M
    unsigned char **ET = generateTransposeMatrix(E, M, L);
    printf("Matrix ET:\n");
    //print_matrix(L, M, ET);
    
    //ST L x N
    unsigned char **ST = generateTransposeMatrix(matrixS, N, L);
    printf("Matrix ST:\n");
    //print_matrix(L, N, ST);
    
    //S^T A L行M列
    unsigned char **STA = multiply_matrices(ST, L, N, matrixA, N, M);
    
    printf("Matrix STA:\n");
    //print_matrix(L, M, STA);

    
    //B^T L行M列
    unsigned char **BT = (unsigned char **)malloc(L * sizeof(unsigned char *));
    if (BT == NULL) {
        printf("Failed to allocate memory for BT\n");
        exit(1);
    }
        //else{printf("pass:\n");}
    ;
    
    for (int i = 0; i < L; i++) {
        BT[i] = (unsigned char *)malloc(M * sizeof(unsigned char));
        if (BT[i] == NULL) {
            printf("Failed to allocate memory for BT[%d]\n", i);
            exit(1);
        }
    }
    
    //STA L M, ET四列八行，+ BT 八行四列
    add_matrices(STA, ET, BT, L, M);
//    printf("Matrix BT:\n");
    //print_matrix(L, M, BT);
    //B
    //unsigned char **B = generateTransposeMatrix(BT, L, M);
    //printf("Matrix B:\n");
    //print_matrix(M, L, B);
    

    //KEM.Ecap(Pk)
    int m_length = K; // Specify the desired m length in bits = 256 bits
//    unsigned char* m = generate_binary_message(m_length);
//
    unsigned char *m = (unsigned char *)malloc(m_length * sizeof(unsigned char));
    generate_binary_vector(m, m_length);
    
    unsigned char *vector_r = generate_Rvector(m, m_length, M);
    
    printf("r:\n");
    print_vector(vector_r, M);
    
    //encode message
    int i;
    extern int length, k, t, d;
    extern int p[21];
    extern int alpha_to[1048576], index_of[1048576], g[548576];
    extern int recd[1048576], data[1048576], bb[548576];
    extern int numerr, errpos[1024], decerror;
    read_p();               /* Read m */
    generate_gf();          /* Construct the Galois Field GF(2**m) */
    gen_poly();             /* Compute the generator polynomial of BCH code */
    
    /* to get encoded message  but how many bits ?????????????????? I define it as L*/
    unsigned char *encodedMessage = encode_bch(m);
    
    printf("r(x) = (this is encoded message )\n");
    for (i = 0; i < length; i++) {
        printf("%1d ", recd[i]);
//        if (i && (((i+1) % 8) == 0))
//            printf(" ");
    }
    printf("\n");
    
//    printf("encoded Message is:\n");
//
//    print_vector(encodedMessage, L);
    

    
    //calculate C0 = Ar
    
    unsigned char *C0 = (unsigned char *)malloc(N * sizeof(unsigned char));
    if (C0 == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    matrix_vector_multiplication(matrixA, N, M, vector_r, C0);
    
    
    //Calculate B^T*r B^T L行M列
    unsigned char *BTr = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (BTr == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    
    matrix_vector_multiplication(BT, L, M, vector_r, BTr);
    
    //C1 = B^Tr + encodedMessage, length = L
    unsigned char *C1 = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (C1 == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    
    add_vectors(BTr,encodedMessage, C1, L);
    printf(" BTr is:\n");
    
    print_vector(BTr, L);
    
    printf("C1 is:\n");
    
    print_vector(C1, L);
    
    //加一个常数sha256对64个0的参数-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //unsigned char *LM = append_zeros(m,m_length,64);
    
    // Use SHA-256 to hash Lm =  H'(m)
    unsigned char C2[SHA256_DIGEST_LENGTH];
    SHA256(m, K / 8, C2);

    printf("SHA-256 C2: \n");
    print_binary(C2, SHA256_DIGEST_LENGTH);
    
    // Use SHA-256 to hash Km =  H(m) session key
    unsigned char KM[SHA256_DIGEST_LENGTH];
    SHA256(m, m_length / 8, KM);

    printf("SHA-256 K: \n");
    print_binary(KM, SHA256_DIGEST_LENGTH);
    
    
    
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //KEY Decap
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // Function to check the length of a binary vector
//    printf("C0 \n");
//    check_length(C0, N);
//    printf("C1 \n");
//    check_length(C1, L);
//    printf("C2 \n");
//    check_length(C2, SHA256_DIGEST_LENGTH);
    
    //m ← Decode(C1 + S⊺C0）
    
    //S^T C0 matrix x vector - Length L
    unsigned char *STC0 = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (STC0 == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    matrix_vector_multiplication(ST, L, M, C0, STC0);
    
    //C1 + S⊺C0 length L
    unsigned char *receivedM = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (receivedM == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    add_vectors(C1,STC0, receivedM, L);
    
    
    printf("received Message is:\n");
    
    print_vector(receivedM, L);
    
    
    //decode received message
    int errorCount;
    unsigned char *decodedM = decode_bch(receivedM, L,&errorCount);
    
    if (decodedM == NULL) {
        printf("Decoding failed.\n");
    }
    
    printf("Original Message is:\n");
    print_vector(m, m_length);
    
    printf("Decoded Message is:\n");
    print_vector(decodedM, m_length);
    
    printf("Error count: %d\n", errorCount);
    

    
    unsigned char *vector_r2 = generate_Rvector(decodedM, m_length, M);
    
    
    printf("vector r \n");
    print_vector(vector_r, M);
    
    printf("vector r' \n");
    print_vector(vector_r2, M);

    i = 0;
//    extern int length, k, t, d;
//    extern int p[21];
//    extern int alpha_to[1048576], index_of[1048576], g[548576];
//    extern int recd[1048576], data[1048576], bb[548576];
//    extern int numerr, errpos[1024], decerror;
    read_p();               /* Read m */
    generate_gf();          /* Construct the Galois Field GF(2**m) */
    gen_poly();             /* Compute the generator polynomial of BCH code */
    
    /* to get encoded message  but how many bits ?????????????????? I define it as L*/
    unsigned char *encodedM_2nd = encode_bch(decodedM);
    
    printf("r(x) = (this is re-encoded message )\n");
    for (i = 0; i < length; i++) {
        printf("%1d ", recd[i]);
//        if (i && (((i+1) % 8) == 0))
//            printf(" ");
    }
    printf("\n");
    
    //calculate C0' = Ar'
    
    unsigned char *C0_2nd = (unsigned char *)malloc(N * sizeof(unsigned char));
    if (C0_2nd == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    matrix_vector_multiplication(matrixA, N, M, vector_r2, C0_2nd);
    
    
    
    
    //Calculate B^T*r B^T L行M列
    unsigned char *BTr2 = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (BTr2 == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    
    matrix_vector_multiplication(BT, L, M, vector_r2, BTr2);
    
    //C1 = B^Tr + encodedMessage, length = L
    unsigned char *C1_2nd = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (C1_2nd == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    
    add_vectors(BTr2,encodedM_2nd, C1_2nd, L);
    
    printf("C1' is:\n");
    
    print_vector(C1_2nd, L);
    
    printf("C1 is:\n");
    
    print_vector(C1, L);
    
    //加一个常数sha256对64个0的参数-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    //unsigned char *LM2 = append_zeros(decodedM,m_length,64);
    
    // Use SHA-256 to hash Lm2 =  H'(m)
    unsigned char C2_2nd[SHA256_DIGEST_LENGTH];
    SHA256(decodedM, m_length / 8, C2_2nd);

    printf("SHA-256 C2': \n");
    print_binary(C2_2nd, SHA256_DIGEST_LENGTH);
    
    printf("SHA-256 C2: \n");
    print_binary(C2, SHA256_DIGEST_LENGTH);
    
    
    //compare values
    
    // Compare C2 and C2_2nd
    if (memcmp(C2, C2_2nd, SHA256_DIGEST_LENGTH) != 0) {
        printf("Decryption unsuccessful: C2 and C2_2nd do not match.\n");
        return 0;
    }

    // Compare C1 and C1_2nd
    if (memcmp(C1, C1_2nd, L) != 0) {
        printf("Decryption unsuccessful: C1 and C1_2nd do not match.\n");
        return 0;
    }

    // Compare C0 and C0_2nd
    if (memcmp(C0, C0_2nd, N) != 0) {
        printf("Decryption unsuccessful: C0 and C0_2nd do not match.\n");
        return 0;
    }

    // If all comparisons were successful, hash decodedM using SHA-256
    // Use SHA-256 to hash Km =  H(m) session key
    unsigned char KM_2nd[SHA256_DIGEST_LENGTH];
    SHA256(decodedM, m_length / 8, KM_2nd);

    printf("Decryption successful, and hash computed.\n");

    printf("SHA-256 K2: \n");
    print_binary(KM_2nd, SHA256_DIGEST_LENGTH);
    printf("Original SHA-256 K: \n");
    print_binary(KM, SHA256_DIGEST_LENGTH);
    

    // 释放所有动态分配的向量
    free(m);
    free(vector_r);
    free(encodedMessage);
    free(C0);
    free(BTr);
    free(C1);
    free(STC0);
    free(receivedM);
    free(decodedM);
    free(vector_r2);
    free(encodedM_2nd);
    free(C0_2nd);
    free(BTr2);
    free(C1_2nd);
//    free(LM);
//    free(LM2);

    // 释放所有动态分配的矩阵
    for (int i = 0; i < N; i++) {
        free(matrixA[i]);
        free(matrixS[i]);
    }
    free(matrixA);
    free(matrixS);

    for (int i = 0; i < M; i++) {
        free(E[i]);
        free(ET[i]);
    }
    free(E);
    free(ET);

    for (int i = 0; i < L; i++) {
        free(ST[i]);
        free(STA[i]);
        free(BT[i]);
    }
    free(ST);
    free(STA);
    free(BT);

    return 0;
}
