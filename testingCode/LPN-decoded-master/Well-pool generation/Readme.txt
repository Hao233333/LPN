Modify parameters in Sampling_Test.h as needed and compile with following command:

gcc -g Sampling_Test.cpp LPN_Oracle.cpp -lm -lstdc++ -o <output_filename> -std=c++11 -fopenmp -O3

(Compiled via gcc 4.7.2)

gcc-13 -g Sampling_Test.cpp LPN_Oracle.cpp -lm -lstdc++ -o WPG -std=c++11 -fopenmp -O3

gcc-13 -g MMT.cpp Permutation.cpp LPN_Oracle.cpp Matrixoperations.cpp -lm -lstdc++ -o MMT -std=c++11 -fopenmp -O3
