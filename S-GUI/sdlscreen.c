#include "sdlscreen.h"
#include "SDL.h"
#include "GUI_Test.h"

static SDL_Surface *screen = NULL;
static int MouseDownFlag;

int GUIThread(void *p);
void MessageLoop(void);
Uint32 TimerCb(Uint32 interval, void *param);
void Key_Send(int KeyVal, char Status);

/* 初始化 */
void sdl_init(void)
{
    SDL_Thread *guiThread;
    SDL_TimerID TimerId;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("Unable to init SDL: %s\n", SDL_GetError());
        return;
    }
    screen = SDL_SetVideoMode(HAL_SCREEN_W, HAL_SCREEN_H, 32, SDL_SWSURFACE);
    TimerId = SDL_AddTimer(0, TimerCb, NULL);
    SDL_WM_SetCaption("S-GUI Demo", NULL);
    guiThread = SDL_CreateThread(GUIThread, NULL);
    MessageLoop();
    SDL_RemoveTimer(TimerId);
    SDL_KillThread(guiThread);
    SDL_FreeSurface(screen);
    SDL_Quit();
}

/* GUI线程 */
int GUIThread(void *p)
{
    GUI_Test();
    return 0;
}

/* 定时器 */
Uint32 TimerCb(Uint32 interval, void *param)
{
    static int x, y;

    SDL_Flip(screen);
    if (MouseDownFlag) {
        SDL_GetMouseState(&x, &y);
        GUI_TouchPadSendValue((u_16)x, (u_16)y, GUI_TP_CHECKED);
    } else {
        GUI_TouchPadSendValue((u_16)x, (u_16)y, GUI_TP_REMOVED);
    }
    return 10;
}

/* 消息循环 */
void MessageLoop(void)
{
    int quit = 1;
    SDL_Rect rect;
    SDL_Event event;

    rect.x = 0;
    rect.y = 0;
    rect.w = HAL_SCREEN_W;
    rect.h = HAL_SCREEN_H;
    while (quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                MouseDownFlag = 1;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            case SDL_BUTTON_MIDDLE:
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
            case SDL_BUTTON_LEFT:
                MouseDownFlag = 0;
                break;
            case SDL_BUTTON_RIGHT:
                break;
            case SDL_BUTTON_MIDDLE:
                break;
            }
            break;
        case SDL_KEYDOWN:
            Key_Send(event.key.keysym.sym, 1);
            break;
        case SDL_KEYUP:
            Key_Send(event.key.keysym.sym, 0);
            break;
        case SDL_QUIT:
            quit = 0;
            break;
        default:
            break;
        }
    }
}

/* 键盘值传递，作用：将Win32键盘输入键值传递到S-GUI */
void Key_Send(int KeyVal, char Status)
{
    u_8 Key;

    /* 键值转换 */
    switch (KeyVal) {
    case SDLK_UP:
        Key = KEY_UP;
        break;
    case SDLK_DOWN:
        Key = KEY_DOWN;
        break;
    case SDLK_LEFT:
        Key = KEY_LEFT;
    case SDLK_RIGHT:
        Key = KEY_RIGHT;
        break;
    case SDLK_LSHIFT:
        /* 第二功能键处理 */
        Key = KEY_SHIFT;
        break;
    case SDLK_RSHIFT:
        /* 第二功能键处理 */
        Key = KEY_SHIFT;
        break;
    default:
        Key = (u_8)KeyVal; /* 其他键直接赋值 */
    }
    if (Status == 1) {
        GUI_SendKey(Key, GUI_KEYDOWN);
    } else {
        GUI_SendKey(Key, GUI_KEYUP);
    }
}

/* 写像素，内部调用 */
static void SDL_PixelNolock(SDL_Surface *surface, int x, int y, Uint32 color)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
    case 4:
        *(Uint32 *)p = color;
        break;
    case 1:
        *p = color;
        break;
    case 2:
        *(Uint16 *)p = (Uint16)color;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0x0f;
        } else {
            p[0] = color & 0x0f;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
        break;
    }
}

/* 写像素，内部调用 */
static void SDL_PixelColor(SDL_Surface *surface, int x, int y, Uint32 color)
{
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0) {
            return;
        }
    }
    SDL_PixelNolock(surface, x, y, color);
}

/* 写像素 */
void HAL_SetPixel(int x, int y, UINT32 Color)
{
    SDL_PixelColor(screen, x, y, Color);
}

/* 读取像素 */
UINT32 HAL_GetPixel(int x, int y)
{
    int bpp = screen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    return *(UINT32 *)p;
}

/* 填充矩形 */
void HAL_FillRect(int x0, int y0, int x1, int y1, UINT32 Color)
{
    SDL_Rect r;

    r.x = x0;
    r.y = y0;
    r.w = x1 - x0 + 1;
    r.h = y1 - y0 + 1;
    SDL_FillRect(screen, &r, Color);
}

/* 绘制RGB888位图 */
void _drawBitmap24b(const unsigned char *pPixel,
    int x0, int y0, int xSize, int ySize, int Offset)
{
    int bpp = screen->format->BytesPerPixel;
    int i, win_w = HAL_SCREEN_W, offset = win_w - xSize;
    UINT32 *pLCD = screen->pixels, Color;

    if (bpp != 4) {
        return;
    }
    Offset *= 3;
    pLCD += y0 * win_w + x0; /* 偏移到第一个要显示的点的位置 */
    while (ySize--) {
        for (i = 0; i < xSize; ++i) {
            Color = *pPixel++;
            Color |= *pPixel++ << 8;
            Color |= *pPixel++ << 16;
            *pLCD++ = Color;
        }
        pLCD += offset;
        pPixel += Offset;
    }
}

/* 绘制位图 */
void HAL_DrawBitmap(int ColorFormat, const unsigned char *pPixel,
    int x0, int y0, int xSize, int ySize, int Offset)
{
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }
    if (ColorFormat == HAL_RGB888) {
        _drawBitmap24b(pPixel, x0, y0, xSize, ySize, Offset);
    }
}
