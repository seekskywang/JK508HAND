/**
  ******************************************************************************
  * @file    sysytem.c
  * @author  fire
  * @version V1.0
  * @date    2018-04-29
  * @brief   �ֳֶ�·�¶Ȳ�����ϵͳ���ý���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./led/bsp_led.h"
#include "./lcd/bsp_lcd.h"
#include "./key/bsp_key.h"
#include "./beep/bsp_beep.h"
#include "./tim/bsp_basic_tim.h"
#include "jk508.h"

extern union 
{
   unsigned char FLAG_VAL7;
	
   struct 
   {
       unsigned char  FN:2;    //����
       unsigned char  SP:2;    //�ٶ�
       unsigned char  BD:3;    //����
       unsigned char  LG:1;	   //����
   }BIT_FLAG;
}FLAG7;

extern union 
{
   unsigned char FLAG_VAL8;
	
   struct 
   {
       unsigned char  BR:3;    //����
       unsigned char  DM:3;    //��������
       
   }BIT_FLAG;
}FLAG8;

void page_sys(void)
{
	char buf[10];
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	
	
	DrawMenu();
	Drawsysmenu();
	if(LANG == chs)
	{
		LCD_SetTextColor(LCD_COLOR_HLT);  		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "ϵͳ����");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,13,"LANGUAGE");
		LCD_DisplayStringLine(94,10,"����/ʱ��");
//		LCD_DisplayStringLine(134,10,"�˺�");
		LCD_DisplayStringLine(134,10,"����");
		LCD_DisplayStringLine(174,10,"��������");
		LCD_DisplayStringLine(214,10,"������");
	}else{
		LCD_SetTextColor(LCD_COLOR_HLT);  		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(3,10, "<SYSTEM CONFIG>");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,13,"LANGUAGE");
		LCD_DisplayStringLine(90,10,"DATE/TIME");
//		LCD_DisplayStringLine(130,10,"ACCOUNT");
		LCD_DisplayStringLine(130,10,"BRIGHTNESS");
		LCD_DisplayStringLine(170,10,"DIM BRIGHT");
		LCD_DisplayStringLine(210,10,"TOUCH PANEL");
	}
	
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetBackColor(LCD_COLOR_YELLOW);
	if(LANG == chs)
	{
		LCD_DisplayStringLine(50,170+30,"CHINESE");
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_SetBackColor(LCD_COLOR_BACK);
		if(BRTS == L0)
		{
			sprintf(buf,"%d",10);
		}else if(BRTS == L1){
			sprintf(buf,"%d",25);
		}else if(BRTS == L2){
			sprintf(buf,"%d",50);
		}else if(BRTS == L3){
			sprintf(buf,"%d",75);
		}else if(BRTS == L4){
			sprintf(buf,"%d",100);
		}		
		LCD_DisplayStringLine(130,170+30,(uint8_t*)buf);
		if(DIM == DOFF)
		{
			LCD_DisplayStringLine(174,170+30,"�ر�");
		}else if(DIM == D5){
			sprintf(buf,"%d",5);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D10){
			sprintf(buf,"%d",10);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D15){
			sprintf(buf,"%d",15);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D30){
			sprintf(buf,"%d",30);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}
		if(TOUCH == op_on)
		{
			LCD_DisplayStringLine(214,170+30,"��");
		}else{
			LCD_DisplayStringLine(214,170+30,"�ر�");
		}
	}else{
		LCD_DisplayStringLine(50,170+30,"ENGLISH");
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_SetBackColor(LCD_COLOR_BACK);
		if(BRTS == L0)
		{
			brightness = 5;
			sprintf(buf,"%d",10);
		}else if(BRTS == L1){
			brightness = 15;
			sprintf(buf,"%d",25);
		}else if(BRTS == L2){
			brightness = 30;
			sprintf(buf,"%d",50);
		}else if(BRTS == L3){
			brightness = 45;
			sprintf(buf,"%d",75);
		}else if(BRTS == L4){
			brightness = 60;
			sprintf(buf,"%d",100);
		}		
		LCD_DisplayStringLine(130,170+30,(uint8_t*)buf);
		if(DIM == DOFF)
		{
			LCD_DisplayStringLine(172,170+30,"OFF");
		}else if(DIM == D5){
			sprintf(buf,"%d",5);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D10){
			sprintf(buf,"%d",10);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D15){
			sprintf(buf,"%d",15);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}else if(DIM == D30){
			sprintf(buf,"%d",30);
			LCD_DisplayStringLine(172,170+30,(uint8_t*)buf);
		}
		
		if(TOUCH == op_on)
		{
			LCD_DisplayStringLine(212,170+30,"ON");
		}else{
			LCD_DisplayStringLine(212,170+30,"OFF");
		}
	}
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BACK);
	
	LCD_DisplayStringLine(130,230+30,"%");
	if(DIM == DOFF)
	{
		LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
		LCD_DrawFullRect(230+30,172,55,32);
	}else{
		LCD_DisplayStringLine(172,230+30,"Min");
	}
	
	if(LANG == chs)
	{
		DrawInstruction("ϵͳ����ѡ��");
	}else{
		DrawInstruction("Select system language");
	}
	page_flag = sysset;
	op_flag = set_lang;
}
