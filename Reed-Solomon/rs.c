
#include "rs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>





/* Encoder parity bytes */
int pBytes[MAXDEG];

/* Decoder syndrome bytes */
int synBytes[MAXDEG];

/* generator polynomial */
int genPoly[MAXDEG*2];

int DEBUG = FALSE;

static void
compute_genpoly (int nbytes, int genpoly[]);

/* Initialize lookup tables, polynomials, etc. */
void
initialize_ecc ()
{
  /* Initialize the galois field arithmetic tables */
    init_galois_tables();

    /* Compute the encoder generator polynomial */
    compute_genpoly(NPAR, genPoly);
}

void
zero_fill_from (unsigned char buf[], int from, int to)
{
  int i;
  for (i = from; i < to; i++) buf[i] = 0;
}

/* debugging routines */
void
print_parity (void)
{
  int i;
  printf("Parity Bytes: ");
  for (i = 0; i < NPAR; i++)
    printf("[%d]:%x, ",i,pBytes[i]);
  printf("\n");
}


void
print_syndrome (void)
{
  int i;
  printf("Syndrome Bytes: ");
  for (i = 0; i < NPAR; i++)
    printf("[%d]:%x, ",i,synBytes[i]);
  printf("\n");
}

/* Append the parity bytes onto the end of the message */
void
build_codeword (unsigned char msg[], int nbytes, unsigned char dst[])
{
  int i;
    
  for (i = 0; i < nbytes; i++) dst[i] = msg[i];
    
  for (i = 0; i < NPAR; i++) {
    dst[i+nbytes] = pBytes[NPAR-1-i];
  }
}
    
/**********************************************************
 * Reed Solomon Decoder
 *
 * Computes the syndrome of a codeword. Puts the results
 * into the synBytes[] array.
 */
 
void
decode_data(unsigned char data[], int nbytes)
{
  int i, j, sum;
  for (j = 0; j < NPAR;  j++) {
    sum    = 0;
    for (i = 0; i < nbytes; i++) {
      sum = data[i] ^ gmult(gexp[j+1], sum);
    }
    synBytes[j]  = sum;
  }
}


/* Check if the syndrome is zero */
int
check_syndrome (void)
{
 int i, nz = 0;
 for (i =0 ; i < NPAR; i++) {
  if (synBytes[i] != 0) {
      nz = 1;
      break;
  }
 }
 return nz;
}


void
debug_check_syndrome (void)
{
  int i;
    
  for (i = 0; i < 3; i++) {
    printf(" inv log S[%d]/S[%d] = %d\n", i, i+1,
       glog[gmult(synBytes[i], ginv(synBytes[i+1]))]);
  }
}


/* Create a generator polynomial for an n byte RS code.
 * The coefficients are returned in the genPoly arg.
 * Make sure that the genPoly array which is passed in is
 * at least n+1 bytes long.
 */

static void
compute_genpoly (int nbytes, int genpoly[])
{
  int i, tp[256], tp1[256];
    
  /* multiply (x + a^n) for n = 1 to nbytes */

  zero_poly(tp1);
  tp1[0] = 1;

  for (i = 1; i <= nbytes; i++) {
    zero_poly(tp);
    tp[0] = gexp[i];        /* set up x+a^n */
    tp[1] = 1;
      
    mult_polys(genpoly, tp, tp1);
    copy_poly(tp1, genpoly);
  }
}

/* Simulate a LFSR with generator polynomial for n byte RS code.
 * Pass in a pointer to the data array, and amount of data.
 *
 * The parity bytes are deposited into pBytes[], and the whole message
 * and parity are copied to dest to make a codeword.
 *
 */

void
encode_data (unsigned char msg[], int nbytes, unsigned char dst[])
{
  int i, LFSR[NPAR+1],dbyte, j;
    
  for(i=0; i < NPAR+1; i++) LFSR[i]=0;

  for (i = 0; i < nbytes; i++) {
    dbyte = msg[i] ^ LFSR[NPAR-1];
    for (j = NPAR-1; j > 0; j--) {
      LFSR[j] = LFSR[j-1] ^ gmult(genPoly[j], dbyte);
    }
    LFSR[0] = gmult(genPoly[0], dbyte);
  }

  for (i = 0; i < NPAR; i++)
    pBytes[i] = LFSR[i];
    
  build_codeword(msg, nbytes, dst);
}

//。。。。。。。。。。。。。。。。。。。。。。。。。。。

//
///* Convert a binary string to array of bytes */
//void str2bytes(char *str, unsigned char *bytes, size_t str_len) {
//    for (size_t i = 0; i < str_len; i += 8) {
//        bytes[i/8] = strtol(str + i, NULL, 2);
//    }
//}
//
//
///* Convert a byte to a binary string */
//void byte2str(unsigned char byte, char *str) {
//    for (int i = 7; i >= 0; i--) {
//        str[7-i] = (byte & (1 << i)) ? '1' : '0';
//    }
//    str[8] = '\0';
//}



