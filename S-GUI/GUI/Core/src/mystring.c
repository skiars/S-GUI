#include "mystring.h"

/* 将字符串中的大写字母转换为小写 */
void mystrlwr(char *str)
{
    do {
        if (*str >= 'A' && *str <= 'Z')  *str += 32;
    } while (*str++);
}

/* 检查两个字符串是否相同，不区分大小写 */
int mystricmp(char *dst, char *src)
{
    int ch1, ch2;
    do {
        if (((ch1 = (unsigned char)(*dst++)) >= 'A') && (ch1 <= 'Z'))
            ch1 += 0x20;
        if (((ch2 = (unsigned char)(*dst++)) >= 'A') && (ch2 <= 'Z'))
            ch2 += 0x20;
    } while (ch1 && (ch1 == ch2));
    return (ch1 - ch2);
}
