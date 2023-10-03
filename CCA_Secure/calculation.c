
#include <stdio.h>
#include <openssl/rand.h>
#include <assert.h>
#include <string.h>
#include "calculation.h"
#include <omp.h>


//unsigned char **multiply_matrices(unsigned char **matrix1, int matrix1_rows, int matrix1_cols, unsigned char **matrix2, int matrix2_rows, int matrix2_cols) {
//    // Ensure the column count of the first matrix equals the row count of the second matrix.
//    printf("this is in STA");
//    if (matrix1_cols != matrix2_rows) {
//        printf("Error: The number of columns in the first matrix must equal the number of rows in the second matrix for multiplication.\n");
//        exit(1);
//    }
//
//    // Allocate memory for the result matrix.
//    unsigned char **result = (unsigned char **)malloc(matrix1_rows * sizeof(unsigned char *));
//    if (result == NULL) {
//        printf("Error: Unable to allocate memory for the result matrix\n");
//        exit(1);
//    }
//
//    for (int i = 0; i < matrix1_rows; i++) {
//        // Allocate memory for each row of the result matrix.
//        result[i] = (unsigned char *)malloc(matrix2_cols * sizeof(unsigned char));
//        if (result[i] == NULL) {
//            printf("Error: Unable to allocate memory for result matrix row\n");
//            exit(1);
//        }
//
//        for (int j = 0; j < matrix2_cols; j++) {
//            unsigned char sum = 0;
//            for (int k = 0; k < matrix1_cols; k++) {  // The number of columns in matrix1 should be equal to the number of rows in matrix2.
//                sum ^= (matrix1[i][k] & matrix2[k][j]);  // XOR and AND operations for binary matrix multiplication.
//            }
//            result[i][j] = sum;
//        }
//    }
//    return result;
//}


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

    #pragma omp parallel for
    for (int i = 0; i < matrix1_rows; i++) {
        // Allocate memory for each row of the result matrix.
        result[i] = (unsigned char *)malloc(matrix2_cols * sizeof(unsigned char));
        if (result[i] == NULL) {
            printf("Error: Unable to allocate memory for result matrix row\n");
            exit(1);
        }
        
        for (int j = 0; j < matrix2_cols; j++) {
            unsigned char sum = 0;
            for (int k = 0; k < matrix1_cols; k++) {
                sum ^= (matrix1[i][k] & matrix2[k][j]);
            }
            result[i][j] = sum;
        }
    }
    return result;
}



void generate_binary_vector(unsigned char *vector, int length) {
    unsigned char buf;
    for (int i = 0; i < length; i++) {
        RAND_bytes(&buf, 1);
        vector[i] = buf & 1;
    }
}


//matrix vector multiplication
//**
//void matrix_vector_multiplication(unsigned char **matrix, int rows, int cols, unsigned char *vector, unsigned char *result) {
//
//    for (int i = 0; i < cols; i++) {//从第一列第一行开始，一个一个相乘，累和
//        int sum = 0;
//        for (int j = 0; j < rows; j++) {
//            sum ^= (vector[j] & matrix[j][i]);//第一列的不同行与第一列第一个数相乘在与结果异或
//        }
//        result[i] = sum;//结果应该有n个
//    }
//
//}

void matrix_vector_multiplication(unsigned char **matrix, int rows, int cols, unsigned char *vector, unsigned char *result) {
    
    for (int i = 0; i < rows; i++) {//从第一列第一行开始，一个一个相乘，累和
        int sum = 0;
        for (int j = 0; j < cols; j++) {
            sum ^= (matrix[i][j] & vector[j]);//第一列的不同行与第一列第一个数相乘在与结果异或
        }
        result[i] = sum;//结果应该有n个
    }
    
}

void matrix_vector_multiplication2(unsigned char **matrix, int rows, int cols, unsigned char *vector, unsigned char *result) {
    
    for (int i = 0; i < rows; i++) {//从第一列第一行开始，一个一个相乘，累和
        int sum = 0;
        for (int j = 0; j < cols; j++) {
            sum ^= (matrix[i][j] & vector[j]);//第一列的不同行与第一列第一个数相乘在与结果异或
        }
        result[i] = sum;//结果应该有n个
    }
    
}




//calculate y=sA+e
void add_vectors(unsigned char *vector1, unsigned char *vector2, unsigned char *result, int length) {
    
    for (int i = 0; i < length; i++) {
        result[i] = vector1[i] ^ vector2[i];
    }
}

//calculate matrices addition
void add_matrices(unsigned char **matrix1, unsigned char **matrix2, unsigned char **result, int rows, int cols) {
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] ^ matrix2[i][j];
        }
    }
}




// Write a vector to a file
void write_vector_to_file(const char *file_path, unsigned char *vector, int n) {
    
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error: Unable to open file for writing: %s\n", file_path);
        exit(1);
    }
    
    fwrite(vector, sizeof(unsigned char), n, file);
    fflush(file);
    fclose(file);
}

//add zeros (nonce)
unsigned char* append_zeros(unsigned char* vector, int len, int n) {
    // Calculate the number of bytes needed for the new vector
    int new_len = len + n;
    int num_bytes = new_len / 8 + (new_len % 8 != 0);  // Calculate the number of bytes needed

    // Allocate memory for the new vector
    unsigned char* new_vector = (unsigned char*) malloc(sizeof(unsigned char) * num_bytes);
    if (new_vector == NULL) {
        fprintf(stderr, "Failed to allocate memory for the new vector.\n");
        exit(1);
    }

    // Initialize all bits of new_vector to 0
    memset(new_vector, 0, num_bytes);

    // Copy the original vector into the new vector, starting from the n-th bit
    for (int i = 0; i < len; i++) {
        if (vector[i / 8] & (1 << (7 - i % 8))) {  // If the i-th bit of vector is 1
            new_vector[(i + n) / 8] |= (1 << (7 - (i + n) % 8));  // Set the (i+n)-th bit of new_vector to 1
        }
    }

    // Return the new vector
    return new_vector;
}

