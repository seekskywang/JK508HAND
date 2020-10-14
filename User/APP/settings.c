/**
  ******************************************************************************
  * @file    sysytem.c
  * @author  fire
  * @version V1.0
  * @date    2018-04-29
  * @brief   �ֳֶ�·�¶Ȳ����ǹ������ý���
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
   unsigned char FLAG_VAL6;
	
   struct 
   {
       unsigned char  TC:4;    //�ȵ�ż����
       unsigned char  FT:1;    //��ѡ/�Ƚ�
       unsigned char  BP:1;    //Ѷ��
       unsigned char  UT:2;	   //��λ
   }BIT_FLAG;
}FLAG6;

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


void page_set(void)
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
	Drawsetmenu();
	DrawUnit();
	
	LCD_SetTextColor(LCD_COLOR_HLT);  		
	LCD_SetBackColor(LCD_COLOR_BACK);
	if(LANG == chs)
	{
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "��������");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,10,"�Ƚ�");
		LCD_DisplayStringLine(90,10,"Ѷ��");
		LCD_DisplayStringLine(130,10,"��λ");
		LCD_DisplayStringLine(170,10,"��ʱ�ɼ�");
		LCD_DisplayStringLine(210,10,"��ʼʱ��");
		LCD_DisplayStringLine(250,10,"����ʱ��");
		LCD_DisplayStringLine(130,360,"�ظ��ɼ�");
//		LCD_DisplayStringLine(170,10,"����");
		LCD_DisplayStringLine(285,10,"U������");
		LCD_DisplayStringLine(320,10,"��ʱ");
		LCD_DisplayStringLine(50,360,"�ٶ�");
		LCD_DisplayStringLine(90,360,"����");
//		LCD_DisplayStringLine(320,360,"��ʱ");
	}else{
		LCD_DisplayStringLine(3,10, "<SETUP>");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,10,"COMP");
		LCD_DisplayStringLine(90,10,"BEEP");
		LCD_DisplayStringLine(127,10,"UNIT");
		LCD_DisplayStringLine(170,10,"TRIGTIME");
		LCD_DisplayStringLine(210,10,"START");
		LCD_DisplayStringLine(250,10,"END");
		LCD_DisplayStringLine(130,360,"REPEAT");
		LCD_DisplayStringLine(285,10,"USBHDD SET");
		LCD_DisplayStringLine(317,10,"TIME");
		LCD_DisplayStringLine(50,360,"SPEED");
		LCD_DisplayStringLine(90,360,"BAUD");
//		LCD_DisplayStringLine(320,360,"TIME");
	}
	
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetBackColor(LCD_COLOR_YELLOW);
	
	if(FILTER == ft_off){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(50,150,"�ر�");
		}else{
			LCD_DisplayStringLine(50,150,"OFF");
		}
	}else{
		if(LANG == chs)
		{
			LCD_DisplayStringLine(50,150,"��");
		}else{
			LCD_DisplayStringLine(50,150,"ON");
		}
	}
	
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BACK);
	if(BEEP == beep_off){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(90,150,"�ر�");
		}else{
			LCD_DisplayStringLine(90,150,"OFF");
		}
	}else{
		if(LANG == chs)
		{
			LCD_DisplayStringLine(90,150,"��");
		}else{
			LCD_DisplayStringLine(90,150,"ON");
		}
	}
	
	if(TIMETIRG == trig_off){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(170,150,"�ر�");
		}else{
			LCD_DisplayStringLine(170,150,"OFF");
		}
	}else{
		if(LANG == chs)
		{
			LCD_DisplayStringLine(170,150,"��");
		}else{
			LCD_DisplayStringLine(170,150,"ON");
		}
	}
	sprintf(buf,"%0.2d:%0.2d",
			STARTH,
			STARTM);
	LCD_DisplayStringLine(208,150,(uint8_t *)buf);
	sprintf(buf,"%0.2d:%0.2d",
			ENDH,
			ENDM);
	LCD_DisplayStringLine(248,150,(uint8_t *)buf);
	if(REPEAT == rtp_off){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(130,500,"�ر�");
		}else{
			LCD_DisplayStringLine(130,500,"OFF");
		}
	}else{
		if(LANG == chs)
		{
			LCD_DisplayStringLine(130,500,"��");
		}else{
			LCD_DisplayStringLine(130,500,"ON");
		}
	}
	
	if(UNIT == C){
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		DISP_CNL_S(127,150,"o");
		LCD_DisplayStringLine(127,155,"C");
	}else if(UNIT == F){
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		DISP_CNL_S(127,150,"o");
		LCD_DisplayStringLine(127,155,"F");
	}else{
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_DisplayStringLine(127,155,"K");
	}
	
	
//	if(FONT == big){
//		if(LANG == chs)
//		{
//			LCD_DisplayStringLine(170,150,"��");
//		}else{
//			LCD_DisplayStringLine(170,150,"L");
//		}
//	}else if(FONT == middle){
//		if(LANG == chs)
//		{
//			LCD_DisplayStringLine(170,150,"��");
//		}else{
//			LCD_DisplayStringLine(170,150,"M");
//		}
//	}else if(FONT == small){
//		if(LANG == chs)
//		{
//			LCD_DisplayStringLine(170,150,"С");
//		}else{
//			LCD_DisplayStringLine(170,150,"S");
//		}
//	}
	
//	LCD_DisplayStringLine(317,150,"AUTO");
	
	if(SPEED == fast)
	{
		if(LANG == chs)
		{
			LCD_DisplayStringLine(50,500,"����");
		}else{
			LCD_DisplayStringLine(50,500,"FAST");
		}
	}else if(SPEED == moderate){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(50,500,"����");
		}else{
			LCD_DisplayStringLine(50,500,"MED");
		}
	}else if(SPEED == slow){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(50,500,"����");
		}else{
			LCD_DisplayStringLine(50,500,"SLOW");
		}
	}
	
	if(BAUD == b9600)
	{
		LCD_DisplayStringLine(87,500,"9600");
	}else if(BAUD == b19200){
		LCD_DisplayStringLine(87,500,"19200");
	}else if(BAUD == b38400){
		LCD_DisplayStringLine(87,500,"38400");
	}else if(BAUD == b57600){
		LCD_DisplayStringLine(87,500,"57600");
	}else if(BAUD == b115200){
		LCD_DisplayStringLine(87,500,"115200");
	}
	
//	LCD_DisplayStringLine(317,150,"1");
	stimefocus_off(usavetime[0]);
	LCD_DisplayStringLine(319,182,"S");
	
	if(LANG == chs)
	{
		DrawInstruction("�Ƚ�������");
	}else{
		DrawInstruction("Comparator ON/OFF");
	}		
	
	page_flag = settings;
	op_flag = set_filter;
	bit_flag = 1;
}
