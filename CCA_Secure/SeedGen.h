#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef SeedGen_h
#define SeedGen_h


int seed(void);


unsigned int lcg_rand(void);


void seed_lcg(unsigned int seed);


void generate_seed(unsigned int *seed);


void save_seed_to_file(unsigned int seed, const char *file_path);

#endif /* SeedGen_h */
