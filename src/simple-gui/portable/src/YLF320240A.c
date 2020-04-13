/*************************************************************************/
/** Copyright.															**/
/** FileName: SDKInterface.cpp											**/
/** Author: Polarix														**/
/** Description: SDK swap interface function defines.					**/
/*************************************************************************/
//=======================================================================//
//= Include files.													    =//
//=======================================================================//
#include "SGUI_Typedef.h"

void SGUI_SDK_SetPixel(int iX, int iY, int iPixelValue);
/*************************************************************************
1   2   3  4    5    6     7       8    9    10   11   12  13  14  15  16  17   18 19 20
GND VDD NC WR   RD   CS    A0(RS)  RST  D0   D1   D2   D3  D4  D5  D6  D7  Vout  NC A  K  
           PD5  PD4	 PG12  PF0     PF1  PD14 PD15 PD0  PD1 PE7 PE8 PE9 PE10
		   NWE	NOE  NE4   A0	   A1   D0   D1	  D2   D3  D4  D5  D6  D7
STM32F103ZET6 FSMC 连接:
 	       NE4 nor/psram  : 0x6C00 0000 - 0x6FFF FFFF
*************************************************************************/
//=======================================================================//
//= 本地驱动程序										                =//
//=======================================================================//
static SGUI_UINT32 lcd_buffer[320*240/32];     //显存
static SGUI_UINT8  lcd_dirty=1;                  //显示脏

#pragma pack(1)
typedef struct{unsigned char data;unsigned char cmd;}YLF_LCD_TYPE;
#define YLF_LCD_BASE  ((unsigned int)0x6C000000)
YLF_LCD_TYPE* YLF_LCD = ((YLF_LCD_TYPE*)YLF_LCD_BASE);
#pragma pack()

#define WData(d)	   YLF_LCD->data=d	   //写数据
#define WCMD(c)	       YLF_LCD->cmd=c	   //写命令


//命令列表
#define systemset  0x40  //初始化设置，后续8字节参数串systemset_P8[]
char const systemset_P8[] =  {0x30, 0x87, 0x07, 0x27, 0x3f, 0xEF, 0x28, 0x00};
                              //P1    P2    P3    P4    P5    P6    P7    P8

#define scroll     0x44  //设定显示起始地址和显示范围，后续10字节参数串scroll_P10[]
#define basepart1  0x00
#define basepart2  0x28
#define basepart3  0x50
#define basepart4  0x00
char const scroll_P10[]   = 
                 { 0x00, basepart1, 0xf0,  //第一显示区RAM，10KB，240行
                   0x00, basepart2, 0xf0,  //第二显示区RAM，10KB，240行
                   0x00, basepart3,        //第三显示区RAM，10KB
                   0x00, basepart4};       //第四显示区RAM，32KB
#define sleepin    0x53  //休闲模式设置     后续参数：无
#define csrdirr    0x4c  //光标向右方向移动 后续参数：无
#define csrdirl    0x4d  //光标向左方向移动 后续参数：无
#define csrdiru    0x4e  //光标向上方向移动 后续参数：无
#define csrdird    0x4f  //光标向下方向移动 后续参数：无
#define dispoff    0x58  //显示关闭         后续参数1字节
#define dispon     0x59  //显示状态打开     后续参数1字节
#define hdotscr    0x5a  //点位移量设置     后续参数1字节
#define ovlay      0x5b  //显示合成设置     后续参数1字节
#define cgramadr   0x5c  //CGRAM首地址设置  后续参数2字节
#define csrform    0x5d  //光标形状设置     后续参数2字节
#define csrw       0x46  //设置光标地址     后续参数2字节
#define csrr       0x47  //读出光标地址     后续参数2字节
#define mwrite     0x42  //显示数据写入     后续参数N字节
#define mread      0x43  //显示数据读取     后续参数N字节
#define busy       0x40  //忙信号D6   0100 0000
#define AP (systemset_P8[6])//一行所占显示存储单元的字节数

//向命令写入数据 
static void Write(unsigned char cmd,unsigned char*data,SGUI_INT16 len)
{
  WCMD(cmd);
  while(len-->0){
  	  WData(*data++);
  }
}

/*************************************************************************
	YLF 初始化
**************************************************************************/
void YLF_cls(char part,char dat)
{	
	unsigned int i=10240;
	WCMD(csrw);//写入光标地址
	WData(0);
	if(part==1)WData(basepart1);//显示区1
	if(part==2)WData(basepart2);//显示区2
	if(part==3)WData(basepart3);//显示区3
	if(part==4)WData(basepart4);//显示区4
	else{i=32768;WData(0);}     //所有显示区

	WCMD(csrdirr);//光标向右方向移动
	WCMD(mwrite); //显示数据写入
	while(i--)	 
	{	
		WData(dat);//清数据	 
	}
}

