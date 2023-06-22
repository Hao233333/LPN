#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "../calculation.h"
//#define n 4
//

//clang -o encode2 errorCorrectCodeTest.c  -I/opt/homebrew/opt/openssl@3/include -L/opt/homebrew/opt/openssl@3/lib -lssl -lcrypto

//./encode2

//void reverse(unsigned char *str)
////让输入的字符串前后对调。因为通常从右到左读取二进制字符串，
////但在字符串表示中，最左边的字符在数组的0索引处。
////是否需要？
//{
//    int n = N;
//    for(int i = 0; i < n / 2; i++)
//    {
//        unsigned char temp = str[i];
//        str[i] = str[n - i - 1];
//        str[n - i - 1] = temp;
//    }
//}

void setRedundantBits(unsigned char* msg, int m, int x)
{
    int bit = 0;
    for(int i = 1; i <= m+x; i*=2)
    {
        int count = 0;
        for(int j = i+1; j <= m+x; j++)
        {
            if(j & (1 << bit))//如果 j 在二进制形式下的第 bit 位为 1，那么继续执行。（这里用到了位操作）
            {
                if(msg[j] == '1')
                    count++;
            }
        }
        
        if(count & 1) msg[i] = '1';//如果 count 是奇数，那么将 msg 的第 i 位设为 '1'，
        else msg[i] = '0';//否则设为 '0'。这是奇偶校验的规则
        bit++;
    }
}

//void showMsg(unsigned char* msg, int m, int x)
//{
//    for(int i = m+x; i >= 1; i--)
//    {
//        printf("%c ", msg[i]);
//    }
//    printf("\n");
//}
//
void showMsg(unsigned char* msg, int m, int x)
{
    for(int i = 1; i <= m+x; i++)
    {
        printf("%c ", msg[i]);
    }
    printf("\n");
}


//-----
void flipCharAtPosition(unsigned char* msg, int position) {
    if (msg[position] == '1')
        msg[position] = '0';
    else if (msg[position] == '0')
        msg[position] = '1';
}

//-----

void receiver(unsigned char* msg, int m, int x)
//用于检测接收的消息（包括数据和冗余位）中是否存在错误，并尝试进行纠正
{
    unsigned char* ans = (unsigned char*)calloc(x+1, sizeof(unsigned char));
    int bit = 0;
    for(int i = 1 ; i <= m+x ; i*=2)
    {
        int count = 0;
        for(int j = i+1 ; j<=m+x ; j++)
        {
            if(j & (1 << bit))
            {
                if(msg[j] == '1') count++;
            }
        }
        
        if(count & 1)
        {
            if(msg[i] == '1') ans[bit] = '0';
            else ans[bit] = '1';
        }
        else
        {
            if(msg[i]=='0') ans[bit] = '0';
            else ans[bit] = '1';
        }
        bit++;
    }
    
    if(strchr((char *)ans, '1'))//if there are 1s in ans, it means there is a error
    {
        int wrongbit = 0;
        for(int i = 0 ; i < strlen((char *)ans) ; i++)
        {
            if(ans[i]=='1') wrongbit += pow(2, i);
        }
        printf("bit number %d is flipped\n\n", wrongbit);

        // correct error, just flip the bit
        if (msg[wrongbit] == '1')
            msg[wrongbit] = '0';
        else
            msg[wrongbit] = '1';

        // correct message
        printf("Corrected message: \n");
        showMsg(msg, m, x);
    }
    else
    {
        printf("data is correct\n");
    }
    
    free(ans);
}



unsigned char* decode(unsigned char* msg, int m, int x) {
    unsigned char* decoded_msg = (unsigned char*)malloc((m+1)*sizeof(unsigned char)); // 分配空间
    decoded_msg[m] = '\0'; // 确保字符串以 null 结尾
    int curr = 0;
    for(int i = 1; i <= m+x; i++) {
        if(i & (i-1)) { // 仅处理非冗余位
            decoded_msg[curr++] = msg[i];
        }
    }
    return decoded_msg;
}


int main()
{
    
//    //generate message vector v--m bits
//    unsigned char *data = (unsigned char *)malloc(n * sizeof(unsigned char));
//
//    if (data == NULL) {
//        printf("Error: Unable to allocate memory for the vector\n");
//        exit(1);
//    }
//
//    generate_binary_vector(data, n);
    
    
    unsigned char data[] = "101101";//add linear code, rend log_2(n)+1
        //some other alt linear code;encrypt < 256 bit; Hybird encryption; 128 * 8 reden bits;
    //
    
//    reverse(data);
    int m = (int)strlen((char*)data);
    int x = 0;
    int power = 1;
    
    while(power < (m + x + 1))
    {
        x++;
        power *= 2;
    }
    
    unsigned char* msg = (unsigned char*)malloc((m+x+1)*sizeof(unsigned char));
    msg[m+x] = '\0';
    
    int curr = 0;
    for(int i = 1 ; i <= m+x ; i++)
    {
        if(i & (i-1))
        {
            msg[i] = data[curr++];
        }
        else msg[i] = 'n';
    }
    
    setRedundantBits(msg, m, x);
    printf("Encodeed code:\n");
    showMsg(msg, m, x);


    flipCharAtPosition(msg, m + x - 3 + 1);//左数第2个数
    printf("Encodeed code with 1 bit error:\n");
    showMsg(msg, m, x);

    
    receiver(msg, m, x);
    
    unsigned char* decoded_msg = decode(msg, m, x);
    printf("Original message:%s\n",data);
    printf("Decoded message: %s\n", decoded_msg);

    free(decoded_msg);
    free(msg);
    return 0;
}
