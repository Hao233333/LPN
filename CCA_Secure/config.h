
//this file is used to modify the value of N M l and t
#include <math.h>

#ifndef config_h
#define config_h

//#define N_VALUE 9000
//#define R_VALUE 0.0044

#define N_VALUE 256 //rows 10k,
#define M_VALUE 264 //cols, m > n + log_2(n)
#define T_VALUE 0.01//tau
#define L_VALUE 490 //L code length = bch(n)
#define K_VALUE 256

//#define N_VALUE 1024 //rows 10k,
//#define M_VALUE 1034 //cols, m = n + log_2(n)
//#define T_VALUE 0.0102//tau
//#define L_VALUE 2042 //L code length = bch(n)
//#define K_VALUE 128





//#define N_VALUE 9000 //rows 10k,
//#define M_VALUE N_VALUE + log(N_VALUE) //cols, m > n + log_2(n)
//#define T_VALUE 0.001//tau
//#define L_VALUE 962 //L code length = bch(n)
//#define K_VALUE 512

//#define N_VALUE 4 //rows
//#define M_VALUE 8 //cols, m > n + log_2(n)
//#define T_VALUE 0.2//t
//#define L_VALUE 4 //L code length = bch(n)
//#define K_VALUE 2

//tau = 0 but no security,
//128 256 security level


// sqrt(N_VALUE)
#endif /* config_h */

//
