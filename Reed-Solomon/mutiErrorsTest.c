#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rs.h"


//clang -o test mutiErrorsTest.c rs.c galois.c crcgen.c berlekamp.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./test

unsigned char msg[] = {0x00,  0x01,  0x02,  0x03,  0x04,
                       0x05,  0x06,  0x07,  0x08,  0x09,
                       0x0A,  0x0B,  0x0C,  0x0D,  0x0E,
                       0x0F,  0x10,  0x11,  0x12,  0x13,
                       0x14,  0x15,  0x16,  0x17,  0x18,
                       0x19,  0x1A,  0x1B};
unsigned char codeword[256];

void
print_word (int n, unsigned char *data) {
  int i;
  for (i=0; i < n; i++) {
    printf ("%02X ", data[i]);
  }
  printf("\n");
}

void print_inverse_hex(int loc, unsigned char* data) {
    unsigned char byte = data[loc];
    unsigned char inverse = byte ^ 0xFF; // XOR with 0xFF to get the inverse
    printf("Byte at location %d is 0x%02X, its inverse is 0x%02X\n", loc+1, byte, inverse);
}

int
main (int argc, char *argv[])
{
  int erasures[16];
  int nerasures = 0;

  /* Initialization the ECC library */
 
  initialize_ecc ();
 
#define ML (sizeof (msg) + NPAR)

  /* Add one error and two erasures */
  /* We need to indicate the position of the erasures.  Eraseure
     positions are indexed (1 based) from the end of the message... */
 
  decode_data(codeword, ML);

  srand(time(NULL));   // Initialization, should only be called once.

  int fails = 0;
  for (int i = 0; i < 2; i++) {
      printf("Round %d\n", i+1);
      int j;
      // make random msg data
      for (j=0; j<sizeof(msg); j++) {
          msg[j] = rand() % 256;
      }
      printf("msg =\n");
      print_word(28, msg);
      encode_data(msg, sizeof(msg), codeword);
      printf("encoded data:\n");
      print_word(ML, codeword);

   //add errors
      
      
    //error 1
      
      // Choose a specific location to introduce an error
      int specific_loc = 5;  // Change this to the desired location
      print_inverse_hex(specific_loc, msg);
      unsigned char specific_inverse = msg[specific_loc] ^ 0xFF;

      // Introduce the error at the chosen location
      codeword[specific_loc] = specific_inverse;

//      printf("encoded data with controlled error at loc %d\n", specific_loc);


    //error 2
      int specific_loc2 = 8;  // Change this to the desired location
      print_inverse_hex(specific_loc2, msg);
      unsigned char specific_inverse2 = msg[specific_loc2] ^ 0xFF;

      // Introduce the error at the chosen location
      codeword[specific_loc2] = specific_inverse2;

//      printf("encoded data with controlled error at loc %d\n", specific_loc2);
      //print_word(ML, codeword);
      
    //error 3
      int specific_loc3 = 10;  // Change this to the desired location
      print_inverse_hex(specific_loc3, msg);
      unsigned char specific_inverse3 = msg[specific_loc3] ^ 0xFF;

      // Introduce the error at the chosen location
      codeword[specific_loc3] = specific_inverse3;

//      printf("encoded data with controlled error at loc %d\n", specific_loc2);
      print_word(ML, codeword);
 //error adding end
      
      /* Now decode -- encoded codeword size must be passed */
      decode_data(codeword, ML);


      {
          int syndrome = check_syndrome ();
          printf("syndrome = %d\n",syndrome);
          /* check if syndrome is all zeros */
          if (syndrome == 0) {
              // no errs detected, codeword payload should match message
              for (int k=0; k < sizeof(msg); k++) {
                  if (msg[k] != codeword[k]) {
                      printf("#### FAILURE TO DETECT ERROR @ %d: %d != %d\n", k, msg[k], codeword[k]);
                      fails++;
                  }
              }

          } else {
              printf("nonzero syndrome, attempting correcting errors\n");
              int result = 0;
              result =correct_errors_erasures (codeword,
                                               ML,
                                               nerasures,
                                               erasures);
              printf("correct_errors_erasures = %d\n", result);
              print_word(28, codeword);
              int k;
              for (k=0; k < sizeof(msg); k++) {
                  if (msg[k] != codeword[k]) {
                      printf("##### FAILURE TO CORRECT ERROR @ %d: %d != %d\n", k+1, msg[k], codeword[k]);
                      fails++;
                  }
              }
              printf("\n");
          }
      }
  }


  if (fails == 0) {
      printf("\n\n All Tests Passed: No failures to correct codeword\n");
  } else {
      printf("### ERROR Algorithm failed to correct codeword %d times!!!\n", fails);
  }

  exit(0);
}