void YLF_init(void)
{
 	extern void GUI_Configuration(void);
	GUI_Configuration();
	Write(systemset,(char*)systemset_P8,8);
    Write(scroll,(char*)scroll_P10,10);
//	WCMD(hdotscr);WData(0);   //点位移设置:不移动 
	WCMD(dispoff);//WData(0x57);//关
//	WCMD(ovlay);WData(0x08);     //显示合成设置:二重合成，文本显示，逻辑或
////	WCMD(cgramadr);WData(0);WData(0x78);
//	WCMD(csrform);WData(0x07);WData(0x87);//光标形状
	memset(lcd_buffer,0,sizeof(lcd_buffer));
	YLF_cls(0,0);                //清数据
	WCMD(dispon);WData(0x57); //开
}
//=======================================================================//
//= Function define.										            =//
//=======================================================================//
/*************************************************************************/
/** Function Name:	SGUI_SDK_SetPixel                                     **/
/** Purpose:		Set virtual device pixel register data.             **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.			**/
/**	@ iY[in]:		Pixel y-coordinate on display panel.			**/
/**	@ iPixelValue[out]:	Pixel value, 0 for clear, 1 for set.			**/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_SetPixel(int iX, int iY, int iPixelValue)
{
//	char *b = (char*)lcd_buffer;
//	int x=7-iX%8;
//	int index = iX/8 + iY*40;

	char *b = (char*)lcd_buffer;
	int x=iX%8;
	int index = (39-iX/8) + (239-iY)*40;
	int v = (b[index] & (1<<x))?1:0;
	if(v!=iPixelValue){
		lcd_dirty = 1;
		b[index] &=~(1<<x);
		b[index] |=(iPixelValue?1:0)<<x;	
	}
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetPixel									**/
/** Purpose:		Get a pixel value form virtual device register.     **/
/** Params:																**/
/**	@ iX[in]:		Pixel x-coordinate on display panel.			**/
/**	@ iY[in]:		Pixel y-coordinate on display panel.		    **/
/** Return:			Pixel state, 0 for cleared, 1 for set.              **/
/** Notice:			None.                                               **/
/*************************************************************************/
int SGUI_SDK_GetPixel(int iX, int iY)
{
//	char *b = (char*)lcd_buffer;
//	int x=7-iX%8;
//	int index = iX/8 + iY*40;

	char *b = (char*)lcd_buffer;
	int x=iX%8;
	int index = (39-iX/8) + (239-iY)*40;

	return (int)(((b[index]&(1<<x))==0)?0:1);
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_RefreshDisplay								**/
/** Purpose:		Refresh virtual device display.                     **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_RefreshDisplay(void)
{
	unsigned int i=9600;
	unsigned char*b=(unsigned char*)lcd_buffer;
	if(lcd_dirty==0)return;
	WCMD(csrw);WData(0);WData(0);//写入光标地址
	WCMD(csrdirr);//光标向右方向移动
	WCMD(mwrite); //显示数据写入
	while(i--)	 
	{	
		WData(*b++);//清数据	 
	}
	lcd_dirty=0;
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_ClearDisplay								**/
/** Purpose:		Clear screen display.                               **/
/** Params:			None.                                               **/
/** Return:			None.                                               **/
/** Notice:			None.                                               **/
/*************************************************************************/
void SGUI_SDK_ClearDisplay(void)
{
	memset(lcd_buffer,0,sizeof(lcd_buffer));
	lcd_dirty=1;
	//YLF_cls(0,0);
}

/*************************************************************************/
/** Function Name:	SGUI_SDK_SyncKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:																**/
/**	@ uiKeyCode[in]:	Key code.										**/
/** Return:			None.												**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
void SGUI_SDK_SyncKeyEventData(unsigned int uiKeyCode)
{

}

/*************************************************************************/
/** Function Name:	SGUI_SDK_GetKeyEventData.							**/
/** Purpose:		Set key code value when key press event targets.	**/
/** Params:			none.												**/
/** Return:			Last targeted key event value.						**/
/** Notice:			This function used to save and keep pressed key 	**/
/**					code value, must be called before SetEvnetFlag.		**/
/*************************************************************************/
#include "DemoActions.h"
unsigned int SGUI_SDK_GetKeyEventData(void)
{
	if(ld_gpio_get(2)==0)return KEY_VALUE_ENTER;
	if(ld_gpio_get(3)==0)return KEY_VALUE_DOWN;
	return 0;
}




bool CheckEventFlag(int idx)
{
 	switch(idx)
	{
	    //检查是否有按键触发
		case 3:
			if( ld_gpio_get(2)==0 || ld_gpio_get(3)==0 )return true;
			else return false;
		default: return false;

	
	}


}


void SGUI_SDK_ConfigHearBeatTimer(int ms)
{}


void SGUI_SDK_EnableRTCInterrupt(bool bEnable)
{}

