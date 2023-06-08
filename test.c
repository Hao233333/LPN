#include <stdio.h>
#include "test.h"
#include <stdlib.h>
#include <openssl/rand.h>
#include <time.h>
#include <assert.h>
#include "config.h"
#include "berVec.h"

#define N N_VALUE
#define M M_VALUE
#define r R_VALUE

//clang -o test test.c  -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
int rows = N;
int cols = M;

void print_vector(unsigned char *vector, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}


unsigned char **multiply_matrices(unsigned char **matrix1, int matrix1_rows, int matrix1_cols, unsigned char **matrix2, int matrix2_rows, int matrix2_cols) {
    // Ensure the column count of the first matrix equals the row count of the second matrix.
    if (matrix1_cols != matrix2_rows) {
        printf("Error: The number of columns in the first matrix must equal the number of rows in the second matrix for multiplication.\n");
        exit(1);
    }
    
    // Allocate memory for the result matrix.
    unsigned char **result = (unsigned char **)malloc(matrix1_rows * sizeof(unsigned char *));
    if (result == NULL) {
        printf("Error: Unable to allocate memory for the result matrix\n");
        exit(1);
    }
    
    for (int i = 0; i < matrix1_rows; i++) {
        // Allocate memory for each row of the result matrix.
        result[i] = (unsigned char *)malloc(matrix2_cols * sizeof(unsigned char));
        if (result[i] == NULL) {
            printf("Error: Unable to allocate memory for result matrix row\n");
            exit(1);
        }
        
        for (int j = 0; j < matrix2_cols; j++) {
            unsigned char sum = 0;
            for (int k = 0; k < matrix1_cols; k++) {  // The number of columns in matrix1 should be equal to the number of rows in matrix2.
                sum ^= (matrix1[i][k] & matrix2[k][j]);  // XOR and AND operations for binary matrix multiplication.
            }
            result[i][j] = sum;
        }
    }
    return result;
}

unsigned char *generate_bernoulli_vector(int n) {
    int len = n;
    unsigned char *vector = (unsigned char *)malloc(len * sizeof(unsigned char));
    if (vector == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    assert(r >= 0.0 && r <= 1.0);
    
    for (int i = 0; i < len; i++) {
        unsigned char rand_byte; //用于存储0-255的正整数，1 byte 1 字符
        RAND_bytes(&rand_byte, 1);
        double rand_fraction = rand_byte / 255.0;
        
        if (rand_fraction < r) {
            vector[i] = 1;
        } else {
            vector[i] = 0;
        }
    }
    
    return vector;
}



//calculate y=sA+e
void add_vectors(unsigned char *vector1, unsigned char *vector2, unsigned char *result, int length) {
    for (int i = 0; i < length; i++) {
        result[i] = vector1[i] ^ vector2[i];
    }
}




void generate_binary_vector(unsigned char *vector, int length) {
    unsigned char buf;
    for (int i = 0; i < length; i++) {
        RAND_bytes(&buf, 1);
        vector[i] = buf & 1;
    }
}





int main(){
    unsigned char **matrixA = (unsigned char **)malloc(rows * sizeof(unsigned char *));
    
    for (int i = 0; i < rows; i++) {
        matrixA[i] = (unsigned char *)malloc(cols * sizeof(unsigned char));
        for (int j = 0; j < cols; j++) {
            matrixA[i][j] = rand() % 2;
        }
    }
    
    unsigned char *vectorS = (unsigned char *)malloc(N * sizeof(unsigned char *));
    
    generate_binary_vector(vectorS,N);
    
    //将vector 转换为matrix
    unsigned char **vectorS_2D = (unsigned char **)malloc(sizeof(unsigned char *));
    vectorS_2D[0] = vectorS;
    
    unsigned char **result =multiply_matrices(vectorS_2D,1,N,matrixA,N,M);
    
    unsigned char *vector = (unsigned char *)malloc(M * sizeof(unsigned char));
    if (vector == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    for (int j = 0; j < M; j++) {
        vector[j] = result[0][j];
    }
    
    // print
    printf("result of sA:\n");
    print_vector(vector, M);
    printf("\n");
    
    unsigned char *vector_e = generate_bernoulli_vector(M);
    
    printf("e of :\n");
    print_vector(vector_e, M);
    printf("\n");
    
    unsigned char *y = (unsigned char *)malloc(rows * sizeof(unsigned char));
    
    add_vectors(vector, vector_e, y, M);
    
    printf("result of y:\n");
    print_vector(y, M);
    printf("\n");
    
    
    
    for (int i = 0; i < rows; i++) {
        free(matrixA[i]);
    }
    // Free the pointer to matrixA
    free(matrixA);
    
    // Free vectorA
    free(vectorS);
    
    // Free vectorA_2D
    free(vectorS_2D);
    
    // Free each row of result
    for (int i = 0; i < 1; i++) {
        free(result[i]);
    }
    // Free the pointer to result
    free(result);
    
    // Free vector
    free(vector);
    
    // Free vector_e
    free(vector_e);
    
    // Free y
    free(y);
    
    
}


