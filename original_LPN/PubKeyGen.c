#include <stdio.h>
#include <stdlib.h>

#include "PubKeyGen.h"
#include "config.h"
#include "berVec.h"
#include "read_files.h"
#include "calculation.h"

#define N N_VALUE
#define M M_VALUE
#define r R_VALUE


//clang -o PubKeyGen PubKeyGen.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./PubKeyGen


int PubKeyGen(void) {
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

    unsigned char **U = multiply_matrices(matrixA, T, rows, cols);
    
    printf("Matrix U:\n");
    print_matrix(rows, cols, U);

    
    //88888
    printf("这是啥？\n");
    
//    // Write the result vector b to the file
//    const char *U_file_path1 = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/U.bin";
//    const char *U_file_path2 = "/Users/qh/Documents/projects/LPN/LPN/pubKeys/T.bin";
//    
//    
//    write_matrix_to_file(U_file_path1, U, M);
//    printf("cunchuwanle1？\n");
//    write_matrix_to_file(U_file_path2, T, M);
//    printf("cunchuwanle2？\n");
    // Free memory
    for (int i = 0; i < rows; i++) {
        free(matrixA[i]);
        free(T[i]);
        free(U[i]);
    }
    free(matrixA);
    free(T);
    free(U);
    


    return 0;
}
