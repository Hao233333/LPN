#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "EncAndDec.h"
#include "berVec.h"
#include "read_files.h"
#include "calculation.h"
#include "config.h"
#define N N_VALUE
#define M M_VALUE
#define r R_VALUE

int EncAndDec(void){
    
    int rows = N;
    int cols = M;
    printf("Now we are in Enc.\n");

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
    
    const char *U_path = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/U.txt";
    unsigned char** matrixU = read_matrix_from_file(U_path, M);
    
    const char *T_path = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/T.txt";
    unsigned char** matrixT = read_matrix_from_file(T_path, M);
    
    
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
    
    // Generate Bernoulli distributed vector e (length M)
    unsigned char *vector_e = generate_bernoulli_vector(M);
    
    printf("\nVector e:\n");
    for (int i = 0; i < M; i++) {
        printf("%d ", vector_e[i]);
    }
    printf("\n");
    
    //compute sA
    unsigned char *sA = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (sA == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }

    // 计算u=f的转置与A的乘积加上向量v u=f^T*A 长度为n
    matrix_vector_multiplication(matrixA, rows, cols, vector_s, sA);


    // print sA
    printf("Recreated Matrix A:\n");
    print_vector(sA, M);
    printf("\n");
    
    //do y = sA + e
    
    unsigned char *y = (unsigned char *)malloc(rows * sizeof(unsigned char));
    add_vectors(sA, vector_e, y, rows);
    
    // print y (As + e)
    printf("y = (aA + e):\n");
    print_vector(y, rows);
    
    
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

    matrix_vector_multiplication(matrixU, rows, cols, vector_s, sU);

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

    matrix_vector_multiplication(matrixT, rows, cols, y, yT);
    
    //compute dycption = yT + C
    unsigned char *dycption = (unsigned char *)malloc(cols * sizeof(unsigned char));
    if (dycption == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    add_vectors(yT, c, dycption, M);
    
    
    
    // Free memory
    
    free(matrixA);
        for (int i = 0; i < rows; i++) {
            free(matrixA[i]);
        }

        free(matrixU);
        for (int i = 0; i < M; i++) {
            free(matrixU[i]);
        }

        free(matrixT);
        for (int i = 0; i < M; i++) {
            free(matrixT[i]);
        }

        free(vector_v);
        free(vector_s);
        free(vector_e);
        free(y);
        free(vector_x);
        free(sA);
        free(sU);
        free(sUx);
        free(c);
        free(yT);
        free(dycption);
    
    return dycption == vector_v;
}
