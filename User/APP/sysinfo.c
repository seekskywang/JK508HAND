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


void page_sysinfo(void)
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
	Drawsysinfomenu();
	if(LANG == chs)
	{
		LCD_SetTextColor(LCD_COLOR_HLT);  		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "ϵͳ��Ϣ");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,10,"�ͺ�");
		
		LCD_DisplayStringLine(134,10,"�������ͺ�");
		LCD_DisplayStringLine(174,10,"ͨ����");
		LCD_DisplayStringLine(214,10,"�����汾");
		LCD_DisplayStringLine(254,10,"����ϵͳ");
		LCD_DisplayStringLine(294,10,"USB�ӿ�");
		LCD_DisplayStringLine(334,10,"�������к�");
	}else{
		LCD_SetTextColor(LCD_COLOR_HLT);  		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(3,10, "<SYSTEM INFOMATION>");
		
		LCD_SetTextColor(LCD_COLOR_LIGHTBLUE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(50,13,"MODEL");
		LCD_DisplayStringLine(130,10,"SENSOR MODEL");
		LCD_DisplayStringLine(170,10,"CHAN AMOUNT");
		LCD_DisplayStringLine(210,10,"FW VERSION");
		LCD_DisplayStringLine(250,10,"OS");
		LCD_DisplayStringLine(290,10,"USB I/F");
		LCD_DisplayStringLine(330,10,"SERIAL NO.");
	}
	
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetBackColor(LCD_COLOR_BACK);
	if(LANG == chs)
	{
		if(JKFLAG == 1)
		{
			if(CHNUM == 8)
			{
				LCD_DisplayStringLine(50,170,"JK508");
			}else if(CHNUM == 16){
				LCD_DisplayStringLine(50,170,"JK516");
			}
			LCD_DisplayStringLine(94,170,"�ֳֶ�·�¶Ȳ�����");
		}else{
			LCD_DisplayStringLine(50,170,"�ֳֶ�·�¶Ȳ�����");
		}
		LCD_DisplayStringLine(130,170,"T,K,J,N,E,S,R,B,PT100");
		sprintf(buf,"%03d",CHNUM);
		LCD_DisplayStringLine(170,170,(uint8_t *)buf);
		LCD_DisplayStringLine(210,170,"REV 2.3");
		LCD_DisplayStringLine(250,170,"");
		LCD_DisplayStringLine(290,170,"REV A0");
		LCD_DisplayStringLine(330,170,(uint8_t *)SN);
		//1.3����SD����ʷ���ݺͿ�ʼֹͣ�ɼ�����
		//1.5������ʾ�߿�
		//1.6����U�̶�ʱʱ������
		//1.7֧��sd�����ݵ�����U��
		//1.8	
		//2.1�޸����߸��¶���ʾbug	
		//2.2�޸���λ����·����bug��U�̿�¼����bug
		//2.3�޸���λ����·����Ϊ7FFF
	}else{
		if(JKFLAG == 1)
		{
				if(CHNUM == 8)
				{		
					LCD_DisplayStringLine(50,220,"JK508 Handheld");
				}else if(CHNUM == 16){
					LCD_DisplayStringLine(50,170,"JK516 Handheld");
				}
				LCD_DisplayStringLine(90,220,"Muti-Channel Temp Meter");
		}else{
			LCD_DisplayStringLine(50,220,"Muti-Channel Temp Meter");
		}
		LCD_DisplayStringLine(130,220,"T,K,J,N,E,S,R,B,PT100");
		sprintf(buf,"%03d",CHNUM);
		LCD_DisplayStringLine(170,220,(uint8_t *)buf);
		LCD_DisplayStringLine(210,220,"REV 2.3");
		LCD_DisplayStringLine(250,220,"");
		LCD_DisplayStringLine(290,220,"REV A0");
		LCD_DisplayStringLine(330,220,(uint8_t *)SN);
	}
	
	if(LANG == chs)
	{ 
		DrawInstruction("ϵͳ��Ϣҳ");
	}else{
		DrawInstruction("System information page");
	}
	page_flag = sysinfo;
}
