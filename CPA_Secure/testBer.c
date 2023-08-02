
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/rand.h>
#include "config.h"
#include "BerParameter.h"
#include "read_files.h"
#include "calculation.h"
#include "CPASystem.h"
#include "bch.h"

//clang -o testBer testBer.c SeedGen.c BerParameter.c bch.c read_files.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//tesBer

#define N N_VALUE
#define M M_VALUE
#define T T_VALUE
//#define L L_VALUE
#define K K_VALUE

int count_ones(unsigned char* vec, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (vec[i] == 1) {
            count++;
        }
    }
    return count;
}

    

int main(){
    //generate r -->Ber^m_t
    
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CPA_Secure/Seeds/seed.txt";
    
    unsigned int seed = read_seed_from_file(seed_file_path);
    printf("Seed from file: %u\n", seed);
    srand(seed);
    
    unsigned char *vector_r = generateBernoulliVector(M);
    

    
    int count = count_ones(vector_r, M);

    printf("The vector contains %d ones.\n", count);

    int num = M * T;

    printf("The vector should contains %d ones.\n", num);
}
