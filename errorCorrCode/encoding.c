
#include "encoding.h"
#include <stdio.h>
#include <stdbool.h>


int parity(int n, unsigned char *arr) {
    unsigned char p = 0;
    for (int i = 0; i < n; i++) {
        if (i != (1 << p) - 1) {//1 << p相当于计算2^p
            p++;
        }
        else {
            p++;
            i += (1 << p) - 2; //1 << p = 2^p，左移一位，二进制增大2
        }
    }
    unsigned char sum = 0;
    for (int i = 0; i < n; i++) {
        if ((i & (1 << (p - 1))) == 0) {
            sum ^= arr[i];
        }
    }
    return sum;
}


void encode(unsigned char *data, int n, unsigned char *encoded) {
    unsigned char x = 0;
    while ((1 << x) < n + x + 1) {
        x++;
    }
    unsigned char j = 0;
    for (int i = 0; i < n + x; i++) {
        if ((i & (i + 1)) == 0) {
            encoded[i] = 0;
        }
        else {
            encoded[i] = data[j];
            j++;
        }
    }
    for (int i = 0; i < x; i++) {
        unsigned char p = parity(n + x, encoded);
        encoded[(1 << i) - 1] = p;
    }
}

int decode(unsigned char *data, int n, unsigned char *received) {
    unsigned char x = 0;
    while ((1 << x) < n + x + 1) {
        x++;
    }
    unsigned char error = 0;
    for (int i = 0; i < x; i++) {
        unsigned char p = parity(n + x, received);
        if (p != received[(1 << i) - 1]) {
            error += (1 << i) - 1;
        }
    }
    if (error != 0) {
        received[error - 1] ^= 1;
    }
    unsigned char j = 0;
    for (int i = 0; i < n + x; i++) {
        if ((i & (i + 1)) != 0) {
            data[j] = received[i];
            j++;
        }
    }
    return error == 0;
}

int main() {
    unsigned char n = 4; // size of data
    unsigned char x = 3; // number of redundant bits
    unsigned char data[4] = {1, 0, 1, 1}; // data to be transmitted
    unsigned char encoded[7] = {0}; // encoded data
    // remainder of code
    return 0;
}
