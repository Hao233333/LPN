
#include <stdio.h>
#include "PRNG_Seed.h"
#include "keyGenAndED.h"
//#include "PubKeyGen.h"
//#include "EncAndDec.h"

//clang -o main main.c PRNG_Seed.c PubKeyGen.c keyGenAndED.c EncAndDec.c read_files.c berVec.c calculation.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto


//clang -o main main.c PRNG_Seed.c read_files.c berVec.c calculation.c keyGenAndED.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto
//./main
int main() {
    int equal_count = 0;
    int total_runs = 1; // running times
    seed();
    
    for (int i = 0; i < total_runs; i++) {
        int result = PubKeyGenAndED();

        if (result) {
            equal_count++;
        }
    }
    //printf("在 %d 次运行中，有 %d 次 d 和 v 的值相等。\n", total_runs, equal_count);

    printf("In %d runs, there are %d times when the values of decryption and v are equal.\n", total_runs, equal_count);

    return 0;
}
