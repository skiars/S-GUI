#include "GUI_2D.h"
#include "GUI.h"
#include "math.h"

u_16 GUI_RGB888To565(u_32 RGB)
{
    u_16 R,G,B;
    B = (RGB >> 19) & 0x1F;
    G = (RGB >> 10) & 0x3F;
    R = (RGB >>  3) & 0x1F;
    return (R << 11) | (G << 5) | B;   
}

u_32 GUI_RGB565To888(u_16 RGB)
{
    u_32 Color;
    Color  = ((u_32)RGB & 0x001F) << 19;  //B
    Color |= ((u_32)RGB & 0x07E0) << 5;   //G
    Color |= ((u_32)RGB & 0xF800) >> 8;   //R
    return Color;
}

/*
*Alpha混合
*BkColor:背景颜色
*Color:目标颜色
*Alpha:透明度(0~256)
*/
GUI_COLOR GUI_AlphaBlend(GUI_COLOR Color, GUI_COLOR BkColor, u_16 Alpha)
{
    /* Calc Color seperations for FgColor first */
    GUI_COLOR R = (BkColor & 0xFF)    * Alpha;
    GUI_COLOR G = (BkColor & 0xFF00)  * Alpha;
    GUI_COLOR B = (BkColor & 0xFF0000)* Alpha;
    /* Add Color seperations for BkColor */
    Alpha = 256 - Alpha;
    R += (Color & 0xFF)     * Alpha;
    G += (Color & 0xFF00)   * Alpha;
    B += (Color & 0xFF0000) * Alpha;
    R = (R >> 8);
    G = (G >> 8) & 0xFF00;
    B = (B >> 8) & 0xFF0000;
    return R | G | B;
}


//画裁剪点
void GUI_DrawTailorPoint(i_16 x,i_16 y,GUI_COLOR Color)
{
    GUI_RECT r1, r2;

    GUI_Val2Rect(&r1, x, y, 1, 1);
    GUI_DrawAreaInit();
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1) == GUI_OK) {
            if (Color >> 24) {
                Color = GUI_AlphaBlend(Color, GUI_ReadPixel(x, y), Color >> 24);
            }
            GUI_DrawPixel(x, y, Color);
        }
    }
}

/* 读取裁剪点 */
GUI_COLOR GUI_ReadTailorPoint(i_16 x, i_16 y)
{
    GUI_RECT *r;
    
    r = GUI_GetNowArea();
    if (x >= r->x0 && x <= r->x1 &&
        y >= r->y0 && y <= r->y1) {
        return GUI_ReadPixel(x, y);
    } else {
        return 0x00000000;
    }
}

/* 画有透明效果的垂直线 */
void GUI_VertLineAlpha(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;
    
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x0, y0 + len), Alpha);
        GUI_DrawPixel(x0, y0 + len, tColor);
    }    
}

/* 画垂直线 */
void GUI_VertLine(i_16 x0, i_16 y0, u_16 len, GUI_COLOR Color)
{
    while (len--) {
        GUI_DrawPixel(x0, y0 + len, Color);
    }    
}

//画垂直裁剪线
//x0,y0:坐标
//len:线长度
//color:颜色
void GUI_VertTailorLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1, r2;

    if (GUI_Val2Rect(&r1, x0, y0, 1, len) == GUI_ERR) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit();
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1) == GUI_OK) {
            x0 = r2.x0;
            y0 = r2.y0;
            len = r2.y1 - r2.y0 + 1;
            if (Color >> 24) {
                GUI_VertLineAlpha(x0, y0, len, Color);
            } else {
                GUI_VertLine(x0, y0, len, Color);
            }
        }
    }
}

/*  
*画有透明效果的水平线
*/
void GUI_HoriLineAlpha(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor;
    while (len--) {
        tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x0 + len, y0), Alpha);
        GUI_DrawPixel(x0 + len, y0, tColor);
    }
}

//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void GUI_HoriLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    while (len--) {
        GUI_DrawPixel(x0 + len, y0, Color);
    }    
}

