
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void read_vector_from_file(const char *file_path, unsigned char *vector, int n) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error: Unable to open file for reading: %s\n", file_path);
        exit(1);
    }
    
    fread(vector, sizeof(unsigned char), n, file);
    
    fclose(file);
}

unsigned int read_seed_from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        printf("Error: Unable to open file for reading: %s\n", file_path);
        exit(1);
    }
    
    unsigned int seed;
    fscanf(file, "%u", &seed);
    fclose(file);
    return seed;
}


unsigned char** read_matrix_from_file(const char* file_path, int n) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error: Unable to open file for reading: %s\n", file_path);
        exit(1);
    }
    
    unsigned char** matrix = malloc(n * sizeof(unsigned char*));
    for (int i = 0; i < n; i++) {
        matrix[i] = malloc(n * sizeof(unsigned char));
        fread(matrix[i], sizeof(unsigned char), n, file);
    }
    
    fclose(file);
    return matrix;
}

void print_vector(unsigned char *vector, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}


void print_matrix(int rows, int cols, unsigned char **matrix) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_binary(unsigned char *str, size_t len)
{
    int i, j;
    for(i = len - 1; i >= 0; i--)
    {
        for(j = 7; j >= 0; j--)
            printf("%d ", ((str[i] >> j) & 0x01));
    }
    printf("\n");
}
void write_matrix_to_file(const char *file_path, unsigned char **matrix, int rows, int cols) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error: Unable to open file for writing: %s\n", file_path);
        exit(1);
    }
    
    for (int i = 0; i < rows; i++) {
        fwrite(matrix[i], sizeof(unsigned char), cols, file);
    }
    
    fflush(file);
    fclose(file);
}



