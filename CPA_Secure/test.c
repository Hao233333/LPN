
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


#define N N_VALUE
#define M M_VALUE
//#define T T_VALUE
//#define L L_VALUE
#define K K_VALUE

//clang -o test test.c SeedGen.c BerParameter.c bch.c read_files.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//test
int main(){
    
    int L = 490;
    int messageLen = 256;
    const char *seed_file_path = "/Users/qh/Documents/projects/LPN/LPN/CPA_Secure/Seeds/seed.txt";
    
    unsigned int seed = read_seed_from_file(seed_file_path);
    printf("Seed from file: %u\n", seed);
    srand(seed);
    
    
    //random message length = 256
    unsigned char *message = (unsigned char *)malloc(messageLen * sizeof(unsigned char));
    
    if (message == NULL) {
        printf("Error: Unable to allocate memory for the vector\n");
        exit(1);
    }
    
    generate_binary_vector(message, messageLen);
    
    printf("message is:\n");
    
    print_vector(message, messageLen);
    printf("\n");
    
    
    //encode the message; length = L = 490, max error = 28
    
    int i;
    extern int length, k, t, d;
    extern int p[21];
    extern int alpha_to[1048576], index_of[1048576], g[548576];
    extern int recd[1048576], data[1048576], bb[548576];
    extern int numerr, errpos[1024], decerror;
    read_p();               /* Read m */
    generate_gf();          /* Construct the Galois Field GF(2**m) */
    gen_poly();             /* Compute the generator polynomial of BCH code */
    
    /* to get encoded message  but how many bits ?????????????????? I define it as L*/
    unsigned char *encodedMessage = encode_bch(message);
    
    printf("r(x) = \n");
    for (i = 0; i < length; i++) {
        printf("%1d", recd[i]);
        if (i && (((i+1) % 8) == 0))
            printf(" ");
    }
    printf("\n");
    
    
    printf(" encoded message is:\n");
    
    print_vector(encodedMessage, L);
    printf("\n");
    //generate Ber error vector, length = 490, (length)
    
    //    unsigned char *error = (unsigned char *)malloc(length * sizeof(unsigned char));
    //
    //    if (error == NULL) {
    //        printf("Error: Unable to allocate memory for the vector\n");
    //        exit(1);
    //    }
    
    unsigned char *error = generateBernoulliVector(L);
    
    
    printf(" error message is:\n");
    
    print_vector(error, L);
    printf("\n");
    
    //add errors into the encoded message
    
    unsigned char *receivedMesssage = (unsigned char *)malloc(L * sizeof(unsigned char));
    if (receivedMesssage == NULL) {
        printf("Error: Unable to allocate memory for the result vector\n");
        exit(1);
    }
    
    add_vectors(error,encodedMessage, receivedMesssage, L);
    
    printf(" received message is:\n");
    
    print_vector(receivedMesssage, L);
    printf("\n");
    
    //decode received message
    int errorCount;
    unsigned char *decodedMessage = decode_bch(receivedMesssage,L,&errorCount);
    
    printf("message is:\n");
    
    print_vector(message, messageLen);
    printf("\n");
    
    printf(" decoded message is:\n");

    print_vector(decodedMessage, messageLen);
    printf("\n");
    
    int is_same = (memcmp(decodedMessage, message, messageLen) == 0);
    
    if (is_same){
        printf("Decoding successfully!!!!!!!!!\n");
    }else{
        printf("Decoding fail........\n");
    };
    free(message);
    free(encodedMessage);
    free(error);
    free(receivedMesssage);
    free(decodedMessage);
}
