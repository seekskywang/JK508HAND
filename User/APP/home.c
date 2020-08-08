/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-04-29
  * @brief   �ֳֶ�·�¶Ȳ����ǽ���
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
#include "./flash/bsp_spi_flash.h"
#include "jk508.h"
#include "sdio/bsp_sdio_sd.h"


extern union 
{
   unsigned char FLAG_VAL;
		
   struct 
   {
       unsigned char  SW1:1;
       unsigned char  SW2:1;
       unsigned char  SW3:1;
       unsigned char  SW4:1;
       unsigned char  SW5:1;
       unsigned char  SW6:1;
       unsigned char  SW7:1;
       unsigned char  SW8:1; 
   }BIT_FLAG;
}FLAG1;

extern union 
{
   unsigned char FLAG_VAL;
		
   struct 
   {
       unsigned char  SW9:1;
       unsigned char  SW10:1;
       unsigned char  SW11:1;
       unsigned char  SW12:1;
       unsigned char  SW13:1;
       unsigned char  SW14:1;
       unsigned char  SW15:1;
       unsigned char  SW16:1;  
   }BIT_FLAG;
}FLAG2;

extern union 
{
   unsigned char FLAG_VAL;
		
   struct 
   {
       unsigned char  SW17:1;
       unsigned char  SW18:1;
       unsigned char  SW19:1;
       unsigned char  SW20:1;
       unsigned char  SW21:1;
       unsigned char  SW22:1;
       unsigned char  SW23:1;
       unsigned char  SW24:1;  
   }BIT_FLAG;
}FLAG3;

extern union 
{
   unsigned char FLAG_VAL;
		
   struct 
   {
       unsigned char  SW25:1;
       unsigned char  SW26:1;
       unsigned char  SW27:1;
       unsigned char  SW28:1;
       unsigned char  SW29:1;
       unsigned char  SW30:1;
       unsigned char  SW31:1;
       unsigned char  SW32:1;
   }BIT_FLAG;
}FLAG4;

extern union 
{
   unsigned char FLAG_VAL;
		
   struct 
   {
       unsigned char  SW33:1;
       unsigned char  SW34:1;
       unsigned char  SW35:1;
       unsigned char  SW36:1;
       unsigned char  SW37:1;
       unsigned char  SW38:1;
       unsigned char  SW39:1;	
       unsigned char  SW40:1;
   }BIT_FLAG;
}FLAG5;
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

const uint8_t TC_Type[][4]=
{
	{"T"},
	{"K"},
	{"J"},
	{"N"},
	{"E"},
	{"S"},
	{"R"},
	{"B"},
	{"PT"},
	{""},
	{""},
	{" "},
	{"1V"},
	{"1V"},
	{"1V"},
	{"18"},
	{"AM"},
};

