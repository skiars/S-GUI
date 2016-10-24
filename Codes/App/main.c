#include "sdlscreen.h"
#include "GUI.h"

#if GUI_DEBUG_MODE == 0
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

int main(void)
{
    sdl_init();
    return 0;
}
