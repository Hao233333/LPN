#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef PRNG_seed_h
#define PRNG_seed_h


int seed(void);


unsigned int lcg_rand(void);


void seed_lcg(unsigned int seed);


void generate_seed(unsigned int *seed);


void save_seed_to_file(unsigned int seed, const char *file_path);

#endif /* PRNG_seed_h */
