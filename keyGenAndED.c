
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/rand.h>
#include "config.h"
#include "berVec.h"
#include "read_files.h"
#include "calculation.h"
#include "keyGenAndED.h"


#define N N_VALUE
#define M M_VALUE
#define r R_VALUE


//clang -o PubKeyGen PubKeyGen.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./PubKeyGen


int PubKeyGenAndED(void) {
    int rows = N;
    int cols = M;
    printf("This is in PubKeyGen\n");
    
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/Seeds/seed.txt";
    
    unsigned int seed = read_seed_from_file(seed_file_path);
    printf("Seed from file: %u\n", seed);
    srand(seed);
    
    unsigned char **matrixA = (unsigned char **)malloc(rows * sizeof(unsigned char *));
    
    for (int i = 0; i < rows; i++) {
        matrixA[i] = (unsigned char *)malloc(cols * sizeof(unsigned char));
        for (int j = 0; j < cols; j++) {
            matrixA[i][j] = rand() % 2;
        }
    }
    

        // print matrix
        printf("Matrix A:\n");
        print_matrix(rows, cols, matrixA);
 

    
    unsigned char **T = generate_bernoulli_matrix(cols);
    //cols = M
    printf("Matrix T:\n");
    print_matrix(cols, cols, T);

    unsigned char **U = multiply_matrices(matrixA, N,M,T, M, M);
    
    printf("Matrix U:\n");
    print_matrix(rows, cols, U);

    
//    // Write the result vector b to the file
//    const char *U_file_path1 = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/U.bin";
//    const char *U_file_path2 = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/T.bin";
//
//
//    write_matrix_to_file(U_file_path1, U, M);
//    printf("cunchuwanle1？\n");
//    write_matrix_to_file(U_file_path2, T, M);
//    printf("cunchuwanle2？\n");
    
    //encryption
   
    //generate message vector v--m bits
    unsigned char *vector_v = (unsigned char *)malloc(rows * sizeof(unsigned char));
    
    if (vector_v == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    generate_binary_vector(vector_v, rows);
    
    
    //generate vector s
    unsigned char *vector_s = (unsigned char *)malloc(N * sizeof(unsigned char));
    
    if (vector_s == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    generate_binary_vector(vector_s, N);
    
    printf("Generated vector s:\n");
    print_vector(vector_s, N);
    
    
    
    //将vector 转换为matrix
    unsigned char **vectorS_2D = (unsigned char **)malloc(sizeof(unsigned char *));
    vectorS_2D[0] = vector_s;
    
    
    unsigned char **sA = multiply_matrices(vectorS_2D, 1,N,matrixA, N, M);
    
    
    unsigned char *vector_sA = (unsigned char *)malloc(M * sizeof(unsigned char));
    if (vector_sA == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    for (int j = 0; j < M; j++) {
        vector_sA[j] = sA[0][j];
    }
    
    // print
    printf("result of sA:\n");
    print_vector(vector_sA, M);
    printf("\n");
    
    
    
    
    //compute sA
//    unsigned char *sA = (unsigned char *)malloc(cols * sizeof(unsigned char));
//    if (sA == NULL) {
//        printf("Error: Unable to allocate memory for the result vector\n");
//        exit(1);
//    }
//
//
//    matrix_vector_multiplication(matrixA, rows, cols, vector_s, sA);
//

    // Generate Bernoulli distributed vector e (length M)
    unsigned char *vector_e = generate_bernoulli_vector(M);
    
    printf("\nVector e:\n");
    print_vector(vector_e, M);
    printf("\n");
    
    //do y = sA + e
    
    unsigned char *y = (unsigned char *)malloc(M * sizeof(unsigned char));
    add_vectors(vector_sA, vector_e, y, M);
    
    // print y (As + e)
    printf("y = (sA + e):\n");
    print_vector(y, M);
    printf("\n");
    
    //    // Write the result vector y to the file
    //    const char *y_file_path = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/y.txt";
    //    write_vector_to_file(y_file_path, y, M);
    //
    
    // Generate Bernoulli distributed vector x (length M)
    unsigned char *vector_x = generate_bernoulli_vector(M);
    
    printf("\nVector x:\n");
    for (int i = 0; i < M; i++) {
        printf("%d ", vector_x[i]);
    }
    printf("\n");
    
    //compute sU

    unsigned char *sU = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (sU == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }

    matrix_vector_multiplication(U, rows, cols, vector_s, sU);

    // print sU
    printf("Vector sU:\n");
    print_vector(sU, M);
    printf("\n");
    
    //compute sU + x
    unsigned char *sUx = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (sUx == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    add_vectors(sU, vector_x, sUx, M);
    
    //compute c = sU + x + v
    unsigned char *c = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (c == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    add_vectors(sUx, vector_v, c, M);
    
    
    //decryption
    
    //yT + C
    //compute yT

    unsigned char *yT = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (yT == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }

    matrix_vector_multiplication(T, rows, cols, y, yT);
    
    //compute dycption = yT + C
    unsigned char *decryption = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (decryption == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    add_vectors(yT, c, decryption, M);
    
    printf("decryption:\n");
    
    print_vector(decryption,M);
    
    printf("message v:\n");
    
    print_vector(vector_v,M);
    
    // Free memory
    for (int i = 0; i < rows; i++) {
        free(matrixA[i]);
        free(T[i]);
        free(U[i]);
    }
    free(matrixA);
    free(T);
    free(U);
    free(vector_v);
    free(vectorS_2D); // free vectorS_2D before vector_s
    free(vector_s);
    free(vector_e);
    free(y);
    free(vector_x);
    free(sA);
    free(vector_sA); // free vector_sA
    free(sU);
    free(sUx);
    free(c);
    free(yT);
    free(decryption);

    return memcmp(decryption, vector_v, M) == 0;


}
