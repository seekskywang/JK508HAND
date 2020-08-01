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
float hisyset[2];


void page_gset(void)
{
	u8 i,j;
	char buf[5];
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	DrawMenu();
	Drawgsethmenu();
	
	if(LANG == chs)
	{
		LCD_SetTextColor(LCD_COLOR_HLT);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "��������");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,10,"Y-��");
		LCD_DisplayStringLine(54,69,"��");
		LCD_DisplayStringLine(90,10,"Y-");
		LCD_DisplayStringLine(94,42,"����");
		LCD_DisplayStringLine(134,10,"������");
//		LCD_DisplayStringLine(174,10,"ͨ��״̬");
	}else{
		LCD_SetTextColor(LCD_COLOR_HLT);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(3,10, "<GRAPH SETUP>");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,13,"Y-HIGH");
		LCD_DisplayStringLine(90,10,"Y-LOW");
		LCD_DisplayStringLine(130,10,"MULTI");
//		LCD_DisplayStringLine(170,10,"CHANNEL STATUS");
	}
	
	yhfocus_on(YHLIMIT);
	ylfocus_off(YLLIMIT);
	multifocus_off(MULTI);
	
	if(LANG == chs)
	{
		DrawInstruction("Y������");
	}else{
		DrawInstruction("Y-High Value");
	}
	
//	LCD_SetBackColor(LCD_COLOR_BACK);
//	LCD_SetTextColor(LCD_COLOR_YELLOW);
//	for(j = 0;j < 4;j++)
//	{
//		for(i = 1 + j*10;i < 11 + j*10 ;i++)
//		{
//			sprintf(buf,"%03d",i);
//			DISP_CNL_M(210 + j*50,10+(i-j*10-1)*60,(uint8_t*)buf);
//		}
//	}
	
	
	page_flag = graphset;
	op_flag = set_yhigh;
	bit_flag = 1;
}

void his_gset(void)
{
	u8 i,j;
	char buf[5];
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	DrawMenu();
	Drawhisgsethmenu();
	
	if(LANG == chs)
	{
		LCD_SetTextColor(LCD_COLOR_HLT);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "��ʷ����");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,10,"Y-��");
		LCD_DisplayStringLine(54,69,"��");
		LCD_DisplayStringLine(90,10,"Y-");
		LCD_DisplayStringLine(94,42,"����");
//		LCD_DisplayStringLine(134,10,"������");
//		LCD_DisplayStringLine(174,10,"ͨ��״̬");
	}else{
		LCD_SetTextColor(LCD_COLOR_HLT);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(3,10, "<GRAPH SETUP>");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,13,"Y-HIGH");
		LCD_DisplayStringLine(90,10,"Y-LOW");
//		LCD_DisplayStringLine(130,10,"MULTI");
//		LCD_DisplayStringLine(170,10,"CHANNEL STATUS");
	}
	
	yhfocus_on(YHLIMIT);
	ylfocus_off(YLLIMIT);
//	multifocus_off(MULTI);
	
	if(LANG == chs)
	{
		DrawInstruction("Y������");
	}else{
		DrawInstruction("Y-High Value");
	}
	
//	LCD_SetBackColor(LCD_COLOR_BACK);
//	LCD_SetTextColor(LCD_COLOR_YELLOW);
//	for(j = 0;j < 4;j++)
//	{
//		for(i = 1 + j*10;i < 11 + j*10 ;i++)
//		{
//			sprintf(buf,"%03d",i);
//			DISP_CNL_M(210 + j*50,10+(i-j*10-1)*60,(uint8_t*)buf);
//		}
//	}
	
	
	page_flag = hisgset;
	op_flag = set_yhigh;
	bit_flag = 1;
}