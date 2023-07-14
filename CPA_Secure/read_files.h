
#ifndef read_files_h
#define read_files_h



void read_vector_from_file(const char *file_path, unsigned char *vector, int n);

unsigned int read_seed_from_file(const char *file_path);

unsigned char** read_matrix_from_file(const char* file_path, int n);

void print_vector(unsigned char *vector, int length);

void print_matrix(int rows, int cols, unsigned char **matrix);

void write_matrix_to_file(const char *file_path, unsigned char **matrix, int n);

#endif /* read_files_h */
