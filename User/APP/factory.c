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

u8 passverify;
char SN[8];
char SearchBuffer[8];
const uint8_t calch[][3] =
{
	{"001"},
	{"002"},
	{"003"},
	{"004"},
	{"005"},
	{"006"},
	{"007"},
	{"008"},
	{"009"},
	{"010"},
	{"011"},
	{"012"},
	{"013"},
	{"014"},
	{"015"},
	{"016"},
};


u8 caltype;

void page_factory(void)
{
	char buf[10];
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	if(passverify == 0)
	{
		input_pass("0");
		del_num();
	}else{
		DrawMenu();
		Drawsysinfomenu();

		LCD_SetTextColor(LCD_COLOR_HLT);  		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "��������");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);

		LCD_DisplayStringLine(54,10,"ͨ����");
		LCD_DisplayStringLine(94,10,"�������к�");
//		LCD_DisplayStringLine(134,10,"У׼ͨ��");
//		LCD_DisplayStringLine(174,10,"����������");
		LCD_DisplayStringLine(134,10,"LOGO");
		
		Disp_Factory();
	
	}
	page_flag = factory;
}

void Disp_Factory(void)
{
	char buf[10];
	
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetBackColor(LCD_COLOR_BACK);
	sprintf(buf,"%03d",CHNUM);
	LCD_DisplayStringLine(50,170,(uint8_t *)buf);
	LCD_DisplayStringLine(90,170,(uint8_t *)SN);
	sprintf(buf,"%d",JKFLAG);
	LCD_DisplayStringLine(130,170,(uint8_t *)buf);
}