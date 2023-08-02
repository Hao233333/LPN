#ifndef BerParameter_h
#define BerParameter_h

#include <stdio.h>

unsigned char *generate_Rvector(const unsigned char* message, int num_bits, int m);

unsigned char **generateBinaryMatrix(int rows, int cols);

unsigned char *generateBernoulliVector(int n);

unsigned char **generateBernoulliMatrix(int rows, int cols);

unsigned char **generateTransposeMatrix(unsigned char **matrix, int rows, int cols);

#endif /* BerParameter_h */