/* Main test function */
//int main() {
//    /* Initialize ECC */
//    initialize_ecc();
//
//    /* Input string (binary representation) */
//    char str[] = "10000010101010101001111110000110";
//    size_t str_len = strlen(str);
//
//
//    /* Calculate the number of bytes and allocate memory */
//    size_t num_bytes = str_len / 8;
//    unsigned char bytes[num_bytes];
//    unsigned char codeword[num_bytes + NPAR];  // Codeword will have extra NPAR bytes for parity
//
//    /* Convert input string to bytes */
//    str2bytes(str, bytes, str_len);
//
//    /* Encode data */
//    encode_data(bytes, num_bytes, codeword);
//
//    /* Print encoded data */
//    char str_byte[9];  // 8 bits + null terminator
//    for (int i = 0; i < num_bytes + NPAR; i++) {
//        byte2str(codeword[i], str_byte);
//        printf("encoded data:\n ")
//        printf("%s ", str_byte);
//    }
//    printf("\n");
//
//
//
//    /* Simulate an error (flip a bit) */
//    codeword[0] ^= 1;
//
//    /* Decode data */
//    decode_data(codeword, num_bytes + NPAR);
//
//    /* Check for errors */
//    if (check_syndrome() != 0) {
//        printf("Error detected!\n");
//
//        /* Try to correct errors */
//        int erasures[] = {-1};  // No known erasures
//        if (correct_errors_erasures(codeword, num_bytes + NPAR, 0, erasures) != 0) {
//            printf("Error corrected!\n");
//        } else {
//            printf("Error cannot be corrected.\n");
//        }
//    } else {
//        printf("No error detected.\n");
//    }
//
//    return 0;
//}







//
//int main() {
//    /* Initialize ECC */
//    initialize_ecc();
//
//    /* Input string (binary representation) */
//    char str[] = "10000010101010101001111110000110";
//    int str_len = strlen(str);
//
//    /* Calculate the number of bytes and allocate memory */
//    int num_bytes = str_len / 8;
//    unsigned char bytes[num_bytes];
//    unsigned char codeword[num_bytes + NPAR];  // Codeword will have extra NPAR bytes for parity
//
//    /* Convert input string to bytes */
//    str2bytes(str, bytes, str_len);
//
//    /* Encode data */
//    encode_data(bytes, num_bytes, codeword);
//
//    /* Simulate 4 random errors */
//    srand(time(NULL));
//    for (int i = 0; i < 4; i++) {
//        int index = rand() % (num_bytes + NPAR);
//        codeword[index] ^= 1;
//    }
//
//    /* Decode data */
//    decode_data(codeword, num_bytes + NPAR);
//
//    /* Check for errors */
//    if (check_syndrome() != 0) {
//        printf("Error detected!\n");
//
//        /* Try to correct errors */
//        int erasures[] = {-1};  // No known erasures
//        if (correct_errors_erasures(codeword, num_bytes + NPAR, 0, erasures) != 0) {
//            printf("Error corrected!\n");
//        } else {
//            printf("Error cannot be corrected.\n");
//        }
//    } else {
//        printf("No error detected.\n");
//    }
//    /* Print original data */
//    printf("Original Data: ");
//    for (int i = 0; i < num_bytes; i++) {
//        char str_byte[9];  // 8 bits + null terminator
//        byte2str(bytes[i], str_byte);
//        printf("%s ", str_byte);
//    }
//    printf("\n");
//
//    /* Print modified data */
//    printf("Modified Data: ");
//    for (int i = 0; i < num_bytes + NPAR; i++) {
//        char str_byte[9];  // 8 bits + null terminator
//        byte2str(codeword[i], str_byte);
//        printf("%s ", str_byte);
//    }
//    printf("\n");
//
//
//    /* Print decoded data */
//       printf("Decoded Data: ");
//       for (int i = 0; i < num_bytes; i++) {
//           byte2str(codeword[i], str);
//           printf("%s ", str);
//       }
//       printf("\n");
//
//    return 0;
//}
//
//int main() {
//    /* Initialize ECC */
//    initialize_ecc();
//
//    /* Input string (binary representation) */
//    char str[] = "11100010101010101001111110000110";
//    int str_len = strlen(str);
//
//    /* Calculate the number of bytes and allocate memory */
//    int num_bytes = str_len / 8;
//    unsigned char bytes[num_bytes];
//    unsigned char codeword[num_bytes + NPAR];  // Codeword will have extra NPAR bytes for parity
//
//    /* Convert input string to bytes */
//    str2bytes(str, bytes, str_len);
//
//    /* Encode data */
//    encode_data(bytes, num_bytes, codeword);
//
//    /* Simulate 4 random errors */
//    srand(time(NULL));
//    for (int i = 0; i < 4; i++) {
//        int index = rand() % (num_bytes + NPAR);
//        codeword[index] ^= 1;
//    }
//
//    /* Decode data */
//    decode_data(codeword, num_bytes + NPAR);
//
//    /* Check for errors */
//    if (check_syndrome() != 0) {
//        printf("Error detected!\n");
//
//        /* Try to correct errors */
//        int erasures[] = {-1};  // No known erasures
//        if (correct_errors_erasures(codeword, num_bytes + NPAR, 0, erasures) != 0) {
//            printf("Error corrected!\n");
//        } else {
//            printf("Error cannot be corrected.\n");
//        }
//    } else {
//        printf("No error detected.\n");
//    }
//
//    /* Print original data */
//    printf("Original Data: ");
//    for (int i = 0; i < num_bytes; i++) {
//        byte2str(bytes[i], str);
//        printf("%s ", str);
//    }
//    printf("\n");
//
//    /* Print modified data */
//    printf("Modified Data: ");
//    for (int i = 0; i < num_bytes + NPAR; i++) {
//        byte2str(codeword[i], str);
//        printf("%s ", str);
//    }
//    printf("\n");
//
//    /* Print decoded data */
//    printf("Decoded Data: ");
//    for (int i = 0; i < num_bytes; i++) {
//        byte2str(codeword[i], str);
//        printf("%s ", str);
//    }
//    printf("\n");
//
//    return 0;
//}