void Draw_Frame(void)
{
	u8 i;
	LCD_SetColors(LCD_COLOR_GR3,LCD_COLOR_BACK);
	if(FONT == big)
	{
		for(i=0;i<5;i++)
		{
			LCD_DrawLine(0,90+80*i,630,LCD_DIR_HORIZONTAL);
		}
		LCD_DrawLine(0,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(90,90,320,LCD_DIR_VERTICAL);
		LCD_DrawLine(320,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(410,90,320,LCD_DIR_VERTICAL);
		LCD_DrawLine(630,90,320,LCD_DIR_VERTICAL);
	}else if(FONT == middle){
		for(i=0;i<10;i++)
		{
			LCD_DrawLine(0,90+40*i,630,LCD_DIR_HORIZONTAL);
		}
		LCD_DrawLine(0,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(70,90,320,LCD_DIR_VERTICAL);
		LCD_DrawLine(320,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(270,90,320,LCD_DIR_VERTICAL);
		LCD_DrawLine(630,90,320,LCD_DIR_VERTICAL);
	}else if(FONT == small){
//		for(i=0;i<10;i++)
//		{
//			LCD_DrawLine(0,90+40*i,630,LCD_DIR_HORIZONTAL);
//		}
//		LCD_DrawLine(0,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(126,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(320,90,320,LCD_DIR_VERTICAL);
////		LCD_DrawLine(270,90,320,LCD_DIR_VERTICAL);
//		LCD_DrawLine(630,90,320,LCD_DIR_VERTICAL);
	}
}
void page_home(void)
{
//	CHNUM = 16;
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	ch_page = page1;
	tcpage = 1;
	DrawMenu();
	DrawUnit();
	if(sdstatus == 1)//��ʼ��SD��
	{
		DrawSD2();
	}else{
		DrawSD1();
	}
	if(LANG == chs)
	{
		LCD_SetTextColor(LCD_COLOR_HLT);  	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_DisplayStringLine(0,10, "<      >");
		LCD_DisplayStringLine(5,26, "������ʾ");
		LCD_DisplayStringLine(50,10,"�ͺ�");
		LCD_DisplayStringLine(47,200,"����");
		LCD_DisplayStringLine(47,390,"ҳ��");
			
		DrawInstruction("�ȵ�ż�ͺ�ѡ��");
	}else{
		LCD_SetTextColor(LCD_COLOR_HLT);  	
		LCD_SetBackColor(LCD_COLOR_BACK);
//		LCD_DisplayStringLine(0,10, "");
		LCD_DisplayStringLine(3,10,"<DISP>");
		LCD_DisplayStringLine(47,10,"MODEL");
		LCD_DisplayStringLine(47,200,"FONT");
		LCD_DisplayStringLine(47,390,"PAGE");
		
		DrawInstruction("Thermocouple model");
	}
	Draw_Frame();
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetBackColor(LCD_COLOR_YELLOW);
	
	
	
	
	if(TCTYPE == TCK)
	{
		LCD_DisplayStringLine(47,100,"TC-K");
	}else if(TCTYPE == TCT){
		LCD_DisplayStringLine(47,100,"TC-T");
	}else if(TCTYPE == TCJ){
		LCD_DisplayStringLine(47,100,"TC-J");
	}else if(TCTYPE == TCN){
		LCD_DisplayStringLine(47,100,"TC-N");
	}else if(TCTYPE == TCE){
		LCD_DisplayStringLine(47,100,"TC-E");
	}else if(TCTYPE == TCS){
		LCD_DisplayStringLine(47,100,"TC-S");
	}else if(TCTYPE == TCR){
		LCD_DisplayStringLine(47,100,"TC-R");
	}else if(TCTYPE == TCB){
		LCD_DisplayStringLine(47,100,"TC-B");
	}else if(TCTYPE == PT100){
		LCD_DisplayStringLine(47,100,"PT100");
	}else if(TCTYPE == V1){
		LCD_DisplayStringLine(47,100,"1~5V");
	}
	

	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BACK);
	if(FONT == big){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(47,290,"��");
		}else{
			LCD_DisplayStringLine(47,290,"L");
		}
	}else if(FONT == middle){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(47,290,"��");
		}else{
			LCD_DisplayStringLine(47,290,"M");
		}
	}else if(FONT == small){
		if(LANG == chs)
		{
			LCD_DisplayStringLine(47,290,"С");
		}else{
			LCD_DisplayStringLine(47,290,"S");
		}
	}
	
	LCD_DisplayStringLine_48(47,480,"01/01");
	
	Drawhomemenu();
	Disp_Type();
	if(FONT == big)
	{
		
		if(CH1_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_HLT);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,10,"001");
		}else if(CH1_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,10,"001");
		}
		
		if(CH2_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170+10,10,"002");
		}else if(CH2_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170+10,10,"002");
		}
		
		if(CH3_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240+20,10,"003");
		}else if(CH3_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240+20,10,"003");
		}
		
		if(CH4_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310+30,10,"004");
		}else if(CH4_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310+30,10,"004");
		}
		
		if(CH5_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,330,"005");
		}else if(CH5_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,330,"005");
		}
		
		if(CH6_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170+10,330,"006");
		}else if(CH6_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170+10,330,"006");
		}
		
		if(CH7_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240+20,330,"007");
		}else if(CH7_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240+20,330,"007");
		}
		
		if(CH8_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310+30,330,"008");
		}else if(CH8_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310+30,330,"008");
		}

	}else if(FONT == middle){
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_SetTextColor(LCD_COLOR_HLT);
		
		if(CH1_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(100,10,"001");
		}else if(CH1_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(100,10,"001");
		}
		
		if(CH2_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(140,10,"002");
		}else if(CH2_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(140,10,"002");
		}
		
		if(CH3_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(180,10,"003");
		}else if(CH3_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(180,10,"003");
		}
		
		if(CH4_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(220,10,"004");
		}else if(CH4_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(220,10,"004");
		}
		
		if(CH5_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(260,10,"005");
		}else if(CH5_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(260,10,"005");
		}
		
		if(CH6_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(300,10,"006");
		}else if(CH6_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(300,10,"006");
		}
		
		if(CH7_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(340,10,"007");
		}else if(CH7_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(340,10,"007");
		}
		
		if(CH8_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(380,10,"008");
		}else if(CH8_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(380,10,"008");
		}
		if(CHNUM > 8)
		{
			if(CH9_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(100,330,"009");
			}else if(CH9_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(100,330,"009");
			}
			
			if(CH10_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(140,330,"010");
			}else if(CH10_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(140,330,"010");
			}
			
			if(CH11_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(180,330,"011");
			}else if(CH11_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(180,330,"011");
			}
			
			if(CH12_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(220,330,"012");
			}else if(CH12_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(220,330,"012");
			}
			
			if(CH13_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(260,330,"013");
			}else if(CH13_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(260,330,"013");
			}
			
			if(CH14_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(300,330,"014");
			}else if(CH14_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(300,330,"014");
			}
			
			if(CH15_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(340,330,"015");
			}else if(CH15_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(340,330,"015");
			}
			
			if(CH16_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_M(380,330,"016");
			}else if(CH16_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_M(380,330,"016");
			}
			if(CHNUM > 16)
			{
				if(CH17_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(100,410,"017");
				}else if(CH17_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(100,410,"017");
				}
				
				if(CH18_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(140,410,"018");
				}else if(CH18_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(140,410,"018");
				}
				
				if(CH19_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(180,410,"019");
				}else if(CH19_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(180,410,"019");
				}
				
				if(CH20_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(220,410,"020");
				}else if(CH20_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(220,410,"020");
				}
				
				if(CH21_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(260,410,"021");
				}else if(CH21_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(260,410,"021");
				}
				
				if(CH22_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(300,410,"022");
				}else if(CH22_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(300,410,"022");
				}
				
				if(CH23_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(340,410,"023");
				}else if(CH23_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(340,410,"023");
				}
				
				if(CH24_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_M(380,410,"024");
				}else if(CH24_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_M(380,410,"024");
				}
			}
		}
		
		
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_SetTextColor(LCD_COLOR_WHITE);
	}else if(FONT == small){
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_SetTextColor(LCD_COLOR_HLT);
		
		if(CH1_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(100,10,"001");
		}else if(CH1_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(100,10,"001");
		}
		
		if(CH2_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(140,10,"002");
		}else if(CH2_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(140,10,"002");
		}
		
		if(CH3_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(180,10,"003");
		}else if(CH3_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(180,10,"003");
		}
		
		if(CH4_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(220,10,"004");
		}else if(CH4_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(220,10,"004");
		}
		
		if(CH5_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(260,10,"005");
		}else if(CH5_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(260,10,"005");
		}
		
		if(CH6_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(300,10,"006");
		}else if(CH6_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(300,10,"006");
		}
		
		if(CH7_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(340,10,"007");
		}else if(CH7_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(340,10,"007");
		}
		
		if(CH8_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_S(380,10,"008");
		}else if(CH8_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_S(380,10,"008");
		}
		if(CHNUM > 8)
		{
			if(CH9_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(100,134,"009");
			}else if(CH9_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(100,134,"009");
			}
			
			if(CH10_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(140,134,"010");
			}else if(CH10_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(140,134,"010");
			}
			
			if(CH11_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(180,134,"011");
			}else if(CH11_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(180,134,"011");
			}
			
			if(CH12_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(220,134,"012");
			}else if(CH12_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(220,134,"012");
			}
			
			if(CH13_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(260,134,"013");
			}else if(CH13_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(260,134,"013");
			}
			
			if(CH14_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(300,134,"014");
			}else if(CH14_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(300,134,"014");
			}
			
			if(CH15_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(340,134,"015");
			}else if(CH15_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(340,134,"015");
			}
			
			if(CH16_SW == ch_off)
			{
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
				DISP_CNL_S(380,134,"016");
			}else if(CH16_SW == ch_on){
				LCD_SetBackColor(LCD_COLOR_BACK);
				LCD_SetTextColor(LCD_COLOR_HLT);
				DISP_CNL_S(380,134,"016");
			}
			if(CHNUM > 16)
			{
				if(CH17_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(100,258,"017");
				}else if(CH17_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(100,258,"017");
				}
				
				if(CH18_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(140,258,"018");
				}else if(CH18_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(140,258,"018");
				}
				
				if(CH19_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(180,258,"019");
				}else if(CH19_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(180,258,"019");
				}
				
				if(CH20_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(220,258,"020");
				}else if(CH20_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(220,258,"020");
				}
				
				if(CH21_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(260,258,"021");
				}else if(CH21_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(260,258,"021");
				}
				
				if(CH22_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(300,258,"022");
				}else if(CH22_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(300,258,"022");
				}
				
				if(CH23_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(340,258,"023");
				}else if(CH23_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(340,258,"023");
				}
				
				if(CH24_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(380,258,"024");
				}else if(CH24_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(380,258,"024");
				}
				
				if(CH25_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(100,382,"025");
				}else if(CH25_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(100,382,"025");
				}
				
				if(CH26_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(140,382,"026");
				}else if(CH26_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(140,382,"026");
				}
				
				if(CH27_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(180,382,"027");
				}else if(CH27_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(180,382,"027");
				}
				
				if(CH28_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(220,382,"028");
				}else if(CH28_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(220,382,"028");
				}
				
				if(CH29_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(260,382,"029");
				}else if(CH29_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(260,382,"029");
				}
				
				if(CH30_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(300,382,"030");
				}else if(CH30_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(300,382,"030");
				}
				
				if(CH31_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(340,382,"031");
				}else if(CH31_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(340,382,"031");
				}
				
				if(CH32_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(380,382,"032");
				}else if(CH32_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(380,382,"032");
				}
				
				if(CH33_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(100,506,"033");
				}else if(CH33_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(100,506,"033");
				}
				
				if(CH34_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(140,506,"034");
				}else if(CH34_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(140,506,"034");
				}
				
				if(CH35_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(180,506,"035");
				}else if(CH35_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(180,506,"035");
				}
				
				if(CH36_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(220,506,"036");
				}else if(CH36_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(220,506,"036");
				}
				
				if(CH37_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(260,506,"037");
				}else if(CH37_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(260,506,"037");
				}
				
				if(CH38_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(300,506,"038");
				}else if(CH38_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(300,506,"038");
				}
				
				if(CH39_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(340,506,"039");
				}else if(CH39_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(340,506,"039");
				}
				
				if(CH40_SW == ch_off)
				{
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
					DISP_CNL_S(380,506,"040");
				}else if(CH40_SW == ch_on){
					LCD_SetBackColor(LCD_COLOR_BACK);
					LCD_SetTextColor(LCD_COLOR_HLT);
					DISP_CNL_S(380,506,"040");
				}
			}
		}	
		LCD_SetBackColor(LCD_COLOR_BACK);
		LCD_SetTextColor(LCD_COLOR_WHITE);
	}
	LCD_SetBackColor(LCD_COLOR_BACK);
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	DISP_AVG(420+2,400-5,"AVG");
	DISP_AVG(420+2,480-5,"MAX");
	DISP_AVG(420+2,560-5,"MIN");
	LCD_SetColors(LCD_COLOR_LIGHTBLUE,LCD_COLOR_BACK);
	LCD_DrawFullRect(385+5,415,2,25);
	LCD_DrawFullRect(466+5,415,2,25);
	LCD_DrawFullRect(547+5,415,2,25);
	LCD_DrawFullRect(630+5,415,2,25);
	LCD_DrawFullRect(385+5,415,247,2);

	
	page_flag = display;
	op_flag = home_type;
}
//��ʾ��ͨ���ȵ�ż����
void Disp_Type(void)
{
	u8 i;
	const u8 (*pt)[4];
	pt = TC_Type;
	LCD_SetBackColor(LCD_COLOR_BACK);
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	if(FONT == big)
	{	
		if(ch_page == page1)
		{
			DISP_AVG(120,270,"    ");
			DISP_AVG(200,270,"    ");
			DISP_AVG(280,270,"    ");
			DISP_AVG(360,270,"    ");
			DISP_AVG(120,590,"    ");
			DISP_AVG(200,590,"    ");
			DISP_AVG(280,590,"    ");
			DISP_AVG(360,590,"    ");
			
			DISP_AVG(120,270,pt[CH1TYPE-1]);
			DISP_AVG(200,270,pt[CH2TYPE-1]);
			DISP_AVG(280,270,pt[CH3TYPE-1]);
			DISP_AVG(360,270,pt[CH4TYPE-1]);
			DISP_AVG(120,590,pt[CH5TYPE-1]);
			DISP_AVG(200,590,pt[CH6TYPE-1]);
			DISP_AVG(280,590,pt[CH7TYPE-1]);
			DISP_AVG(360,590,pt[CH8TYPE-1]);
			for(i=60;i<67;i++)
			{
				if(savedata[i] == 11)
				{
					if(i<64)
					{
						DISP_TEMP_L(95+80*(i-60),240,(uint8_t *)"%",0);
					}else{
						DISP_TEMP_L(95+80*(i-64),560,(uint8_t *)"%",0);
					}
				}
			}
		}else if(ch_page == page2){
			DISP_AVG(120,270,"    ");
			DISP_AVG(200,270,"    ");
			DISP_AVG(280,270,"    ");
			DISP_AVG(360,270,"    ");
			DISP_AVG(120,590,"    ");
			DISP_AVG(200,590,"    ");
			DISP_AVG(280,590,"    ");
			DISP_AVG(360,590,"    ");
			
			DISP_AVG(120,270,pt[CH9TYPE-1]);
			DISP_AVG(200,270,pt[CH10TYPE-1]);
			DISP_AVG(280,270,pt[CH11TYPE-1]);
			DISP_AVG(360,270,pt[CH12TYPE-1]);
			DISP_AVG(120,590,pt[CH13TYPE-1]);
			DISP_AVG(200,590,pt[CH14TYPE-1]);
			DISP_AVG(280,590,pt[CH15TYPE-1]);
			DISP_AVG(360,590,pt[CH16TYPE-1]);
		}
	}else if(FONT == middle){
			DISP_AVG(112,190,"   ");
			DISP_AVG(152,190,"   ");
			DISP_AVG(192,190,"   ");
			DISP_AVG(232,190,"   ");
			DISP_AVG(272,190,"   ");
			DISP_AVG(312,190,"   ");
			DISP_AVG(352,190,"   ");
			DISP_AVG(392,190,"   ");
			if(CHNUM == 16)
			{
				DISP_AVG(112,510,"   ");
				DISP_AVG(152,510,"   ");
				DISP_AVG(192,510,"   ");
				DISP_AVG(232,510,"   ");
				DISP_AVG(272,510,"   ");
				DISP_AVG(312,510,"   ");
				DISP_AVG(352,510,"   ");
				DISP_AVG(392,510,"   ");
			}
			
			DISP_AVG(112,190,pt[CH1TYPE-1]);
			DISP_AVG(152,190,pt[CH2TYPE-1]);
			DISP_AVG(192,190,pt[CH3TYPE-1]);
			DISP_AVG(232,190,pt[CH4TYPE-1]);
			DISP_AVG(272,190,pt[CH5TYPE-1]);
			DISP_AVG(312,190,pt[CH6TYPE-1]);
			DISP_AVG(352,190,pt[CH7TYPE-1]);
			DISP_AVG(392,190,pt[CH8TYPE-1]);
			if(CHNUM == 16)
			{
				DISP_AVG(112,510,pt[CH9TYPE-1]);
				DISP_AVG(152,510,pt[CH10TYPE-1]);
				DISP_AVG(192,510,pt[CH11TYPE-1]);
				DISP_AVG(232,510,pt[CH12TYPE-1]);
				DISP_AVG(272,510,pt[CH13TYPE-1]);
				DISP_AVG(312,510,pt[CH14TYPE-1]);
				DISP_AVG(352,510,pt[CH15TYPE-1]);
				DISP_AVG(392,510,pt[CH16TYPE-1]);
			}
	}else if(FONT == small){
			DISP_AVG(104,115,"  ");
			DISP_AVG(144,115,"  ");
			DISP_AVG(184,115,"  ");
			DISP_AVG(224,115,"  ");
			DISP_AVG(264,115,"  ");
			DISP_AVG(304,115,"  ");
			DISP_AVG(344,115,"  ");
			DISP_AVG(384,115,"  ");
		    if(CHNUM == 16)
			{
				DISP_AVG(104,239,"  ");
				DISP_AVG(144,239,"  ");
				DISP_AVG(184,239,"  ");
				DISP_AVG(224,239,"  ");
				DISP_AVG(264,239,"  ");
				DISP_AVG(304,239,"  ");
				DISP_AVG(344,239,"  ");
				DISP_AVG(384,239,"  ");
			}
			
			DISP_AVG(104,115,pt[CH1TYPE-1]);
			DISP_AVG(144,115,pt[CH2TYPE-1]);
			DISP_AVG(184,115,pt[CH3TYPE-1]);
			DISP_AVG(224,115,pt[CH4TYPE-1]);
			DISP_AVG(264,115,pt[CH5TYPE-1]);
			DISP_AVG(304,115,pt[CH6TYPE-1]);
			DISP_AVG(344,115,pt[CH7TYPE-1]);
			DISP_AVG(384,115,pt[CH8TYPE-1]);
			if(CHNUM == 16)
			{
				DISP_AVG(104,239,pt[CH9TYPE-1]);
				DISP_AVG(144,239,pt[CH10TYPE-1]);
				DISP_AVG(184,239,pt[CH11TYPE-1]);
				DISP_AVG(224,239,pt[CH12TYPE-1]);
				DISP_AVG(264,239,pt[CH13TYPE-1]);
				DISP_AVG(304,239,pt[CH14TYPE-1]);
				DISP_AVG(344,239,pt[CH15TYPE-1]);
				DISP_AVG(384,239,pt[CH16TYPE-1]);
			}
	}
	Save_flag();
	
}