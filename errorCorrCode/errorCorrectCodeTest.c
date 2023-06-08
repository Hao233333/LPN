#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "config.h"

#define N N_VALUE
#define M M_VALUE
#define r R_VALUE

void reverse(unsigned char *str)//让输入的字符串前后对调。
{
    int n = N;
    for(int i = 0; i < n / 2; i++)
    {
        unsigned char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

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
                if(msg[j] == '1') count++;
            }
        }

        if(count & 1) msg[i] = '1';//如果 count 是奇数，那么将 msg 的第 i 位设为 '1'，
        else msg[i] = '0';//否则设为 '0'。这是奇偶校验的规则
        bit++;
    }
}

void showMsg(unsigned char* msg, int m, int x)
{
    printf("the data packet to be sent is: ");
    for(int i = m+x; i >= 1; i--)
    {
        printf("%c ", msg[i]);
    }
    printf("\n");
}

void receiver(unsigned char* msg, int m, int x)
        //用于检测接收的消息（包括数据和冗余位）中是否存在错误
{
    unsigned char* ans = (unsigned char*)calloc(x+1, sizeof(unsigned char));
    int bit = 0;
    for(int i = 1 ; i <= m+x ; i*=2)
    {
        int count = 0;
        for(int j = i+1 ; j<=m+x ; j++)//用于遍历 msg 中所有的数据位和冗余位，以计算第 i 位的奇偶校验。
        {
            if(j & (1 << bit))//如果 j 在二进制形式下的第 bit 位为 1，那么执行下面的语句。这里用到了位操作。
            {
                if(msg[j] == '1') count++;//如果 msg 的第 j 位为 '1'，那么 count 加 1。
            }
        }

        if(count & 1)//如果 count 是奇数，那么执行下面的语句
        {
            if(msg[i] == '1') ans[bit] = '0';//如果 msg 的第 i 位为 '1'，那么将 ans 的第 bit 位设为 '0'，
            else ans[bit] = '1';//否则设为 '1'。
        }
        else
        {
            if(msg[i]=='0') ans[bit] = '0';//如果 msg 的第 i 位为 '0'，那么将 ans 的第 bit 位设为 '0'
            else ans[bit] = '1';//否则设为 '1
        }
        bit++;
    }

    if(strchr((char *)ans, '1'))//如果 ans 中存在 '1'，说明存在错误
    {
        int wrongbit = 0;//如果 ans 中存在 '1'，说明存在错误
        for(int i = 0 ; i < strlen((char *)ans) ; i++)
        {
            if(ans[i]=='1') wrongbit += pow(2, i);//如果 ans 的第 i 位为 '1'，那么将 2 的 i 次幂加到 wrongbit 上
        }
        printf("bit number %d is wrong and having error\n", wrongbit);
    }
    else//如果 ans 中不存在 '1'，说明没有错误
    {
        printf("correct data packet received\n");
    }

    free(ans);
}

int main()
{
    unsigned char data[] = "1011001";
    reverse(data);
    int m = 7;
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
    showMsg(msg, m, x);
    receiver(msg, m, x);

    free(msg);
    return 0;
}
