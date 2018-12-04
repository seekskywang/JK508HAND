/**
  ******************************************************************************
  * @file    sysytem.c
  * @author  fire
  * @version V1.0
  * @date    2018-04-29
  * @brief   手持多路温度测试仪系统配置界面
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
#include "./flash/bsp_spi_flash.h"
#include "jk508.h"
#include "./RTC/bsp_rtc.h"

u8 Data_buf[16][16];
float hisdata[16][500];
u8 histime[10][7];
float YLIMIT[3];
u16 his_config[10];
u8 his_time[10][7];

//读取的ID存储位置
extern __IO uint32_t DeviceID;
extern __IO uint32_t FlashID;

extern union 
{
   unsigned char FLAG_VAL7;
	
   struct 
   {
       unsigned char  FN:2;    //字体
       unsigned char  SP:2;    //速度
       unsigned char  BD:3;    //波特
       unsigned char  LG:1;	   //语言
   }BIT_FLAG;
}FLAG7;


void page_his(void)
{
	range = (YHLIMIT - YLLIMIT) / 7;
	enrate = 350/(float)(YHLIMIT - YLLIMIT);
	u8 i;
	u16 j;
	char timetemp[100];
	char buf[10];
	
	
	
	
   /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
    LCD_SetTransparency(0xFF);
	LCD_Clear(LCD_COLOR_BACK);
	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/
	LCD_SetTextColor(LCD_COLOR_HLT);  	
	LCD_SetBackColor(LCD_COLOR_BACK);
	LCD_DisplayStringLine(0,10, "<      >");
	LCD_DisplayStringLine(5,26, "历史曲线");
	
	DrawMenu();
	Drawhishmenu();
	
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_SetBackColor(LCD_COLOR_BACK);
	for(i = 0;i < 7;i ++)
	{
		sprintf(buf,"%d",(int)(YHLIMIT - range * i));
		DISP_CNL_S(40 + 50*i,5/*90*/,(uint8_t* )buf);
	}
//	Read_history(1);
//	Read_time(1);
	
	DrawGridLine();
	
