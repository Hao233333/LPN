

#ifndef calculation_h
#define calculation_h


unsigned char **multiply_matrices(unsigned char **matrix1, int matrix1_rows, int matrix1_cols, unsigned char **matrix2, int matrix2_rows, int matrix2_cols);
void generate_binary_vector(unsigned char *vector, int length);


//matrix vector multiplication
void matrix_vector_multiplication(unsigned char **matrix, int rows, int cols, unsigned char *vector, unsigned char *result);

void matrix_vector_multiplication2(unsigned char **matrix, int rows, int cols, unsigned char *vector, unsigned char *result);

    
//calculate y=sA+e
void add_vectors(unsigned char *vector1, unsigned char *vector2, unsigned char *result, int length);

void add_matrices(unsigned char **matrix1, unsigned char **matrix2, unsigned char **result, int rows, int cols);
// Write a vector to a file
void write_vector_to_file(const char *file_path, unsigned char *vector, int n);

unsigned char* append_zeros(unsigned char* vector, int len, int n);
#endif /* calculation_h */
