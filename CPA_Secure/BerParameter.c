#include <stdio.h>
#include <openssl/rand.h>
#include <assert.h>
#include "config.h"
#include "BerParameter.h"

#define N N_VALUE
#define M M_VALUE
#define t T_VALUE

unsigned char **generateBinaryMatrix(int rows, int cols) {
    unsigned char **matrixA = (unsigned char **)malloc(rows * sizeof(unsigned char *));
    
    for (int i = 0; i < rows; i++) {
        matrixA[i] = (unsigned char *)malloc(cols * sizeof(unsigned char));
        for (int j = 0; j < cols; j++) {
            matrixA[i][j] = rand() % 2;
        }
    }
    
    return matrixA;
};



unsigned char *generateBernoulliVector(int n) {
    int len = n;
    unsigned char *vector = (unsigned char *)malloc(len * sizeof(unsigned char));
    if (vector == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }

    assert(t >= 0.0 && t <= 1.0);

    for (int i = 0; i < len; i++) {
        unsigned char rand_byte; //用于存储0-255的正整数，1 byte 1 字符
        RAND_bytes(&rand_byte, 1);
        double rand_fraction = rand_byte / 255.0;

        if (rand_fraction < t) {
            vector[i] = 1;
        } else {
            vector[i] = 0;
        }
    }

    return vector;
}

//cols rows都写出来
//unsigned char **generate_bernoulli_matrix(int n) {
//
//    assert(t >= 0.0 && t <= 1.0);
//
//    int len = n ;
//    unsigned char **matrix = (unsigned char **)malloc(len * sizeof(unsigned char *));
//    if (matrix == NULL) {
//        printf("Error: Unable to allocate memory for the matrix\n");
//        exit(1);
//    }
//
//    for (int i = 0; i < len; i++) {
//        matrix[i] = (unsigned char *)malloc(len * sizeof(unsigned char));
//        if (matrix[i] == NULL) {
//            printf("Error: Unable to allocate memory for matrix row\n");
//            exit(1);
//        }
//
//        for (int j = 0; j < len; j++) {
//            unsigned char rand_byte; //用于存储0-255的正整数，1 byte 1 字符
//            RAND_bytes(&rand_byte, 1);
//            double rand_fraction = rand_byte / 255.0;
//
//            if (rand_fraction < t) {
//                matrix[i][j] = 1;
//            } else {
//                matrix[i][j] = 0;
//            }
//        }
//    }
//
//    return matrix;
//}

unsigned char **generateBernoulliMatrix(int rows, int cols) {
    assert(t >= 0.0 && t <= 1.0);

    unsigned char **matrix = (unsigned char **)malloc(rows * sizeof(unsigned char *));
    if (matrix == NULL) {
        printf("Error: Unable to allocate memory for the matrix\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = (unsigned char *)malloc(cols * sizeof(unsigned char));
        if (matrix[i] == NULL) {
            printf("Error: Unable to allocate memory for matrix row\n");
            exit(1);
        }

        for (int j = 0; j < cols; j++) {
            unsigned char rand_byte;
            RAND_bytes(&rand_byte, 1);
            double rand_fraction = rand_byte / 255.0;

            if (rand_fraction < t) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}

unsigned char **generateTransposeMatrix(unsigned char **matrix, int rows, int cols) {
    unsigned char **transposeMatrix = (unsigned char **)malloc(cols * sizeof(unsigned char *));
    if (transposeMatrix == NULL) {
        printf("Error: Unable to allocate memory for the transpose matrix\n");
        exit(1);
    }

    for (int i = 0; i < cols; i++) {
        transposeMatrix[i] = (unsigned char *)malloc(rows * sizeof(unsigned char));
        if (transposeMatrix[i] == NULL) {
            printf("Error: Unable to allocate memory for transpose matrix row\n");
            exit(1);
        }

        for (int j = 0; j < rows; j++) {
            transposeMatrix[i][j] = matrix[j][i];
        }
    }

    return transposeMatrix;
}
