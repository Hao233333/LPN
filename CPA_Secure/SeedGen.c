
#include "SeedGen.h"
#include "config.h"
#include "read_files.h"

#define cols N_VALUE
#define rows M_VALUE

//*************  the seed for matrix  *******************


//clang -o PRNG_seed PRNG_seed.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./PRNG_seed


unsigned int lcg_seed;

unsigned int lcg_rand(void);


void seed_lcg(unsigned int seed) {
    lcg_seed = seed;
}

unsigned int lcg_rand() {
    lcg_seed = (lcg_seed * 1103515245 + 12345) % 2147483648;
    return lcg_seed;
}

void generate_seed(unsigned int *seed) {
    // Seed the LCG with the current time
    seed_lcg((unsigned int)time(NULL));

    // Generate a random number using LCG
    *seed = lcg_rand();
}

void save_seed_to_file(unsigned int seed, const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing: %s\n", file_path);
        exit(1);
    }

    fprintf(file, "%u", seed);
    fclose(file);
}



int seed(void) {
    // generate the seed and print it out
    unsigned int seed;
    generate_seed(&seed);
    printf("Generated seed: %u\n", seed);
    srand(seed);

    // save the seed to the file
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CPA_Secure/Seeds/seed.txt";
    save_seed_to_file(seed, seed_file_path);
    


    return 0;
}
