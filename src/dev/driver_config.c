/*========================================================

* file name 		:	drv_config.c
* Author 			:	kaihua.yang
* Version			：	v1.00
* Date				：	11/30/2015
* Description		：	硬件驱动配置文件

*********************************************************
*与硬件有关部分在 Configure or interface给出
*********************************************************
========================================================*/
/******************************************************************
			includes
*******************************************************************/
#include "driver_config_types.h"
#include "types.h"


/******************************************************************
			private
*******************************************************************/


/******************************************************************
			configure variables
*******************************************************************/
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------
/*****************
   外部时钟配置
*****************/
//<o> 时钟--外部晶振


/*****************
   IDWG 常量配置
*****************/
//<o> iwdg
//const unsigned int iwatchdog_xpres =  IWATCHDOG_XPRES;
//const unsigned int iwatchdog_xsetr =  IWATCHDOG_XSETR;

#include "stm32f10x.h"

/***********************
			GPIO  配置常量
***********************/
#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
#define PORT_E 4
#define PORT_F 5
#define PORT_G 6
#define PORT_HC  0xFF  
#define NoIO()       {255,255,0,0,0,0,0},
#define OUT(pp,pin)  {PORT_##pp,pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz,0},
#define OUTH(pp,pin) {PORT_##pp,pin,GPIO_Mode_Out_PP,GPIO_Speed_50MHz,1},   
#define ODH(pp,pin)  {PORT_##pp,pin,GPIO_Mode_Out_OD,GPIO_Speed_50MHz,1},  
#define IN(pp,pin)   {PORT_##pp,pin,GPIO_Mode_IPU,GPIO_Speed_50MHz,0},	
#define AF(pp,pin)   {PORT_##pp,pin,GPIO_Mode_AF_OD,GPIO_Speed_50MHz,0},

//<o> gpio
t_gpio_map gpio_map[]=
{
	//系统LED灯                                                                 
	  OUT(G,14)	
	  
	// KEY 
	   IN(E,0) IN(C,13)
   0,
};
const unsigned char gpio_number = (sizeof(gpio_map)/sizeof(t_gpio_map)) - 1;

/***********************
			EXTI  配置常量
***********************/
#define exti(pp,pin)  {PORT_##pp,pin,pin,EXTI_Trigger_Rising},
//<o> exti
t_exti_map exti_map[]={
	0
};
const unsigned char exti_number = (sizeof(exti_map) /sizeof(t_exti_map)) - 1;
/*************************
			Uart
*************************/
//<o> uart
t_uart_map uart_map[] = {
  
	{AF(A,9) AF(A,10) 1},  //uart1 
	{AF(A,2) AF(A,3) 2},   //uart2 
	{0}	
};
const unsigned char uart_number = sizeof(uart_map) /(sizeof(t_uart_map)) - 1;
