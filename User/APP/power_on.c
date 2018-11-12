/**
  ******************************************************************************
  * @file    sysytem.c
  * @author  k
  * @version V1.0
  * @date    2018-04-29
  * @brief   手持多路温度测试仪开机界面
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"
#include "./lcd/bsp_lcd.h"
#include "./key/bsp_key.h"
#include "./beep/bsp_beep.h"
#include "./tim/bsp_basic_tim.h"
#include "jk508.h"


void power_on(void)
{
//	u8 i;
//	char buf[10];
	
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BLACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/
//	for(i=0;i<16;i++)
//	{
//		LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
//		DISP_INS(5+i*20,5,"Initializing Channel");
//		sprintf(buf,"%03d",i+1);
//		DISP_INS(5+i*20,336,(uint8_t*)buf);
//		Delay(500);
//	}
	
	LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
	DISP_SYS(0,0,"JKOS");
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	DISP_INS(50,0,"REV 1.0");
	DISP_INS(200,0,"JK508 Multi-channel Temp.Meter");
	DISP_INS(230,0,"REV 1.0");
	
	
	
	page_flag = poweron;
	
	
}

void DrawLogo(u16 x,u16 y)
{
	LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
	/*J*/
}