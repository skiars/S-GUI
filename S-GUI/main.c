#include "sdlscreen.h"

/* 不显示控制台窗口,GCC编译时用-mwindows */
#ifdef _WIN32
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

int main(void)
{
    sdl_init();
    return 0;
}
