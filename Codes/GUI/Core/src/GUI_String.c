#include "GUI_String.h"

/* 将字符串中的大写字母转换为小写 */
void GUI_Strlwr(char *str)
{
    do {
        if (*str >= 'A' && *str <= 'Z')  *str += 32;
    } while (*str++);
}

/* 检查两个字符串是否相同，不区分大小写 */
int GUI_Stricmp(const char *dst, const char *src)
{
    int ch1, ch2;

    do {
        if (((ch1 = (unsigned char)(*src++)) >= 'A') && (ch1 <= 'Z'))
            ch1 += 0x20;
        if (((ch2 = (unsigned char)(*dst++)) >= 'A') && (ch2 <= 'Z'))
            ch2 += 0x20;
    } while (ch1 && (ch1 == ch2));
    return (ch1 - ch2);
}

/* 字符串拷贝 */
char * GUI_Strcpy(char *dst, const char *src)
{
    char c;  
    char *s = (char *)src;  
    const int off = dst - s - 1;
    
     do {  
          c = *s++;
          s[off] = c;  
     } while (c != '\0');
     return dst;  
}

/* 计算字符串长度 */
int GUI_Strlen(const char *Str)
{
    int i = 0;

    while (*Str++) {
        ++i;
    }
    return i;
}

void *GUI_Memset(void *s, unsigned char c, int n)
{
    const unsigned char uc = c;
    unsigned char *su;

    for (su = s; 0 < n; ++su, --n) {
        *su = uc;
    }
    return s;
}
