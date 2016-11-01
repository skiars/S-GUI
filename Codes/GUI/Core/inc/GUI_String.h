#ifndef __GUI_STRING_H
#define __GUI_STRING_H

void GUI_Strlwr(char *str); /* 将字符串中的大写字母转换为小写 */
int GUI_Stricmp(const char *dst, const char *src);
char * GUI_Strcpy(char *dst, const char *src);
int GUI_Strlen(const char *Str);
void *GUI_Memset(void *s, unsigned char c, int n);

#endif /* __GUI_STRING_H */
