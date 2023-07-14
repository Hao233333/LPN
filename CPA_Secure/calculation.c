
#include <stdio.h>
#include <openssl/rand.h>
#include <assert.h>
#include "calculation.h"


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
//unsigned char **multiply_matrices(unsigned char **matrix1, unsigned char **matrix2, int rows, int cols) {
//    unsigned char **result = (unsigned char **)malloc(rows * sizeof(unsigned char *));
//    if (result == NULL) {
//        printf("Error: Unable to allocate memory for the result matrix\n");
//        exit(1);
//    }
//
//    for (int i = 0; i < rows; i++) {
//        result[i] = (unsigned char *)malloc(cols * sizeof(unsigned char));
//        if (result[i] == NULL) {
//            printf("Error: Unable to allocate memory for result matrix row\n");
//            exit(1);
//        }
//
//        for (int j = 0; j < cols; j++) {
//            unsigned char sum = 0;
//            for (int k = 0; k < cols; k++) {//第一个的第一列乘以后第二个每个数
//                sum ^= (matrix1[i][k] & matrix2[k][j]);  // XOR is used for binary matrix multiplication
//            }
//
//            result[i][j] = sum;
//        }
//    }
//
//    return result;
//}




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


