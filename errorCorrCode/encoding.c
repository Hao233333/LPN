
#include "encoding.h"
#include <stdio.h>
#include <stdbool.h>

//clang -o encode encoding.c -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./encode

int parity(int n, unsigned char *arr) {
    unsigned char p = 0;
    for (int i = 0; i < n; i++) {//如果不是校验位的话，跳过；是的话，i也增大
        if (i != (1 << p) - 1) {//1 << p相当于计算2^p。
            //-1: because index starts from 0 rather than 1
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


void encode(unsigned char *data, int n, unsigned char *encoded) {//n is bits of data
    unsigned char x = 0;
    while ((1 << x) < n + x + 1) {// 2^x> n + x + 1, x是校验位数量，用于找到需要多少个校验bits
        x++;
    }
    unsigned char j = 0;
    for (int i = 0; i < n + x; i++) {//用于插入校验位，默认为0
        if ((i & (i + 1)) == 0) { //举例：0111 & 1000 = 0，0111是7，index starts from 0,
            //encoded[7]是第8位。用于找到所有的2^x 位
            encoded[i] = 0;
        }
        else {
            encoded[i] = data[j];
            j++;
        }
    }
    for (int i = 0; i < x; i++) {//根据偶校验赋值为1或0
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
    unsigned char n = 4; // data bits
    unsigned char x = 3; // number of redundant bits
    unsigned char data[4] = {1, 0, 1, 1}; // data to be transmitted
    unsigned char encoded[n + x]; // encoded data
    unsigned char received[n]; // place to hold the decoded data
    
    // Initialize encoded array to 0
    for(int i = 0; i < n + x; i++){
        encoded[i] = 0;
    }
    
    // Encoding the data
    encode(data, n, encoded);
    
    // Printing encoded data
    printf("Encoded data: ");
    for (int i = 0; i < n + x; i++) {
        printf("%u", encoded[i]);
    }
    printf("\n");
    
    
    
    unsigned char received1[7]={1,0,1,1,0,1,1};
    unsigned char output1[3];
    // Decoding the data
    //bool success = decode(received1, n, encoded);
    bool success = decode(output1, n, received1);
    // Check if decoding was successful
    if (success) {
        // Printing decoded data
        printf("Decoded data: ");
        for (int i = 0; i < n; i++) {
            printf("%u", output1[i]);
        }
        printf("\n");
    } else {
        printf("Error in decoding data.\n");
    }
    
    return 0;
}




//int main() {
//    unsigned char n = 4; // data bits
//    unsigned char x = 3; // 校验位数量
//    unsigned char data[4] = {1, 0, 1, 1}; // data to be transmitted
//    unsigned char encoded[n+x] = {0}; // encoded data
//    // remainder of code
//
//    return 0;
//}



//int main() {
//    unsigned char n = 4; // data bits
//    unsigned char x = 3; // number of redundant bits
//    unsigned char data[4] = {1, 0, 1, 1}; // data to be transmitted
//    unsigned char encoded[n + x]; // encoded data
//
//    // Initialize encoded array to 0
//    for(int i = 0; i < n + x; i++){
//        encoded[i] = 0;
//    }
//
//    // Encoding the data
//    encode(data, n, encoded);
//
//    // Printing encoded data
//    printf("Encoded data: ");
//    for (int i = 0; i < n + x; i++) {
//        printf("%u", encoded[i]);
//    }
//    printf("\n");
//
//
//
//    return 0;
//}
