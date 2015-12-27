#include "simulate_lcd.h"
#undef WM_PAINT /* WM_PAINT宏定义冲突 */
#include "GUI_Test.h"

#define SIM_LCD_WIDTH   480  /* 模拟屏幕宽度 */
#define SIM_LCD_HEIGHT  320  /* 模拟屏幕高度 */

static SIM_DEVICE sim_lcd;

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
            GUI_TouchPadSendValue((u_16)sim_lcd.tPad.x, (u_16)sim_lcd.tPad.y, GUI_TP_CHECKED);
        }
        sim_updata();
        Sleep(10);
    }
    return 0;
}

/* 模拟器开始运行 */
void simulate_lcd_start(void)
{
    CreateThread(NULL, 0, ThreadGUI, NULL, 0, NULL); /* 新建线程 */
    CreateThread(NULL, 0, ThreadDisp, NULL, 0, NULL); /* 新建线程 */
}

/* 屏幕模拟器初始化 */
void sim_lcd_init(void)
{
    sim_lcd.win_w = GetSystemMetrics(SM_CXSCREEN);
    sim_lcd.win_h = GetSystemMetrics(SM_CYSCREEN);
    sim_lcd.BufSize = sizeof(COLORREF) * sim_lcd.win_w * sim_lcd.win_h;
    /* 分配内存，储存像素数据 */
    sim_lcd.PixBuf = malloc(sim_lcd.BufSize);
    memset(sim_lcd.PixBuf, 0x00, sim_lcd.BufSize);
    /* 获取窗口句柄 */
    sim_lcd.hwnd = GetForegroundWindow();
    /* 获取客户区的DC */
    sim_lcd.hdc = GetDC(sim_lcd.hwnd);
    /* 为帧缓冲创建一个DC */
    sim_lcd.hFrame = CreateCompatibleDC(sim_lcd.hdc);
    /* 为客户区创建一个Bitmap */
    sim_lcd.hBmp = CreateCompatibleBitmap(sim_lcd.hdc, sim_lcd.win_w, sim_lcd.win_h);
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
    COLORREF r, g, b;
    
    r = Color & 0x000000ff;
    g = Color & 0x0000ff00;
    b = Color & 0x00ff0000;
    sim_lcd.PixBuf[y * sim_lcd.win_w + x] = r << 16 | g | b >> 16;
}

/* 屏幕模拟器读取像素 */
COLORREF sim_readPix(int x, int y)
{
    COLORREF r, g, b;

    b = sim_lcd.PixBuf[y * sim_lcd.win_w + x];
    r = b & 0x00ff0000;
    g = b & 0x0000ff00;
    b = b & 0x000000ff;
    return r >> 16 | g | b << 16;
}

/* 屏幕模拟器显示一个区域 */
void sim_dispArea(int x, int y, int Width, int Height, COLORREF *Buffer, int rowlen)
{
    int i, win_w = sim_lcd.win_w;
    COLORREF r, g, b;
    COLORREF *pLCD = sim_lcd.PixBuf;

    Buffer += y * rowlen + x; /* 偏移到第一个要显示的点的位置 */
    pLCD   += y * win_w + x; /* 偏移到第一个要显示的点的位置 */
    while (Height--) {
        for (i = 0; i < Width; ++i) {
            b = Buffer[i];
            r = b & 0x000000ff;
            g = b & 0x0000ff00;
            b = b & 0x00ff0000;
            pLCD[i] = r << 16 | g | b >> 16;
        }
        Buffer += rowlen;
        pLCD += win_w;
    }
}

/* 屏幕模拟器刷新显示,并更新状态 */
void sim_updata(void)
{
    SetBitmapBits(sim_lcd.hBmp, sim_lcd.BufSize, sim_lcd.PixBuf);
    BitBlt(sim_lcd.hdc, 0, 0,
           sim_lcd.win_w, sim_lcd.win_h,
           sim_lcd.hFrame, 0, 0,
           SRCCOPY);
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
    GUI_TouchPadSendValue((u_16)sim_lcd.tPad.x, (u_16)sim_lcd.tPad.y, GUI_TP_REMOVED);
    sim_lcd.padStatus = GUI_TP_REMOVED;
}
