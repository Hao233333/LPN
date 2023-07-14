//
//  bch.h
//  LPN
//
//  Created by 秦昊 on 11/7/2023.
//

#ifndef bch_h
#define bch_h

#include <stdio.h>



void read_p(void);
void generate_gf(void);
void gen_poly(void);
unsigned char* encode_bch(unsigned char *data);
unsigned char* decode_bch(unsigned char* message, int L);
#endif /* bch_h */
