
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/rand.h>
#include "config.h"
#include "BerParameter.h"
#include "read_files.h"
#include "calculation.h"
#include "CPASystem.h"
#include "bch.h"


#define N N_VALUE
#define M M_VALUE
//#define T T_VALUE
#define L L_VALUE
#define K K_VALUE

//clang -o testForPy testForPy.c SeedGen.c BerParameter.c bch.c read_files.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//CPA

int main() {

    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CPA_Secure/Seeds/seed.txt";
    
    unsigned int seed = read_seed_from_file(seed_file_path);
    //printf("Seed from file: %u\n", seed);
    srand(seed);
    
    //generate A Z^(n x m)_2;
    unsigned char **matrixA = generateBinaryMatrix(N, M);
    
    // print matrix
//    printf("Matrix A:\n");
//    print_matrix(N, M, matrixA);
    
    //generate S Z^(n x l)_2;
    unsigned char **matrixS = generateBinaryMatrix(N, L);
    
    // print matrix
    //printf("Matrix S:\n");
    //print_matrix(N, L, matrixS);
    
    
    //generate Ber E Z^(m x L)_t;
    unsigned char **E = generateBernoulliMatrix(M,L);
    
//    printf("Matrix E:\n");
//     print_matrix(M, L, E);
    
    //ET  L x M
    unsigned char **ET = generateTransposeMatrix(E, M, L);
//    printf("Matrix ET:\n");
//    print_matrix(L, M, ET);
    
    //ST L x N
    unsigned char **ST = generateTransposeMatrix(matrixS, N, L);
//    printf("Matrix ST:\n");
    //print_matrix(L, N, ST);
    
    //S^T A L行M列
    unsigned char **STA = multiply_matrices(ST, L, N, matrixA, N, M);
    
    //printf("Matrix STA:\n");
    //print_matrix(L, M, STA);
    
    //B^T L行M列
    unsigned char **BT = (unsigned char **)malloc(L * sizeof(unsigned char *));
    if (BT == NULL) {
        printf("Failed to allocate memory for BT\n");
        exit(1);
    };
    
    for (int i = 0; i < L; i++) {
        BT[i] = (unsigned char *)malloc(M * sizeof(unsigned char));
        if (BT[i] == NULL) {
            printf("Failed to allocate memory for BT[%d]\n", i);
            exit(1);
        }
    }
    
    //STA L M, ET四列八行，+ BT 八行四列
    //应该是E
    add_matrices(STA, ET, BT, L, M);
    //printf("Matrix BT:\n");
    //print_matrix(L, M, BT);
    //B
    //unsigned char **B = generateTransposeMatrix(BT, L, M);
    //printf("Matrix B:\n");
    //print_matrix(M, L, B);
    
    
    //encryption
    //generate messages m bits
    unsigned char *message = (unsigned char *)malloc(K * sizeof(unsigned char));
    
    if (message == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    generate_binary_vector(message, K);
    
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
    unsigned char *encodedMessage = encode_bch(message);
    
//    printf("r(x) = (this is encoded message )\n");
//    for (i = 0; i < length; i++) {
//        printf("%1d ", recd[i]);
////        if (i && (((i+1) % 8) == 0))
////            printf(" ");
//    }
//    printf("\n");
    
//    printf("encoded Message is:\n");
//
//    print_vector(encodedMessage, L);
    
    
    
    //generate r -->Ber^m_t
    unsigned char *vector_r = generateBernoulliVector(M);
    
    //calculate C0 = Ar
    
    unsigned char *C0 = (unsigned char *)malloc(N * sizeof(unsigned char));
    if (C0 == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    matrix_vector_multiplication(matrixA, N, M, vector_r, C0);
    
    //Er
    unsigned char *Er = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (Er == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    matrix_vector_multiplication(ET, L, M, vector_r, Er);
    
    //printf("Er is:\n");
    
    //print_vector(Er, L);
    
    
    
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
    
    //printf("C1 is:\n");
    
    //print_vector(C1, L);
    
    //printf("r is:\n");
    
    //print_vector(vector_r, M);
    

    
    //decryption
   

    
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
    
    
//    printf("received Message is:\n");
//
//    print_vector(receivedM, L);
    
    //decode received message
    int errorCount;
    unsigned char *decodedM = decode_bch(receivedM, L,&errorCount);
    
    printf("Error count: %d\n", errorCount);
    
//    if (decodedM == NULL) {
//        printf("Decoding failed.\n");
//    }
    
//    printf("Original Message is:\n");
//    print_vector(message, N);
//
//    printf("decoded Message is:\n");
//    print_vector(decodedM, N);
    
    
    int is_same = (memcmp(decodedM, message, N) == 0);
    
    if (is_same){
        printf("Decoding successfully!!!!!!!!!\n");
    }else{
        printf("Decoding fail........\n");
    };
    
    // Free memory
    for (int i = 0; i < N; i++) {
        free(matrixA[i]);
        free(matrixS[i]);
    }
    free(matrixA);
    free(matrixS);
    
//    for (int i = 0; i < L; i++) {
//        free(E[i]);
//        free(ET[i]);
//        free(ST[i]);
//        free(STA[i]);
//        free(BT[i]);
//        //free(B[i]);
//    }
    free(E);
    free(ET);
    free(ST);
    free(STA);
    free(BT);
    //free(B);
    free(encodedMessage);
    free(C0);
    free(BTr);
    free(C1);
    free(STC0);
    free(receivedM);
    free(decodedM);
    free(message);
    
    return errorCount;
    
}