//画水平裁剪线
//x0,y0:坐标
//len:线长度
//color:颜色
void GUI_HoriTailorLine(i_16 x0,i_16 y0,u_16 len,GUI_COLOR Color)
{
    GUI_RECT r1, r2;

    if (GUI_Val2Rect(&r1, x0, y0, len, 1) == GUI_ERR) {
        return; /* 长度为0 */
    }
    GUI_DrawAreaInit();
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1) == GUI_OK) {
            x0 = r2.x0;
            y0 = r2.y0;
            len = r2.x1 - r2.x0 + 1;
            if (Color >> 24) {
                GUI_HoriLineAlpha(x0, y0, len, Color);
            } else {
                GUI_HoriLine(x0, y0, len, Color);
            }
        }
    }
}

//画矩形框
void GUI_DrawRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color)
{
    GUI_HoriLine(x0, y0, xSize, Color);
    GUI_HoriLine(x0, y0 + ySize - 1, xSize, Color);
    GUI_VertLine(x0, y0, ySize, Color);
    GUI_VertLine(x0 + xSize - 1, y0, ySize, Color);
}

//画裁剪矩形框
void GUI_DrawTailorRect(i_16 x0,i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color)
{
    GUI_HoriTailorLine(x0 + 1, y0, xSize - 2, Color);
    GUI_HoriTailorLine(x0 + 1, y0 + ySize - 1, xSize - 2, Color);
    GUI_VertTailorLine(x0, y0, ySize, Color);
    GUI_VertTailorLine(x0 + xSize - 1, y0, ySize, Color);
}

void GUI_FillRectAlpha(i_16 x0, i_16 y0,u_16 xSize,u_16 ySize,GUI_COLOR Color)
{
    i_16 x, y;
    u_16 Alpha = Color >> 24;
    GUI_COLOR tColor; 
    
    xSize += x0;
    ySize += y0;
    for (y = y0; y < ySize; y++) {
        for (x = x0; x < xSize; x++) {
            tColor = GUI_AlphaBlend(Color, GUI_ReadPixel(x, y), Alpha);
            GUI_DrawPixel(x, y, tColor);
        }
    }
}

//填充矩形
void GUI_FillRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, GUI_COLOR Color)
{                                     
    i_16 x, y;
    
    xSize += x0;
    ySize += y0;
    for (y = y0; y < ySize; y++) {
        for (x = x0; x < xSize; x++) {
            GUI_DrawPixel(x, y, Color);
        }
    }
}

//填充裁剪矩形
void GUI_FillTailorRect(i_16 x0, i_16 y0, u_16 xSize, u_16 ySize, GUI_COLOR Color)
{                                     
    GUI_RECT r1, r2;
    
    /* 将矩形坐标转换为结构体 */
    if (GUI_Val2Rect(&r1, x0, y0, xSize, ySize) == GUI_ERR) {
        return;//非法
    }
    GUI_DrawAreaInit();
    while (GUI_GetNextArea(&r2)) { /* 遍历所有的显示区域 */
        if (GUI_RectOverlay(&r2, &r2, &r1) == GUI_OK) {
            x0 = r2.x0;
            y0 = r2.y0;
            xSize = r2.x1 + 1 - x0;
            ySize = r2.y1 + 1 - y0;
            if (Color >> 24) {
                GUI_FillRectAlpha(x0, y0, xSize, ySize, Color);
            } else {
                GUI_FillRect(x0, y0, xSize, ySize, Color);
            }
        }
    }
}

