Modify parameters in BKW-Reduction.h as needed and compile with following command:

gcc -g BKW-Reduction.cpp LPN_Oracle.cpp -lm -lstdc++ -o <output_filename> -std=c++11 -fopenmp -Ofast

(Compiled via gcc 4.7.2)



gcc-13 -g BKW-Reduction.cpp LPN_Oracle.cpp -lm -lstdc++ -o BKM -std=c++11 -fopenmp -Ofast
