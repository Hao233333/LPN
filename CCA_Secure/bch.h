
#ifndef bch_h
#define bch_h

#include <stdio.h>



void read_p(void);
void generate_gf(void);
void gen_poly(void);
unsigned char* encode_bch(unsigned char *data);
unsigned char* decode_bch(unsigned char* message, int L, int *errorCount);
#endif /* bch_h */
