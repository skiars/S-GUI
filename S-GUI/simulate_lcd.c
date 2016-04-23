#include "simulate_lcd.h"
#include "memalloc.h"
#include "GUI_Test.h"

#define SIM_LCD_WIDTH   480  /* 模拟屏幕宽度 */
#define SIM_LCD_HEIGHT  320  /* 模拟屏幕高度 */

char sim_mem1[1024 * 10];
char sim_mem2[1024 * 1000];

static SIM_DEVICE sim_lcd;

/* 暂时用一下 */
GUI_RESULT GUI_Init(void);
void GUI_Delay(GUI_TIME tms);

/* 更新缓冲区内容 */
static void sim_updata(void)
{
    SetBitmapBits(sim_lcd.hBmp, sim_lcd.BufSize, sim_lcd.PixBuf);
    BitBlt(sim_lcd.hdc, 0, 0,
        sim_lcd.win_w, sim_lcd.win_h,
        sim_lcd.hFrame, 0, 0,
        SRCCOPY);
}

/* GUI测试线程 */
static DWORD WINAPI ThreadGUI(LPVOID pM)
{
    GUI_Test();
    return 0;
}

/* 刷新显示线程 */
static DWORD WINAPI ThreadDisp(LPVOID pM)
{
    while (1) {
        /* 用鼠标模拟触屏 */
        if (sim_lcd.padStatus == GUI_TP_CHECKED) {
            GetCursorPos(&sim_lcd.tPad); /* 获取鼠标位置 */
            ScreenToClient(sim_lcd.hwnd, &sim_lcd.tPad);
            GUI_TouchPadSendValue((u_16)sim_lcd.tPad.x,
                (u_16)sim_lcd.tPad.y, GUI_TP_CHECKED);
        }
        if (sim_lcd.updata) {
            sim_updata();
            sim_lcd.updata = 0;
        }
        Sleep(20);
    }
    return 0;
}

/* 模拟器开始运行 */
void simulate_lcd_start(HWND hWnd)
{
    Mem_PoolInit(sim_mem1, sizeof(sim_mem1));
    Mem_PoolInit(sim_mem2, sizeof(sim_mem2));
    /* 获取窗口句柄 */
    sim_lcd.hwnd = hWnd;
    CreateThread(NULL, 0, ThreadGUI, "ThreadGUI", 0, NULL); /* 新建线程 */
    CreateThread(NULL, 0, ThreadDisp, "ThreadDisp", 0, NULL); /* 新建线程 */
}

/* 屏幕模拟器初始化 */
void sim_lcd_init(void)
{
    sim_lcd.win_w = GetSystemMetrics(SM_CXSCREEN);
    sim_lcd.win_h = GetSystemMetrics(SM_CYSCREEN);
    sim_lcd.BufSize = sizeof(COLORREF) * sim_lcd.win_w * sim_lcd.win_h;
    sim_lcd.updata = 0;
    /* 分配内存，储存像素数据 */
    sim_lcd.PixBuf = malloc(sim_lcd.BufSize);
    memset(sim_lcd.PixBuf, 0x00, sim_lcd.BufSize);
    /* 获取客户区的DC */
    sim_lcd.hdc = GetDC(sim_lcd.hwnd);
    /* 为帧缓冲创建一个DC */
    sim_lcd.hFrame = CreateCompatibleDC(sim_lcd.hdc);
    /* 为客户区创建一个Bitmap */
    sim_lcd.hBmp = CreateCompatibleBitmap(sim_lcd.hdc,
        sim_lcd.win_w, sim_lcd.win_h);
    /* 为帧缓冲的DC选择client_bitmap作为对象 */
    SelectObject(sim_lcd.hFrame, sim_lcd.hBmp);
}

/* 获取虚拟屏幕宽度 */
int sim_getWidth(void)
{
    return SIM_LCD_WIDTH;
}

/* 获取虚拟屏幕高度 */
int sim_getHeight(void)
{
    return SIM_LCD_HEIGHT;
}

/* 屏幕模拟器显示像素 */
void sim_drawPix(int x, int y, COLORREF Color)
{
    sim_lcd.PixBuf[y * sim_lcd.win_w + x] = Color;
    sim_lcd.updata = 1;
}

/* 屏幕模拟器读取像素 */
COLORREF sim_readPix(int x, int y)
{
    return sim_lcd.PixBuf[y * sim_lcd.win_w + x];
}

/* 屏幕模拟器显示一个区域 */
void sim_dispArea(int x,
    int y,
    int Width,
    int Height,
    COLORREF *Buffer,
    int rowlen)
{
    int i, win_w = sim_lcd.win_w;
    COLORREF *p1, *p2, *pLCD = sim_lcd.PixBuf;

    Buffer += y * rowlen + x; /* 偏移到第一个要显示的点的位置 */
    pLCD   += y * win_w + x; /* 偏移到第一个要显示的点的位置 */
    while (Height--) {
        p1 = pLCD;
        p2 = Buffer;
        for (i = 0; i < Width; ++i) {
            *p1++ = *p2++;
        }
        Buffer += rowlen;
        pLCD += win_w;
    }
    sim_updata();
}

/* 触摸设备按下 */
void sim_pad_cleck(void)
{
    GetCursorPos(&sim_lcd.tPad); /* 获取鼠标位置 */
    ScreenToClient(sim_lcd.hwnd, &sim_lcd.tPad);
    sim_lcd.padStatus = GUI_TP_CHECKED;
}

/* 触摸设备松开 */
void sim_pad_removed(void)
{
    GUI_TouchPadSendValue((u_16)sim_lcd.tPad.x,
        (u_16)sim_lcd.tPad.y, GUI_TP_REMOVED);
    sim_lcd.padStatus = GUI_TP_REMOVED;
}

/* 键盘值传递，作用：将Win32键盘输入键值传递到S-GUI */
void sim_key_send(WPARAM KeyVal, UINT Status)
{
    u_8 Key;

    /* 键值转换 */
    switch (KeyVal) {
    case VK_UP:
        Key = KEY_UP;
        break;
    case VK_DOWN:
        Key = KEY_DOWN;
        break;
    case VK_LEFT:
        Key = KEY_LEFT;
    case VK_RIGHT:
        Key = KEY_RIGHT;
        break;
    case VK_SHIFT:
        /* 第二功能键处理 */
        Key = KEY_SHIFT;
        break;
    default:
        Key = (u_8)KeyVal; /* 其他键直接赋值 */
    }
    if (Status == WM_KEYDOWN) {
        GUI_SendKey(Key, GUI_KEYDOWN);
    } else if (Status == WM_KEYUP) {
        GUI_SendKey(Key, GUI_KEYUP);
    }
}

void sim_setmouse(short x, short y)
{
    sim_lcd.tPad.x = x;
    sim_lcd.tPad.y = y;
    if (x > 0 && x < SIM_LCD_WIDTH && y > 0 && y < SIM_LCD_HEIGHT) {
        //ShowCursor(FALSE);
    } else {
        //ShowCursor(TRUE);
    }
}
