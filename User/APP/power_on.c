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
void DrawLogo(u16 x,u16 y);

void power_on(void)
{
	u8 i;
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
	
	DrawLogo(200,250);

//	Delay(0xffffff);
//	Delay(0xffffff);
//	Delay(0xffffff);
//	Delay(0xffffff);
//	Delay(0xffffff);
//	Delay(0xffffff);
//	LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
//	LCD_DrawFullRect(200,200,150,150);
//	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
	DISP_SYS(0,0,"JKOS");
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	DISP_INS(50,0,"REV 1.0");
	DISP_INS(80,0,"JK508 Multi-channel Temp.Meter");
	DISP_INS(110,0,"REV 1.0");
	
	
	
	page_flag = poweron;
	
	
}

void DrawLogo(u16 x,u16 y)
{
//	page_flag = 0xff;
//	LCD_Clear(LCD_COLOR_BLACK);	
	LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BLACK);
	/*J*/
	LCD_DrawLine(x,y,41,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x-1,y-1,44,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x-1,y-2,46,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x,y-3,46,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+2,y-4,46,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+35,y-5,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+35,y-6,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+35,y-7,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+36,y-8,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+36,y-9,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+37,y-10,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+37,y-11,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+37,y-12,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+38,y-13,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+37,y-14,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+38,y-15,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+38,y-16,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+38,y-17,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+39,y-18,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+39,y-19,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+39,y-20,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+40,y-21,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+40,y-22,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+40,y-23,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+40,y-24,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+41,y-25,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+41,y-26,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+41,y-27,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+42,y-28,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+42,y-29,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+42,y-30,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+42,y-31,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+42,y-32,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+43,y-33,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+43,y-34,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+44,y-35,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+44,y-36,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+44,y-37,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+44,y-38,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+44,y-39,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+45,y-40,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+45,y-41,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+45,y-42,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+45,y-43,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+46,y-44,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+46,y-45,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+46,y-46,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+47,y-47,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+47,y-48,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+47,y-49,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+47,y-50,15,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+48,y-51,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+48,y-52,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+48,y-53,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+49,y-54,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+49,y-55,14,LCD_DIR_HORIZONTAL);
	
	/*i*/
	LCD_DrawFullRect(x+72,y-54,12,3);
	LCD_DrawLine(x+71,y-52,13,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+71,y-51,12,4);
	LCD_DrawFullRect(x+70,y-47,12,3);
	
	LCD_DrawLine(x+68,y-44+4,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+69,y-43+4,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+68,y-42+4,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+68,y-41+4,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+68-1,y-40+4,13,3);
	LCD_DrawLine(x+68-1,y-37+4,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+67-1,y-36+4,13,3);
	LCD_DrawLine(x+67-1,y-33+4,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+66-1,y-32+4,13,2);
	LCD_DrawLine(x+66-1,y-30+4,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+65-1,y-29+4,13,3);
	LCD_DrawLine(x+64-1,y-26+4,14,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+64-1,y-25+4,13,2);
	LCD_DrawFullRect(x+64-1,y-23+4,12,2);
	LCD_DrawLine(x+63-1,y-21+4,13,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+63-1,y-20+4,12,2);
	LCD_DrawFullRect(x+62-1,y-18+4,13,3);
	LCD_DrawLine(x+62-1,y-15+4,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+61-1,y-14+4,13,3);
	LCD_DrawFullRect(x+60-1,y-11+4,13,2);
	LCD_DrawFullRect(x+59-1,y-5,13,2);
	LCD_DrawLine(x+59-1,y-3,14,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+59-1,y-2,13,2);
	LCD_DrawLine(x+59-1,y,12,LCD_DIR_HORIZONTAL);
	
	/*n*/
	LCD_DrawLine(x+88,y-40,23,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+85,y-39,27,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+84,y-38,30,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+83,y-37,32,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+83,y-36,3,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+104,y-36,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+83,y-35,1,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+104,y-35,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+104,y-34,12,3);
	LCD_DrawFullRect(x+103,y-31,13,3);
	LCD_DrawLine(x+102,y-28,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+103,y-27,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+102,y-26,13,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+101,y-25,13,2);
	LCD_DrawFullRect(x+101,y-23,12,2);
	LCD_DrawFullRect(x+100,y-21,13,3);
	LCD_DrawFullRect(x+99,y-18,13,3);
	LCD_DrawLine(x+99,y-15,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+98,y-14,13,3);
	LCD_DrawLine(x+98,y-11,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+97,y-10,13,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+97,y-9,12,4);
	LCD_DrawFullRect(x+96,y-5,12,3);
	LCD_DrawFullRect(x+95,y-2,13,3);
	
	/*k*/
	LCD_DrawLine(x+127,y-56,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+126,y-55,13,3);
	LCD_DrawFullRect(x+126,y-52,12,2);
	LCD_DrawFullRect(x+125,y-50,12,2);
	LCD_DrawLine(x+124,y-48,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+124,y-47,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+124,y-46,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+124,y-45,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+123,y-44,13,2);
	LCD_DrawFullRect(x+123,y-42,12,3);
	LCD_DrawLine(x+153,y-41,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+151,y-40,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+122,y-39,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+150,y-39,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+122,y-38,12,2);
	LCD_DrawLine(x+148,y-38,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+147,y-37,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+121,y-36,13,3);
	LCD_DrawLine(x+146,y-36,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+145,y-35,11,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+143,y-34,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+120,y-33,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+141,y-33,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+121,y-32,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+140,y-32,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+120,y-31,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-31,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+120,y-30,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+137,y-30,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+119,y-29,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+135,y-29,15,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+119,y-28,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+134,y-28,16,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+119,y-27,31,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+118,y-26,31,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+118,y-25,32,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+118,y-24,32,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+117,y-23,33,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+118,y-22,19,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-22,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+117,y-21,19,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-21,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+117,y-20,18,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-20,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+116,y-19,17,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-19,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+116,y-18,16,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-18,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+116,y-17,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-17,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+116,y-16,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+138,y-16,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+115,y-15,13,3);
	LCD_DrawLine(x+115,y-12,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+114,y-11,13,3);
	LCD_DrawLine(x+114,y-8,12,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+113,y-7,13,3);
	LCD_DrawLine(x+112,y-4,14,LCD_DIR_HORIZONTAL);
	LCD_DrawFullRect(x+112,y-3,13,3);
	LCD_DrawLine(x+112,y,12,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+139,y-15,13,3);	
	LCD_DrawFullRect(x+139,y-12,14,2);
	LCD_DrawFullRect(x+140,y-10,13,3);
	LCD_DrawFullRect(x+140,y-7,14,3);
	LCD_DrawFullRect(x+140,y-4,15,2);
	LCD_DrawLine(x+141,y-2,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+143,y-1,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+145,y,10,LCD_DIR_HORIZONTAL);
	
	/*o*/
	LCD_DrawLine(x+173,y-41,35,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+171,y-40,39,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+169,y-39,41,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+168,y-38,43,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+167,y-37,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+198,y-37,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+166,y-36,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+199,y-36,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+166,y-35,12,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+198,y-35,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+165,y-34,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+198,y-34,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+165,y-33,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+198,y-33,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+165,y-32,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+198,y-32,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+164,y-31,14,2);
	LCD_DrawFullRect(x+197,y-31,14,2);
	LCD_DrawLine(x+164,y-29,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+197,y-29,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawFullRect(x+163,y-28,14,2);
	LCD_DrawFullRect(x+196,y-28,14,2);
	LCD_DrawLine(x+163,y-26,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+196,y-26,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+162,y-25,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+196,y-25,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+162,y-25,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+195,y-25,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+162,y-24,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+195,y-24,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+162,y-23,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+195,y-23,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+161,y-22,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+194,y-22,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+161,y-21,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+194,y-21,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+161,y-20,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+194,y-20,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+160,y-19,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+194,y-19,13,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+160,y-18,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+193,y-18,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+160,y-17,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+193,y-17,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+159,y-16,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+193,y-16,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+160,y-15,13,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+193,y-15,13,LCD_DIR_HORIZONTAL);	
	
	LCD_DrawFullRect(x+159,y-14,13,4);
	LCD_DrawLine(x+192,y-14,14,LCD_DIR_HORIZONTAL);	
	LCD_DrawFullRect(x+192,y-13,13,3);
	
	LCD_DrawFullRect(x+159,y-10,12,3);
	LCD_DrawFullRect(x+191,y-10,13,2);
	LCD_DrawLine(x+190,y-8,13,LCD_DIR_HORIZONTAL);	
	
	LCD_DrawLine(x+159,y-7,14,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+188,y-7,14,LCD_DIR_HORIZONTAL);
	
	LCD_DrawLine(x+159,y-6,43,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+160,y-5,41,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+161,y-4,39,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+162,y-3,36,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+163,y-2,34,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(x+164,y-1,32,LCD_DIR_HORIZONTAL);
}