//画圆角矩形/填充圆角矩形
//x,y,width,height:圆角矩形的位置和尺寸
//r:圆角的半径.
//mode:0,画矩形框;1,填充矩形.
//upcolor:上半部分颜色
//downcolor:下半部分颜色
void GUI_DrawArcRect(u_16 x,u_16 y,u_16 width,u_16 height,u_8 r,u_8 mode,GUI_COLOR upcolor,GUI_COLOR downcolor)
{
    u_16 btnxh=0;
    if(height%2)btnxh=height+1;//基偶数处理
    else btnxh=height;
    if(mode)//填充
    {
         GUI_FillRect(x+r,y,width-2*r,btnxh/2,upcolor);            //中上
        GUI_FillRect(x+r,y+btnxh/2,width-2*r,btnxh/2,downcolor);    //中下
        GUI_FillRect(x,y+r,r,btnxh/2-r,upcolor);                    //左上
        GUI_FillRect(x,y+btnxh/2,r,btnxh/2-r,downcolor);            //左下
        GUI_FillRect(x+width-r,y+r,r,btnxh/2-r,upcolor);            //右上
        GUI_FillRect(x+width-r,y+btnxh/2,r,btnxh/2-r,downcolor);    //右下
    }else
    {
        GUI_HoriLine (x+r,y,width-2*r,upcolor);                    //上           
        GUI_HoriLine (x+r,y+btnxh-1,width-2*r,downcolor);            //下           
        GUI_VertLine (x,y+r,btnxh/2-r,upcolor);                    //左上           
        GUI_VertLine (x,y+btnxh/2,btnxh/2-r,downcolor);            //左下           
        GUI_VertLine (x+width-1,y+r,btnxh/2-r,upcolor);            //右上
        GUI_VertLine (x+width-1,y+btnxh/2,btnxh/2-r,downcolor);    //右下                   
    }
    GUI_DrawArc(x,y,x+r,y+r,x+r,y+r,r,upcolor,mode);//左上    
    GUI_DrawArc(x,y+btnxh-r,x+r,y+btnxh-1,x+r,y+btnxh-r-1,r,downcolor,mode);//左下    
    GUI_DrawArc(x+width-r,y,x+width,y+r,x+width-r-1,y+r,r,upcolor,mode);     //右上
    GUI_DrawArc(x+width-r,y+btnxh-r,x+width,y+btnxh-1,x+width-r-1,y+btnxh-r-1,r,downcolor,mode);//右下
}

//在区域内画点
//(sx,sy),(ex,ey):设定的显示范围
//(x,y):点坐标
//color:颜色
static void _draw_expoint(u_16 sx,u_16 sy,u_16 ex,u_16 ey,u_16 x,u_16 y,GUI_COLOR color)        
{
    if(x<=ex && x>=sx && y<=ey && y>=sy)
    {
        GUI_DrawPixel(x,y,color);        
    }
}

//画8点(Bresenham算法)          
//(sx,sy),(ex,ey):设定的显示范围
//(rx,ry,a,b):参数
//color:颜色
static void _draw_circle8(u_16 sx,u_16 sy,u_16 ex,u_16 ey,u_16 rx,u_16 ry,int a,int b,GUI_COLOR color)
{
    _draw_expoint(sx,sy,ex,ey,rx+a,ry-b,color);              
    _draw_expoint(sx,sy,ex,ey,rx+b,ry-a,color);                       
    _draw_expoint(sx,sy,ex,ey,rx+b,ry+a,color);                           
    _draw_expoint(sx,sy,ex,ey,rx+a,ry+b,color);             
    _draw_expoint(sx,sy,ex,ey,rx-a,ry+b,color);                  
    _draw_expoint(sx,sy,ex,ey,rx-b,ry+a,color);               
    _draw_expoint(sx,sy,ex,ey,rx-b,ry-a,color);                      
    _draw_expoint(sx,sy,ex,ey,rx-a,ry-b,color);                  
}        

//在指定位置画一个指定大小的圆
//(rx,ry):圆心
//(sx,sy),(ex,ey):设定的显示范围
//r    :半径
//color:颜色
//mode :0,不填充;1,填充
void GUI_DrawArc(u_16 sx,u_16 sy,u_16 ex,u_16 ey,u_16 rx,u_16 ry,u_16 r,GUI_COLOR color,u_8 mode)
{
    int a,b,c;
    int di;      
    a=0;b=r;      
    di=3-(r<<1);    //判断下个点位置的标志
    while(a<=b)
    {
        if(mode)for(c=a;c<=b;c++)_draw_circle8(sx,sy,ex,ey,rx,ry,a,c,color);//画实心圆
         else _draw_circle8(sx,sy,ex,ey,rx,ry,a,b,color);                       //画空心圆
        a++;
        //使用Bresenham算法画圆     
        if(di<0)di +=4*a+6;      
        else
        {
            di+=10+4*(a-b);   
            b--;
        }                               
    }
}
