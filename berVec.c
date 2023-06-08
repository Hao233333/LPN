
#include <stdio.h>
#include <openssl/rand.h>
#include <assert.h>
#include "config.h"
#include "berVec.h"

#define N N_VALUE
#define M M_VALUE
#define r R_VALUE


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

//cols rows都写出来
unsigned char **generate_bernoulli_matrix(int n) {
    
    assert(r >= 0.0 && r <= 1.0);
    
    int len = n ;
    unsigned char **matrix = (unsigned char **)malloc(len * sizeof(unsigned char *));
    if (matrix == NULL) {
        printf("Error: Unable to allocate memory for the matrix\n");
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        matrix[i] = (unsigned char *)malloc(len * sizeof(unsigned char));
        if (matrix[i] == NULL) {
            printf("Error: Unable to allocate memory for matrix row\n");
            exit(1);
        }

        for (int j = 0; j < len; j++) {
            unsigned char rand_byte; //用于存储0-255的正整数，1 byte 1 字符
            RAND_bytes(&rand_byte, 1);
            double rand_fraction = rand_byte / 255.0;

            if (rand_fraction < r) {
                matrix[i][j] = 1;
            } else {
                matrix[i][j] = 0;
            }
        }
    }

    return matrix;
}