//	for(j = 0; j < 500; j++)
//	{
//		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
//		LCD_DrawUniLine(111 + j,(int)(400-((Data_buf[j] - YLLIMIT)*enrate)),112+j,(400-((Data_buf[j+1] - YLLIMIT)*enrate)));
//	}
	for(j = 0; j < 495; j++)
	{
		if(CH1_SW == ch_on && hisdata[0][j] < YHLIMIT && hisdata[0][j] > YLLIMIT && hisdata[0][j+1] < YHLIMIT && hisdata[0][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[0][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[0][j+1] - YLLIMIT)*enrate)));
		}
		if(CH2_SW == ch_on && hisdata[1][j] < YHLIMIT && hisdata[1][j] > YLLIMIT && hisdata[1][j+1] < YHLIMIT && hisdata[1][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[1][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[1][j+1] - YLLIMIT)*enrate)));
		}
		if(CH3_SW == ch_on && hisdata[2][j] < YHLIMIT && hisdata[2][j] > YLLIMIT && hisdata[2][j+1] < YHLIMIT && hisdata[2][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_MAGENTA,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[2][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[2][j+1] - YLLIMIT)*enrate)));
		}
		if(CH4_SW == ch_on && hisdata[3][j] < YHLIMIT && hisdata[3][j] > YLLIMIT && hisdata[3][j+1] < YHLIMIT && hisdata[3][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_CYAN,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[3][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[3][j+1] - YLLIMIT)*enrate)));
		}		
		if(CH5_SW == ch_on && hisdata[4][j] < YHLIMIT && hisdata[4][j] > YLLIMIT && hisdata[4][j+1] < YHLIMIT && hisdata[4][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[4][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[4][j+1] - YLLIMIT)*enrate)));
		}
		if(CH6_SW == ch_on && hisdata[5][j] < YHLIMIT && hisdata[5][j] > YLLIMIT && hisdata[5][j+1] < YHLIMIT && hisdata[5][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_HLT,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[5][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[5][j+1] - YLLIMIT)*enrate)));
		}
		if(CH7_SW == ch_on && hisdata[6][j] < YHLIMIT && hisdata[6][j] > YLLIMIT && hisdata[6][j+1] < YHLIMIT && hisdata[6][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_BT,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[6][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[6][j+1] - YLLIMIT)*enrate)));
		}
		if(CH8_SW == ch_on && hisdata[7][j] < YHLIMIT && hisdata[7][j] > YLLIMIT && hisdata[7][j+1] < YHLIMIT && hisdata[7][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[7][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[7][j+1] - YLLIMIT)*enrate)));
		}
		if(CH9_SW == ch_on && hisdata[8][j] < YHLIMIT && hisdata[8][j] > YLLIMIT && hisdata[8][j+1] < YHLIMIT && hisdata[8][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[8][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[8][j+1] - YLLIMIT)*enrate)));
		}
		if(CH10_SW == ch_on && hisdata[1][9] < YHLIMIT && hisdata[1][9] > YLLIMIT && hisdata[9][j+1] < YHLIMIT && hisdata[9][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR1,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[9][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[9][j+1] - YLLIMIT)*enrate)));
		}
		if(CH11_SW == ch_on && hisdata[10][j] < YHLIMIT && hisdata[10][j] > YLLIMIT && hisdata[10][j+1] < YHLIMIT && hisdata[10][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR2,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[10][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[10][j+1] - YLLIMIT)*enrate)));
		}
		if(CH12_SW == ch_on && hisdata[11][j] < YHLIMIT && hisdata[11][j] > YLLIMIT && hisdata[11][j+1] < YHLIMIT && hisdata[11][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR3,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[11][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[11][j+1] - YLLIMIT)*enrate)));
		}
		if(CH13_SW == ch_on && hisdata[12][j] < YHLIMIT && hisdata[12][j] > YLLIMIT && hisdata[12][j+1] < YHLIMIT && hisdata[12][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR4,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[12][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[12][j+1] - YLLIMIT)*enrate)));
		}
		if(CH14_SW == ch_on && hisdata[13][j] < YHLIMIT && hisdata[13][j] > YLLIMIT && hisdata[13][j+1] < YHLIMIT && hisdata[13][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR5,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[13][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[13][j+1] - YLLIMIT)*enrate)));
		}
		if(CH15_SW == ch_on && hisdata[14][j] < YHLIMIT && hisdata[14][j] > YLLIMIT && hisdata[14][j+1] < YHLIMIT && hisdata[14][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR6,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[14][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[14][j+1] - YLLIMIT)*enrate)));
		}
		if(CH16_SW == ch_on && hisdata[15][j] < YHLIMIT && hisdata[15][j] > YLLIMIT && hisdata[15][j+1] < YHLIMIT && hisdata[15][j+1] > YLLIMIT)
		{
			LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BACK);
			LCD_DrawUniLine(111-80 + j,(int)(400-((hisdata[15][j] - YLLIMIT)*enrate)),112 - 80 + j,(400-((hisdata[15][j+1] - YLLIMIT)*enrate)));
		}
	}
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetBackColor(LCD_COLOR_BACK);
	sprintf(timetemp,"%d%0.2d-%0.2d-%0.2d",
			histime[0][0],
			histime[0][1],
			histime[0][2],
			histime[0][3]);		
	LCD_DisplayStringLine(10,200,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
			histime[0][4],
			histime[0][5],
			histime[0][6]);
	DISP_CNL_S(402,80-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[1][4],
		histime[1][5],
		histime[1][6]);
	DISP_CNL_S(420,130-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[2][4],
		histime[2][5],
		histime[2][6]);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	DISP_CNL_S(402,180-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[3][4],
		histime[3][5],
		histime[3][6]);
	DISP_CNL_S(420,230-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[4][4],
		histime[4][5],
		histime[4][6]);
	DISP_CNL_S(402,280-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[5][4],
		histime[5][5],
		histime[5][6]);
	DISP_CNL_S(420,330-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[6][4],
		histime[6][5],
		histime[6][6]);
	DISP_CNL_S(402,380-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[7][4],
		histime[7][5],
		histime[7][6]);
	DISP_CNL_S(420,430-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[8][4],
		histime[8][5],
		histime[8][6]);
	DISP_CNL_S(402,480-80,(uint8_t *)timetemp);
	
	sprintf(timetemp,"%0.2d:%0.2d:%0.2d",
		histime[9][4],
		histime[9][5],
		histime[9][6]);
	DISP_CNL_S(420,530-80,(uint8_t *)timetemp);
	
	
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_RED);
	LCD_DrawLine(540,100-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(90-30,10+540,"001");
	
	LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_GREEN);
	LCD_DrawLine(540,120-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(110-30,10+540,"002");
	
	LCD_SetColors(LCD_COLOR_MAGENTA,LCD_COLOR_GREEN);
	LCD_DrawLine(540,140-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(130-30,10+540,"003");
	
	LCD_SetColors(LCD_COLOR_CYAN,LCD_COLOR_GREEN);
	LCD_DrawLine(540,160-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(150-30,10+540,"004");
	
	LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_GREEN);
	LCD_DrawLine(540,180-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(170-30,10+540,"005");
	
	LCD_SetColors(LCD_COLOR_HLT,LCD_COLOR_GREEN);
	LCD_DrawLine(540,200-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(190-30,10+540,"006");
	
	LCD_SetColors(LCD_COLOR_BT,LCD_COLOR_GREEN);
	LCD_DrawLine(540,220-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(210-30,10+540,"007");
	
	LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_GREEN);
	LCD_DrawLine(540,240-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(230-30,10+540,"008");
	
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_GREEN);
	LCD_DrawLine(540,260-30,10,LCD_DIR_HORIZONTAL);
	LCD_SetBackColor(LCD_COLOR_BACK);
	DISP_CNL_S(250-30,10+540,"009");
	
	if(CHNUM > 8)
	{
		LCD_SetColors(LCD_COLOR_GR1,LCD_COLOR_GREEN);
		LCD_DrawLine(540,280-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(270-30,10+540,"010");
		
		LCD_SetColors(LCD_COLOR_GR2,LCD_COLOR_GREEN);
		LCD_DrawLine(540,300-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(290-30,10+540,"011");
		
		LCD_SetColors(LCD_COLOR_GR3,LCD_COLOR_GREEN);
		LCD_DrawLine(540,320-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(310-30,10+540,"012");
		
		LCD_SetColors(LCD_COLOR_GR4,LCD_COLOR_GREEN);
		LCD_DrawLine(540,340-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(330-30,10+540,"013");
		
		LCD_SetColors(LCD_COLOR_GR5,LCD_COLOR_GREEN);
		LCD_DrawLine(540,360-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(350-30,10+540,"014");
		
		LCD_SetColors(LCD_COLOR_GR6,LCD_COLOR_GREEN);
		LCD_DrawLine(540,380-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(370-30,10+540,"015");
		
		LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_GREEN);
		LCD_DrawLine(540,400-30,10,LCD_DIR_HORIZONTAL);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(390-30,10+540,"016");
	}
	
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
	sprintf(buf,"%d/%d",hpage,1000);
	DISP_CNL_S(390,550,buf);

	page_flag = history;
}

void Save_history(u16 rec)
{
//	SPI_FLASH_SectorErase(8192 + 4096 * (rec-1));
	if(rec%16 == 1)
	{
		SPI_FLASH_SectorErase(8192+4096*(rec/16));
	}
	SPI_FLASH_BufferWrite((void*)Data_buf,8192+SPI_FLASH_PageSize*(rec-1), sizeof(Data_buf));
	memset(Data_buf,0,sizeof(time_buf));
}

void Read_history(u16 rec)
{	
	u8 read_buf[16][16];
	u8 i,j,k;
	for(i = 0;i < 62;i++)
	{
		SPI_FLASH_BufferRead((void *)read_buf,8192+15872*(rec-1)+i*256, sizeof(read_buf));
		for(j = 0;j < 16;j ++)
		{
			for(k = 0;k < 8;k++)
			{
				hisdata[j][i*8+k] = (read_buf[j][k%8*2] * 256 +  read_buf[j][k%8*2+1])/10.0;
			}
		}
	}
}

void Save_time(u16 rec)
{
	
	if(rec%16 == 1)
	{
		SPI_FLASH_SectorErase(15880192+4096*(rec/16));
	}
	SPI_FLASH_BufferWrite((void*)time_buf,15880192+SPI_FLASH_PageSize*(rec-1), sizeof(time_buf));
	memset(time_buf,0,sizeof(time_buf));
}

void Read_time(u16 rec)
{
//	memset(histime,0,sizeof(histime));
	SPI_FLASH_BufferRead((void *)histime,15880192+SPI_FLASH_PageSize*(rec-1), sizeof(histime));
}
	
void Save_Sflag(void)
{
	SPI_FLASH_SectorErase(1*4096);
	SPI_FLASH_BufferWrite((void*)his_config,SPI_FLASH_PageSize*16, sizeof(his_config));
}

void Read_Sflag(void)
{
	SPI_FLASH_BufferRead((void *)his_config,SPI_FLASH_PageSize*16, sizeof(his_config));
}

void hispage(u16 page)
{
	Read_history(page);
	Read_time(page);
	page_his();
}
