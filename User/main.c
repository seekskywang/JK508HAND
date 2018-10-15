/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC液晶显示汉字(显示任意大小)
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./key/bsp_key.h"
#include "./beep/bsp_beep.h"
#include "./tim/bsp_basic_tim.h"
#include "./flash/bsp_spi_flash.h"
#include "./systick/bsp_SysTick.h"
#include "./RTC/bsp_rtc.h"
#include "jk508.h"
#include  "usbd_hid_core.h"
#include  "usbd_usr.h"
#include  "usbd_desc.h"
#include "./internalFlash/bsp_internalFlash.h"
#include "./ch376/ch376.h"
#include "touchscreen.h"
#include "./FILESYS/FILESYS.h"
#include <string.h>

/** @defgroup APP_HID_Private_Variables
  * @{
  */ 


void TempDisplay(void);
u8 usbbuf[0x40];
u8 usbsendbuf[0x40];
u8 savedata[80];
u8 uinfo[64];
//u8 usbreadtime = 0;
u8 eqmtstatus;
u16 crcwatch;
uint16_t readcrc;
u8 *crec;
u8 *csend;
u16 datasize;
u8 usbstatus = UNKNOWN;
u16 watch;
//u8 p1,p2,p3,p4,p5,p6,p7,p8;


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


union 
{
   unsigned char FLAG_VAL1;
		
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

union 
{
   unsigned char FLAG_VAL2;
		
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

union 
{
   unsigned char FLAG_VAL3;
		
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

union 
{
   unsigned char FLAG_VAL4;
		
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

union 
{
   unsigned char FLAG_VAL5;
		
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

union 
{
   unsigned char FLAG_VAL6;
	
   struct 
   {
       unsigned char  TC:4;    //热电偶类型
       unsigned char  FT:1;    //分选/比较
       unsigned char  BP:1;    //讯响
       unsigned char  UT:2;	   //单位
   }BIT_FLAG;
}FLAG6;

union 
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

union 
{
   unsigned char FLAG_VAL8;
	
   struct 
   {
       unsigned char  BR:3;    //字体
       unsigned char  SP:3;    //速度
       
   }BIT_FLAG;
}FLAG8;

typedef __packed struct {
    u8 uCmd;
    u8 Size;
    u32 u32Arg1;
    u32 u32Arg2;
    u32 u32Signature;
    u32 u32Checksum;
}CMD_T;

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;


void Delay(__IO u32 nCount); 
void LCD_Test(void);
void Printf_Charater(void);
int i;
u16 count = 0;

float ch_temp[40];

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */

int main(void)
{
	u8 test[9] = {0X01,0X03,0X02,0X58,0X00,0X01,0X02,0X00,0X05};
	 __IO uint32_t i = 0;
//	u8 res;
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */  
		/*初始化SDRAM模块*/
	SDRAM_Init();
	 /*初始化液晶屏*/
    LCD_Init();

    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_BLACK);

	power_on();
	
	SysTick_Init();
	Delay(500);
	/* LED 端口初始化 */
//	LED_GPIO_Config();	 
	
	/* 16M串行flash W25Q128初始化 */
	SPI_FLASH_Init();
	
		/*串口初始化*/
	Debug_USART_Config();
	 
	/* 获取 Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	Touch_GPIO_Config();
	Delay(200);
	  
	
	/*按键初始化*/
	Key_GPIO_Config();
//	/* 初始化系统滴答定时器 */
	

	/*蜂鸣器端口初始化 */
	Beep_GPIO_Config();
	//设置芯片读保护
//	Flash_EnableReadProtection();
	
	//	  /* 初始化通用定时器定时，20m+s产生一次中断 */
	TIMx_Configuration();
	
	/*CH376引脚配置初始化*/
	CH376_GPIO_Init();
    
	
//	Delay_ms(100);
	
	Init_CH376();
	
//	if(res == USB_INT_SUCCESS)
//	{
//		udisk = udisk_scan();
//	}
	
	/* RTC配置：选择时钟源，设置RTC_CLK的分频系数 */
	RTC_CLK_Config();
	
	
	if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		/* 设置时间和日期 */
		RTC_TimeAndDate_Set();
	}
	else
	{
	/* 检查是否电源复位 */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		 printf("\r\n 发生电源复位....\r\n");
	}
	/* 检查是否外部复位 */
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		 printf("\r\n 发生外部复位....\r\n");
	}

	printf("\r\n 不需要重新配置RTC....\r\n");
		
	/* 使能 PWR 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
	PWR_BackupAccessCmd(ENABLE);
	/* 等待 RTC APB 寄存器同步 */
	RTC_WaitForSynchro();
	}
	
	USBD_Init(&USB_OTG_dev,USB_OTG_HS_CORE_ID,
           &USR_desc,&USBD_HID_cb,&USR_cb);
		/* 获取 SPI Flash ID */
	
	
	FlashID = SPI_FLASH_ReadID();
	if (FlashID == sFLASH_ID) 
	{
		Delay(500);
		Read_flag();
		Read_Sflag();
	}
	watch = CRC16(test,9);
//	page_home();
//	watch = sizeof(TempHLimits);
	
//	SECTOR_REC = 0;
	while(1)
	{
//		watch = GPIO_ReadInputDataBit(TOUCH_YPLUS_GPIO_PORT,TOUCH_YPLUS_GPIO_PIN);
		/* 显示时间和日期 */	
		RTC_TimeAndDate_Show();
		/*按键扫描*/		
		Key_Function();
//		CH1TEMP = (RecBuff[21] * 256 + RecBuff[22])/10.0;
		DrawBattery(battery);
		
		TempDisplay();
		BEEP_ON;
		Delay(0xfff);
		BEEP_OFF;
//		DCD_EP_PrepareRx(&USB_OTG_dev,HID_OUT_EP,usbbuf,64);//接收PC数据
//		if(UsbHidReceiveComplete)                         //接收到数据
//		{
//			UsbHidReceiveComplete=0;
//			UsbDataHandle();
//		}
	
	}
}


/*用于测试各种液晶的函数*/
void LCD_Test(void)
{
//	static uint32_t testCNT=0;
//	char dispBuff[100];
//	char buf1[5];
//	char buf2[5];
//	char buf3[5];
//	char buf4[5];
//	char buf5[5];
//	char buf6[5];
//	char buf7[5];
//	char buf8[5];
//	p1 = GPIO_ReadInputDataBit(P1_GPIO_PORT,P1_PIN);
//	p2 = GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN);
//	p3 = GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN);
//	p4 = GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN);
//	p5 = GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN);
//	p6 = GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN);
//	p7 = GPIO_ReadInputDataBit(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN);
//	
//	u8 p1o = GPIO_ReadOutputDataBit(P1_GPIO_PORT,P1_PIN);
//	u8 p2o = GPIO_ReadOutputDataBit(P2_GPIO_PORT,P2_PIN);
//	u8 p3o = GPIO_ReadOutputDataBit(P3_GPIO_PORT,P3_PIN);
//	u8 p4o = GPIO_ReadOutputDataBit(P4_GPIO_PORT,P4_PIN);
//	u8 p5o = GPIO_ReadOutputDataBit(P3_GPIO_PORT,P5_PIN);
//	

//	/*使用不透明前景层*/
////	LCD_SetLayer(LCD_FOREGROUND_LAYER);  
////	LCD_SetTransparency(0xff);
//	
////  LCD_Clear(LCD_COLOR_BLACK);	/* 清屏，显示全黑 */

////	/*设置字体颜色及字体的背景颜色(此处的背景不是指LCD的背景层！注意区分)*/
//  LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
//	
	/*选择字体，使用中英文显示时，尽量把英文选择成16*24的字体，
	*中文字体大小是24*24的，需要其它字体请自行制作字模*/
	/*这个函数只对英文字体起作用*/
//  LCD_SetFont(&Font16x24); 	
//	if(GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN))
//		sprintf(buf1,"%d",sizeof(u64));
//		sp	rintf(buf2,"%d",GPIO_ReadInputDataBit(P2_GPIO_PORT,P2_PIN));
//		sprintf(buf3,"%d",GPIO_ReadInputDataBit(P3_GPIO_PORT,P3_PIN));
//		sprintf(buf4,"%d",GPIO_ReadInputDataBit(P4_GPIO_PORT,P4_PIN));
//		sprintf(buf5,"%d",GPIO_ReadInputDataBit(P5_GPIO_PORT,P5_PIN));
//		sprintf(buf6,"%d",GPIO_ReadInputDataBit(P6_GPIO_PORT,P6_PIN));
//		sprintf(buf8,"%d",GPIO_ReadInputDataBit(LTDC_BL_GPIO_PORT,LTDC_BL_GPIO_PIN));
//		sprintf(buf7,"%d",count);
//		LCD_DisplayStringLine(50,200,(uint8_t* )buf1);
//		LCD_DisplayStringLine(70,200,(uint8_t* )buf2);
//		LCD_DisplayStringLine(90,200,(uint8_t* )buf3);
//		LCD_DisplayStringLine(110,200,(uint8_t* )buf4);
//		LCD_DisplayStringLine(130,200,(uint8_t* )buf5);
//		LCD_DisplayStringLine(150,200,(uint8_t* )buf6);
//		LCD_DisplayStringLine(170,200,(uint8_t* )buf7);
//		LCD_DisplayStringLine(190,200,(uint8_t* )buf8);
		
}

/*GERNERAL CODES*/
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

uint16_t CRC16(uint8_t *puchMsg, uint8_t Len)
{
	uint8_t t, m,n,p;
	uint8_t uchCRCHi=0xFF; /* 高CRC字节初始化*/ 
	uint8_t uchCRCLo =0xFF; /* 低CRC 字节初始化*/ 
	for(t=0;t<Len;t++)
	{	
		uchCRCLo=uchCRCLo^puchMsg[t];
		for(n=0;n<8;n++)
		{
			m=uchCRCLo&1;p=uchCRCHi&1;uchCRCHi>>=1;
			uchCRCLo>>=1;

			if(p)
			{
				uchCRCLo|=0x80;
			}
			if(m)	
			{
				uchCRCHi=uchCRCHi^0xa0;
				uchCRCLo=uchCRCLo^1;
			}
		}
	}
	return (uchCRCHi<<8|uchCRCLo);
}
//开机亮度
void InitBrt(void)
{
	if(BRTS == L0)
	{
		brightness = 10;
	}else if(BRTS == L1){
		brightness = 20;
	}else if(BRTS == L2){
		brightness = 40;
	}else if(BRTS == L3){
		brightness = 60;
	}else if(BRTS == L1){
		brightness = 80;
	}
	TIM_PWMOUTPUT_Config(brightness);
}
//温度显示
void TempDisplay(void)
{
	char buf[10];
	static u8 eqmtstatus;
	u8 i;
		
	if(page_flag == display)
	{		
//		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		sprintf(buf,"%03d",charge);
//		LCD_DisplayStringLine_48(47,200,(uint8_t*)buf);
//		sprintf(buf,"%03d",battery);
//		LCD_DisplayStringLine_48(47,260,(uint8_t*)buf);
		if(eqmtstatus < 60)
		{
			if(LANG == chs)
			{
				LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
				LCD_DisplayStringLine(5,180,"数据采集中");
			}else if(LANG == eng){
				LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
				DISP_INS(10,130,"Collecting data");
			}
			eqmtstatus++;
		}else if(eqmtstatus >= 60 && eqmtstatus < 120){
			if(LANG == chs)
			{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(180,5,130,32);
			}else if(LANG == eng){
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(130,10,235,25);
			}
			eqmtstatus++;
		}else if(eqmtstatus >= 120){
			eqmtstatus = 0;
		}
		
		if(FONT == big)
		{					
			if(ch_page == page1)
			{							
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(1);
				sprintf(buf,"%.1f",CH1TEMP - COR1);
				if(CH1TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH1TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(95,150,(uint8_t*)buf,CH1_SW);
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH2TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(165,150,(uint8_t*)buf,CH2_SW);
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH3TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(235,150,(uint8_t*)buf,CH3_SW);
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH4TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(305,150,(uint8_t*)buf,CH4_SW);
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH5TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(95,470,(uint8_t*)buf,CH5_SW);
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH6TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(165,470,(uint8_t*)buf,CH6_SW);
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH7TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(235,470,(uint8_t*)buf,CH7_SW);
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR7);
				if(CH8TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH8TEMP < 1000){
					strcat(buf," ");
				}				
				DISP_TEMP_L(305,470,(uint8_t*)buf,CH8_SW);
			}else if(ch_page == page2){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(9);
				sprintf(buf,"%.1f",CH9TEMP - COR9);
				if(CH9TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH9TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(95,150,(uint8_t*)buf,CH9_SW);
				Check_limits(10);
				sprintf(buf,"%.1f",CH10TEMP - COR10);
				if(CH10TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH10TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(165,150,(uint8_t*)buf,CH10_SW);
				Check_limits(11);
				sprintf(buf,"%.1f",CH11TEMP - COR11);
				if(CH11TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH11TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(235,150,(uint8_t*)buf,CH11_SW);
				Check_limits(12);
				sprintf(buf,"%.1f",CH12TEMP - COR12);
				if(CH12TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH12TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(305,150,(uint8_t*)buf,CH12_SW);
				Check_limits(13);
				sprintf(buf,"%.1f",CH13TEMP - COR13);
				if(CH13TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH13TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(95,470,(uint8_t*)buf,CH13_SW);
				Check_limits(14);
				sprintf(buf,"%.1f",CH14TEMP);
				if(CH14TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH14TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(165,470,(uint8_t*)buf,CH14_SW);
				Check_limits(15);
				sprintf(buf,"%.1f",CH15TEMP - COR15);
				if(CH15TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH15TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(235,470,(uint8_t*)buf,CH15_SW);
				Check_limits(16);
				sprintf(buf,"%.1f",CH16TEMP - COR16);
				if(CH16TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH16TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_L(305,470,(uint8_t*)buf,CH16_SW); 
			}else if(ch_page == page3){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH17TEMP);
				DISP_TEMP_L(95,150,(uint8_t*)buf,CH17_SW);
				sprintf(buf,"%.1f",CH18TEMP);
				DISP_TEMP_L(165,150,(uint8_t*)buf,CH18_SW);
				sprintf(buf,"%.1f",CH19TEMP);
				DISP_TEMP_L(235,150,(uint8_t*)buf,CH19_SW);
				sprintf(buf,"%.1f",CH20TEMP);
				DISP_TEMP_L(305,150,(uint8_t*)buf,CH20_SW);
				sprintf(buf,"%.1f",CH21TEMP);
				DISP_TEMP_L(95,470,(uint8_t*)buf,CH21_SW);
				sprintf(buf,"%.1f",CH22TEMP);
				DISP_TEMP_L(165,470,(uint8_t*)buf,CH22_SW);
				sprintf(buf,"%.1f",CH23TEMP);
				DISP_TEMP_L(235,470,(uint8_t*)buf,CH23_SW);
				sprintf(buf,"%.1f",CH24TEMP);
				DISP_TEMP_L(305,470,(uint8_t*)buf,CH24_SW); 
			}else if(ch_page == page4){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH25TEMP);
				DISP_TEMP_L(95,150,(uint8_t*)buf,CH25_SW);
				sprintf(buf,"%.1f",CH26TEMP);
				DISP_TEMP_L(165,150,(uint8_t*)buf,CH26_SW);
				sprintf(buf,"%.1f",CH27TEMP);
				DISP_TEMP_L(235,150,(uint8_t*)buf,CH27_SW);
				sprintf(buf,"%.1f",CH28TEMP);
				DISP_TEMP_L(305,150,(uint8_t*)buf,CH28_SW);
				sprintf(buf,"%.1f",CH29TEMP);
				DISP_TEMP_L(95,470,(uint8_t*)buf,CH29_SW);
				sprintf(buf,"%.1f",CH30TEMP);
				DISP_TEMP_L(165,470,(uint8_t*)buf,CH30_SW);
				sprintf(buf,"%.1f",CH31TEMP);
				DISP_TEMP_L(235,470,(uint8_t*)buf,CH31_SW);
				sprintf(buf,"%.1f",CH32TEMP);
				DISP_TEMP_L(305,470,(uint8_t*)buf,CH32_SW); 
			}else if(ch_page == page5){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH33TEMP);
				DISP_TEMP_L(95,150,(uint8_t*)buf,CH33_SW);
				sprintf(buf,"%.1f",CH34TEMP);
				DISP_TEMP_L(165,150,(uint8_t*)buf,CH34_SW);
				sprintf(buf,"%.1f",CH35TEMP);
				DISP_TEMP_L(235,150,(uint8_t*)buf,CH35_SW);
				sprintf(buf,"%.1f",CH36TEMP);
				DISP_TEMP_L(305,150,(uint8_t*)buf,CH36_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_L(95,470,(uint8_t*)buf,CH37_SW);
				sprintf(buf,"%.1f",CH38TEMP);
				DISP_TEMP_L(165,470,(uint8_t*)buf,CH38_SW);
				sprintf(buf,"%.1f",CH39TEMP);
				DISP_TEMP_L(235,470,(uint8_t*)buf,CH39_SW);
				sprintf(buf,"%.1f",CH40TEMP);
				DISP_TEMP_L(305,470,(uint8_t*)buf,CH40_SW); 
			}
		}else if(FONT == middle){
			if(reflag == 1)
			{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(90,100,130,305);
				LCD_DrawFullRect(290,100,130,305);
				LCD_DrawFullRect(490,100,130,305);
			}
			if(ch_page == page1)
			{				
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(1);
				sprintf(buf,"%.1f",CH1TEMP - COR1);
				if(CH1TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH1TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(100,90,(uint8_t*)buf,CH1_SW);
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH2TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(140,90,(uint8_t*)buf,CH2_SW);
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH3TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(180,90,(uint8_t*)buf,CH3_SW);
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH4TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(220,90,(uint8_t*)buf,CH4_SW);
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH5TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(260,90,(uint8_t*)buf,CH5_SW);
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH6TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(300,90,(uint8_t*)buf,CH6_SW);
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH7TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(340,90,(uint8_t*)buf,CH7_SW);
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR8);
				if(CH8TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH8TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(380,90,(uint8_t*)buf,CH8_SW);
				Check_limits(9);
				sprintf(buf,"%.1f",CH9TEMP - COR9);
				if(CH9TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH9TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(100,290,(uint8_t*)buf,CH9_SW);
				Check_limits(10);
				sprintf(buf,"%.1f",CH10TEMP - COR10);
				if(CH10TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH10TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(140,290,(uint8_t*)buf,CH10_SW);
				Check_limits(11);
				sprintf(buf,"%.1f",CH11TEMP - COR11);
				if(CH11TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH11TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(180,290,(uint8_t*)buf,CH11_SW);
				Check_limits(12);
				sprintf(buf,"%.1f",CH12TEMP - COR12);
				if(CH12TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH12TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(220,290,(uint8_t*)buf,CH12_SW);
				Check_limits(13);
				sprintf(buf,"%.1f",CH13TEMP - COR13);
				if(CH13TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH13TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(260,290,(uint8_t*)buf,CH13_SW);
				Check_limits(14);
				sprintf(buf,"%.1f",CH14TEMP - COR14);
				if(CH14TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH14TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(300,290,(uint8_t*)buf,CH14_SW);
				Check_limits(15);
				sprintf(buf,"%.1f",CH15TEMP - COR15);
				if(CH15TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH15TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(340,290,(uint8_t*)buf,CH15_SW);
				Check_limits(16);
				sprintf(buf,"%.1f",CH16TEMP - COR16);
				if(CH16TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH16TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_M(380,290,(uint8_t*)buf,CH16_SW);
				sprintf(buf,"%.1f",CH17TEMP);
				DISP_TEMP_M(100,490,(uint8_t*)buf,CH17_SW);
				sprintf(buf,"%.1f",CH18TEMP);
				DISP_TEMP_M(140,490,(uint8_t*)buf,CH18_SW);
				sprintf(buf,"%.1f",CH19TEMP);
				DISP_TEMP_M(180,490,(uint8_t*)buf,CH19_SW);
				sprintf(buf,"%.1f",CH20TEMP);
				DISP_TEMP_M(220,490,(uint8_t*)buf,CH20_SW);
				sprintf(buf,"%.1f",CH21TEMP);
				DISP_TEMP_M(260,490,(uint8_t*)buf,CH21_SW);
				sprintf(buf,"%.1f",CH22TEMP);
				DISP_TEMP_M(300,490,(uint8_t*)buf,CH22_SW);
				sprintf(buf,"%.1f",CH23TEMP);
				DISP_TEMP_M(340,490,(uint8_t*)buf,CH23_SW);
				sprintf(buf,"%.1f",CH24TEMP);
				DISP_TEMP_M(380,490,(uint8_t*)buf,CH24_SW); 
				
			}else if(ch_page == page2){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH25TEMP);
				DISP_TEMP_M(100,90,(uint8_t*)buf,CH25_SW);
				sprintf(buf,"%.1f",CH26TEMP);
				DISP_TEMP_M(140,90,(uint8_t*)buf,CH26_SW);
				sprintf(buf,"%.1f",CH27TEMP);
				DISP_TEMP_M(180,90,(uint8_t*)buf,CH27_SW);
				sprintf(buf,"%.1f",CH28TEMP);
				DISP_TEMP_M(220,90,(uint8_t*)buf,CH28_SW);
				sprintf(buf,"%.1f",CH29TEMP);
				DISP_TEMP_M(260,90,(uint8_t*)buf,CH29_SW);
				sprintf(buf,"%.1f",CH30TEMP);
				DISP_TEMP_M(300,90,(uint8_t*)buf,CH30_SW);
				sprintf(buf,"%.1f",CH31TEMP);
				DISP_TEMP_M(340,90,(uint8_t*)buf,CH31_SW);
				sprintf(buf,"%.1f",CH32TEMP);	
				DISP_TEMP_M(380,90,(uint8_t*)buf,CH32_SW);
				sprintf(buf,"%.1f",CH33TEMP);
				DISP_TEMP_M(100,290,(uint8_t*)buf,CH33_SW);
				sprintf(buf,"%.1f",CH34TEMP);
				DISP_TEMP_M(140,290,(uint8_t*)buf,CH34_SW);
				sprintf(buf,"%.1f",CH35TEMP);
				DISP_TEMP_M(180,290,(uint8_t*)buf,CH35_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_M(220,290,(uint8_t*)buf,CH36_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_M(260,290,(uint8_t*)buf,CH37_SW);
				sprintf(buf,"%.1f",CH38TEMP);
				DISP_TEMP_M(300,290,(uint8_t*)buf,CH38_SW);
				sprintf(buf,"%.1f",CH39TEMP);
				DISP_TEMP_M(340,290,(uint8_t*)buf,CH39_SW);
				sprintf(buf,"%.1f",CH40TEMP);
				DISP_TEMP_M(380,290,(uint8_t*)buf,CH40_SW);
			}
		}else if(FONT == small){	
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(1);
				sprintf(buf,"%.1f",CH1TEMP - COR1);
				if(CH1TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH1TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(100,60,(uint8_t*)buf,CH1_SW);
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH2TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(140,60,(uint8_t*)buf,CH2_SW);
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH3TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(180,60,(uint8_t*)buf,CH3_SW);
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH4TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(220,60,(uint8_t*)buf,CH4_SW);
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH5TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(260,60,(uint8_t*)buf,CH5_SW);
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH6TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(300,60,(uint8_t*)buf,CH6_SW);
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH7TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(340,60,(uint8_t*)buf,CH7_SW);
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR8);
				if(CH8TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH8TEMP < 1000){
					strcat(buf," ");
				}				
				DISP_TEMP_S(380,60,(uint8_t*)buf,CH8_SW);
				Check_limits(9);
				sprintf(buf,"%.1f",CH9TEMP - COR9);
				if(CH9TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH9TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(100,184,(uint8_t*)buf,CH9_SW);
				Check_limits(10);
				sprintf(buf,"%.1f",CH10TEMP - COR10);
				if(CH10TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH10TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(140,184,(uint8_t*)buf,CH10_SW);
				Check_limits(11);
				sprintf(buf,"%.1f",CH11TEMP - COR11);
				if(CH11TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH11TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(180,184,(uint8_t*)buf,CH11_SW);
				Check_limits(12);
				sprintf(buf,"%.1f",CH12TEMP - COR12);
				if(CH12TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH12TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(220,184,(uint8_t*)buf,CH12_SW);
				Check_limits(13);
				sprintf(buf,"%.1f",CH13TEMP - COR13);
				if(CH13TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH13TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(260,184,(uint8_t*)buf,CH13_SW);
				Check_limits(14);
				sprintf(buf,"%.1f",CH14TEMP - COR14);
				if(CH14TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH14TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(300,184,(uint8_t*)buf,CH14_SW);
				Check_limits(15);
				sprintf(buf,"%.1f",CH15TEMP - COR15);
				if(CH15TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH15TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(340,184,(uint8_t*)buf,CH15_SW);
				Check_limits(16);
				sprintf(buf,"%.1f",CH16TEMP - COR16);
				if(CH16TEMP < 100)
				{
					strcat(buf,"  ");
				}else if(CH16TEMP < 1000){
					strcat(buf," ");
				}
				DISP_TEMP_S(380,184,(uint8_t*)buf,CH16_SW);
				sprintf(buf,"%.1f",CH17TEMP);
				DISP_TEMP_S(100,308,(uint8_t*)buf,CH17_SW);
				sprintf(buf,"%.1f",CH18TEMP);
				DISP_TEMP_S(140,308,(uint8_t*)buf,CH18_SW);
				sprintf(buf,"%.1f",CH19TEMP);
				DISP_TEMP_S(180,308,(uint8_t*)buf,CH19_SW);
				sprintf(buf,"%.1f",CH20TEMP);
				DISP_TEMP_S(220,308,(uint8_t*)buf,CH20_SW);
				sprintf(buf,"%.1f",CH21TEMP);
				DISP_TEMP_S(260,308,(uint8_t*)buf,CH21_SW);
				sprintf(buf,"%.1f",CH22TEMP);
				DISP_TEMP_S(300,308,(uint8_t*)buf,CH22_SW);
				sprintf(buf,"%.1f",CH23TEMP);
				DISP_TEMP_S(340,308,(uint8_t*)buf,CH23_SW);
				sprintf(buf,"%.1f",CH24TEMP);
				DISP_TEMP_S(380,308,(uint8_t*)buf,CH24_SW); 			
				sprintf(buf,"%.1f",CH25TEMP);
				DISP_TEMP_S(100,432,(uint8_t*)buf,CH25_SW);
				sprintf(buf,"%.1f",CH26TEMP);
				DISP_TEMP_S(140,432,(uint8_t*)buf,CH26_SW);
				sprintf(buf,"%.1f",CH27TEMP);
				DISP_TEMP_S(180,432,(uint8_t*)buf,CH27_SW);
				sprintf(buf,"%.1f",CH28TEMP);
				DISP_TEMP_S(220,432,(uint8_t*)buf,CH28_SW);
				sprintf(buf,"%.1f",CH29TEMP);
				DISP_TEMP_S(260,432,(uint8_t*)buf,CH29_SW);
				sprintf(buf,"%.1f",CH30TEMP);
				DISP_TEMP_S(300,432,(uint8_t*)buf,CH30_SW);
				sprintf(buf,"%.1f",CH31TEMP);
				DISP_TEMP_S(340,432,(uint8_t*)buf,CH31_SW);
				sprintf(buf,"%.1f",CH32TEMP);	
				DISP_TEMP_S(380,432,(uint8_t*)buf,CH32_SW);
				sprintf(buf,"%.1f",CH33TEMP);
				DISP_TEMP_S(100,556,(uint8_t*)buf,CH33_SW);
				sprintf(buf,"%.1f",CH34TEMP);
				DISP_TEMP_S(140,556,(uint8_t*)buf,CH34_SW);
				sprintf(buf,"%.1f",CH35TEMP);
				DISP_TEMP_S(180,556,(uint8_t*)buf,CH35_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_S(220,556,(uint8_t*)buf,CH36_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_S(260,556,(uint8_t*)buf,CH37_SW);
				sprintf(buf,"%.1f",CH38TEMP);
				DISP_TEMP_S(300,556,(uint8_t*)buf,CH38_SW);
				sprintf(buf,"%.1f",CH39TEMP);
				DISP_TEMP_S(340,556,(uint8_t*)buf,CH39_SW);
				sprintf(buf,"%.1f",CH40TEMP);
				DISP_TEMP_S(380,556,(uint8_t*)buf,CH40_SW);
		}
	}else if(page_flag == graph){
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
		sprintf(buf,"%.1f",CH1TEMP - COR1);
		DISP_CNL_S(40,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH2TEMP - COR2);
		DISP_CNL_S(60,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_MAGENTA,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH3TEMP - COR3);
		DISP_CNL_S(80,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_CYAN,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH4TEMP - COR4);
		DISP_CNL_S(100,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH5TEMP - COR5);
		DISP_CNL_S(120,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_HLT,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH6TEMP - COR6);
		DISP_CNL_S(140,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_BT,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH7TEMP - COR7);
		DISP_CNL_S(160,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH8TEMP - COR8);
		DISP_CNL_S(180,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH9TEMP - COR9);
		DISP_CNL_S(200,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR1,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH10TEMP - COR10);
		DISP_CNL_S(220,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR2,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH11TEMP - COR11);
		DISP_CNL_S(240,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR3,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH12TEMP - COR12);
		DISP_CNL_S(260,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR4,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH13TEMP - COR13);
		DISP_CNL_S(280,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR5,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH14TEMP - COR14);
		DISP_CNL_S(300,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR6,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH15TEMP - COR15);
		DISP_CNL_S(320,585,(uint8_t*)buf);
		LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH16TEMP - COR16);
		DISP_CNL_S(340,585,(uint8_t*)buf);
	}else if(page_flag == calibrate){
		if(cor_page == page1)
		{
			cal_disp(1,CH1TEMP - COR1);
			cal_disp(2,CH2TEMP - COR2);
			cal_disp(3,CH3TEMP - COR3);
			cal_disp(4,CH4TEMP - COR4);
			cal_disp(5,CH5TEMP - COR5);
			cal_disp(6,CH6TEMP - COR6);
			cal_disp(7,CH7TEMP - COR7);
			cal_disp(8,CH8TEMP - COR8);
		}else if(cor_page == page2){
			cal_disp(1,CH9TEMP - COR9);
			cal_disp(2,CH10TEMP - COR10);
			cal_disp(3,CH11TEMP - COR11);
			cal_disp(4,CH12TEMP - COR12);
			cal_disp(5,CH13TEMP - COR13);
			cal_disp(6,CH14TEMP - COR14);
			cal_disp(7,CH15TEMP - COR15);
			cal_disp(8,CH16TEMP - COR16);
		}else if(cor_page == page3){
			cal_disp(1,CH17TEMP - COR17);
			cal_disp(2,CH18TEMP - COR18);
			cal_disp(3,CH19TEMP - COR19);
			cal_disp(4,CH20TEMP - COR20);
			cal_disp(5,CH21TEMP - COR21);
			cal_disp(6,CH22TEMP - COR22);
			cal_disp(7,CH23TEMP - COR23);
			cal_disp(8,CH24TEMP - COR24);
		}else if(cor_page == page4){
			cal_disp(1,CH25TEMP - COR25);
			cal_disp(2,CH26TEMP - COR26);
			cal_disp(3,CH27TEMP - COR27);
			cal_disp(4,CH28TEMP - COR28);
			cal_disp(5,CH29TEMP - COR29);
			cal_disp(6,CH30TEMP - COR30);
			cal_disp(7,CH31TEMP - COR31);
			cal_disp(8,CH32TEMP - COR32);
		}
		else if(cor_page == page5){
			cal_disp(1,CH33TEMP - COR33);
			cal_disp(2,CH34TEMP - COR34);
			cal_disp(3,CH35TEMP - COR35);
			cal_disp(4,CH36TEMP - COR36);
			cal_disp(5,CH37TEMP - COR37);
			cal_disp(6,CH38TEMP - COR38);
			cal_disp(7,CH39TEMP - COR39);
			cal_disp(8,CH40TEMP - COR40);
		}
	}
	
	
}

void Save_flag(void)
{
	SPI_FLASH_SectorErase(0);
	SPI_FLASH_BufferWrite((void*)savedata,SPI_FLASH_PageSize*0, sizeof(savedata));
	SPI_FLASH_BufferWrite((void*)TempHLimits,SPI_FLASH_PageSize*1, sizeof(TempHLimits));
	SPI_FLASH_BufferWrite((void*)TempLLimits,SPI_FLASH_PageSize*2, sizeof(TempLLimits));
	SPI_FLASH_BufferWrite((void*)YLIMIT,SPI_FLASH_PageSize*3, sizeof(YLIMIT));
	SPI_FLASH_BufferWrite((void*)Correction,SPI_FLASH_PageSize*4, sizeof(Correction));
//	Save_Sflag();
}

void Read_flag(void)
{
	SPI_FLASH_BufferRead((void *)savedata,SPI_FLASH_PageSize*0, sizeof(savedata));
	SPI_FLASH_BufferRead((void *)TempHLimits,SPI_FLASH_PageSize*1, sizeof(TempHLimits));
	SPI_FLASH_BufferRead((void *)TempLLimits,SPI_FLASH_PageSize*2, sizeof(TempLLimits));
	SPI_FLASH_BufferRead((void *)YLIMIT,SPI_FLASH_PageSize*3, sizeof(YLIMIT));
	SPI_FLASH_BufferRead((void*)Correction,SPI_FLASH_PageSize*4, sizeof(Correction));
//	Read_Sflag();
	//	Read_history();
}


    /**************************************************************** 
     * Function:    Flash_EnableReadProtection 
     * Description: Enable the read protection of user flash area. 
     * Input: 
     * Output: 
     * Return:      1: Read Protection successfully enable 
     *              2: Error: Flash read unprotection failed 
    *****************************************************************/  
    uint32_t Flash_EnableReadProtection(void)  	
    {
      /* Returns the FLASH Read Protection level. */  
      if( FLASH_OB_GetRDP() == RESET )  
      {  
        /* Unlock the Option Bytes */  
        FLASH_OB_Unlock();  
        
        /* Sets the read protection level. */  
        FLASH_OB_RDPConfig(OB_RDP_Level_1);
          
        /* Start the Option Bytes programming process. */    
        if (FLASH_OB_Launch() != FLASH_COMPLETE)
        {  
          /* Disable the Flash option control register access (recommended to protect  
             the option Bytes against possible unwanted operations) */  
          FLASH_OB_Lock();  
            
          /* Error: Flash read unprotection failed */  
          return (2);  
        }  
        
        /* Disable the Flash option control register access (recommended to protect  
           the option Bytes against possible unwanted operations) */  
        FLASH_OB_Lock();  
      
        /* Read Protection successfully enable */  
        return (1);  
      }  
        
      /* Read Protection successfully enable */  
      return (1);  
    }  
    
    /**************************************************************** 
     * Function:    Flash_DisableReadProtection 
     * Description: Disable the read protection of user flash area. 
     * Input: 
     * Output: 
     * Return:      1: Read Protection successfully disable 
     *              2: Error: Flash read unprotection failed 
    *****************************************************************/  
    uint32_t Flash_DisableReadProtection(void)  
    {  
      /* Returns the FLASH Read Protection level. */  
      if( FLASH_OB_GetRDP() != RESET )  
      {  
        /* Unlock the Option Bytes */  
        FLASH_OB_Unlock();  
          
        /* Sets the read protection level. */  
        FLASH_OB_RDPConfig(OB_RDP_Level_0);  
          
        /* Start the Option Bytes programming process. */    
        if (FLASH_OB_Launch() != FLASH_COMPLETE)  
        {  
          /* Disable the Flash option control register access (recommended to protect  
             the option Bytes against possible unwanted operations) */  
          FLASH_OB_Lock();  
            
          /* Error: Flash read unprotection failed */  
          return (2);  
        }  
        
        /* Disable the Flash option control register access (recommended to protect  
           the option Bytes against possible unwanted operations) */  
        FLASH_OB_Lock();  
      
        /* Read Protection successfully disable */  
        return (1);  
      }  
        
      /* Read Protection successfully disable */  
      return (1);  
    }  

void UsbDataHandle(void)
{
	u8 i;
	u8 j;
	uint16_t sendcrc;
	u8 creclen;
	u8 csendlen;

		u16 voltage;//电压
	u16 current;
	u32 power;  //功率
	u16 frequancy;
	u16 PF;//功率因数
	
	if(usbbuf[0] == 0x01)
	{
		if(usbbuf[1] == 0x03)//读数据
		{
			free(crec);
			free(csend);
			if(usbbuf[2] == 0 && usbbuf[3] == 0)
			{
				readcrc = usbbuf[4] << 8|usbbuf[5];
				creclen = 4;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
				for(i = 0;i < 4;i++)
				{
					crec[i] = usbbuf[i];
				}
			}else{
				readcrc = usbbuf[6] << 8|usbbuf[7];
				creclen = 6;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
				for(i = 0;i < 6;i++)
				{
					crec[i] = usbbuf[i];
				}				
			}				
			crcwatch = CRC16(crec,creclen);
			if(CRC16(crec,creclen) == readcrc)//CRC校验
			{
				
				if(usbbuf[2] == 0 && usbbuf[3] == 0)//读实时数据
				{
					csendlen = 38;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x10;
//					usbsendbuf[6] = usbbuf[5]*2;
					
//					if(usbsendbuf[5]<= 16)
//					{
						for(i = 0; i < usbsendbuf[5]; i++)
						{
							if((((u16)RecBuff[5+usbbuf[3]+i*2] << 8) + RecBuff[6+usbbuf[3]+i*2] - (int)(Correction[usbbuf[3] + i] * 10)) == 4E1F)
							{
								usbsendbuf[6+i*2] = 0xE0;
								usbsendbuf[7+i*2] = 0xE0;
							}else{
								usbsendbuf[6+i*2] = (u8)((((u16)RecBuff[5+usbbuf[3]+i*2] << 8) + RecBuff[6+usbbuf[3]+i*2] - (int)(Correction[usbbuf[3] + i] * 10)) >> 8);
								usbsendbuf[7+i*2] = (u8)(((u16)RecBuff[5+usbbuf[3]+i*2] << 8) + RecBuff[6+usbbuf[3]+i*2] - (int)(Correction[usbbuf[3] + i] * 10));
							}
	//						usbsendbuf[7+i*2] = 0;
	//						usbsendbuf[8+i*2] = 0;
						}
						for(i = 0;i < csendlen; i++)
						{
							csend[i] = usbsendbuf[i];
						}
						sendcrc = CRC16(csend,csendlen);
						usbsendbuf[6+(usbsendbuf[5])*2] = (u8)(sendcrc >> 8);
						usbsendbuf[7+(usbsendbuf[5])*2] = (u8)(sendcrc);
//						for(i = 8 + (usbsendbuf[5])*2; i < 64 ; i++)
//						{
//							usbsendbuf[i] = 0;
//						}
						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显						
//					}
//					else{
//						for(i = 0; i < usbbuf[5]; i++)
//						{
//							usbsendbuf[7+i*2] = 0xE0;
//							usbsendbuf[8+i*2] = 0xE0;
//	//						usbsendbuf[7+i*2] = 0;
//	//						usbsendbuf[8+i*2] = 0;
//						}
//						for(i = 0;i < csendlen; i++)
//						{
//							csend[i] = usbsendbuf[i];
//						}
//						sendcrc = CRC16(csend,csendlen);
//						usbsendbuf[7+(usbbuf[5])*2] = (u8)(sendcrc >> 8);
//						usbsendbuf[8+(usbbuf[5])*2] = (u8)(sendcrc);
//						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
//					}
					
//					for(i = 0;i < 8;i++)
//					{
//						csendlen = 23;				
//						csend = (u8*)malloc(sizeof(u8) * csendlen);				
//						memset(csend, 0, csendlen);//初始化，每个元素都为零
//						usbsendbuf[0] = 0x01;
//						usbsendbuf[1] = 0x03;
//						usbsendbuf[2] = 0x00;
//						usbsendbuf[3] = 16*(i+1);
//						usbsendbuf[4] = 0x00;
//						usbsendbuf[5] = 16*(i+1)+16;
//						usbsendbuf[6] = 32;
//						for(j=0;j<16;j++)
//						{
//							usbsendbuf[7+j] = 0xee;
//							
//						}
//						for(j = 0;j < csendlen; j++)
//						{
//							csend[j] = usbsendbuf[j];
//						}
//						sendcrc = CRC16(csend,csendlen);
//						usbsendbuf[23] = (u8)(sendcrc >> 8);
//						usbsendbuf[24] = (u8)(sendcrc);
//						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
//					}
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x00){//读取时间
					
					csendlen = 15;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);		
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度

//					usbreadtime = 1;
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x08;
					usbsendbuf[7] = usbreadtime[0];
					usbsendbuf[8] = usbreadtime[1];
					usbsendbuf[9] = usbreadtime[2];
					usbsendbuf[10] = usbreadtime[3];
					usbsendbuf[11] = usbreadtime[4];
					usbsendbuf[12] = usbreadtime[5];
					usbsendbuf[13] = usbreadtime[6];
					usbsendbuf[14] = 0;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[15] = (u8)(sendcrc >> 8);
					usbsendbuf[16] = (u8)(sendcrc);
					for(i = 17; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					
					Delay(200);
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x20){//读取传感器类型
					
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x02;
					usbsendbuf[7] = 0x00;
					usbsendbuf[8] = TCTYPE;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[9] = (u8)(sendcrc >> 8);
					usbsendbuf[10] = (u8)(sendcrc);
					for(i = 11; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x10){//读取仪器状态
					
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x02;
					usbsendbuf[7] = 0x00;
					usbsendbuf[8] = eqmtstatus;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[9] = (u8)(sendcrc >> 8);
					usbsendbuf[10] = (u8)(sendcrc);
					for(i = 11; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}else if(usbbuf[2] == 0x01 && usbbuf[3] == 0x01){//读取上下限
					
					csendlen = 11;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x04;
					usbsendbuf[7] = (u8)((int)(TempHLimits[(usbbuf[3]-1)/2]) >> 8);
					usbsendbuf[8] = (u8)((int)(TempHLimits[(usbbuf[3]-1)/2]));
					usbsendbuf[9] = (u8)((int)(TempLLimits[(usbbuf[3]-1)/2]) >> 8);
					usbsendbuf[10] = (u8)((int)(TempLLimits[(usbbuf[3]-1)/2]));
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[11] = (u8)(sendcrc >> 8);
					usbsendbuf[12] = (u8)(sendcrc);
					for(i = 13; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}else if(usbbuf[2] == 0x02 && usbbuf[3] == 0x58){//读取单位
					free(csend);
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x02;
					usbsendbuf[7] = 0x00;
					usbsendbuf[8] = UNIT;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[9] = (u8)(sendcrc >> 8);
					usbsendbuf[10] = (u8)(sendcrc);
					for(i = 11; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}else if(usbbuf[2] == 0xEE && usbbuf[3] == 0xEE){//检测连接状态
					
					csendlen = 15;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x4A;
					usbsendbuf[7] = 0x4E;
					usbsendbuf[8] = 0x4B;
					usbsendbuf[9] = 0x4F;
					usbsendbuf[10] = 0x35;
					usbsendbuf[11] = 0x30;
					usbsendbuf[12] = 0x38;
					usbsendbuf[13] = 0x7C;
					usbsendbuf[14] = 0xA3;

					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[15] = (u8)(sendcrc >> 8);
					usbsendbuf[16] = (u8)(sendcrc);
					for(i = 17; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
					
				}else if(usbbuf[2] == 0x80 && usbbuf[3] == 0x10){//读取电参数
					
					csendlen = 19;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					//发送数据CRC校验长度
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x0C;
//					usbsendbuf[7] = (u8)(voltage >> 8);
//					usbsendbuf[8] = (u8)voltage;
//					usbsendbuf[9] = (u8)(current >> 8);
//					usbsendbuf[10] = (u8)current;
//					usbsendbuf[11] = (u8)(power >> 24);
//					usbsendbuf[12] = (u8)(power >> 16);
//					usbsendbuf[13] = (u8)(power >> 8);
//					usbsendbuf[14] = (u8)power;
//					usbsendbuf[15] = (u8)(frequancy >> 8);
//					usbsendbuf[16] = (u8)frequancy;
//					usbsendbuf[17] = (u8)(PF >> 8);
//					usbsendbuf[18] = (u8)PF;
					usbsendbuf[7] = 0xE0;
					usbsendbuf[8] = 0xE0;
					usbsendbuf[9] = 0xE0;
					usbsendbuf[10] = 0xE0;
					usbsendbuf[11] = 0xE0;
					usbsendbuf[12] = 0xE0;
					usbsendbuf[13] = 0xE0;
					usbsendbuf[14] = 0xE0;
					usbsendbuf[15] = 0xE0;
					usbsendbuf[16] = 0xE0;
					usbsendbuf[17] = 0xE0;
					usbsendbuf[18] = 0xE0;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[19] = (u8)(sendcrc >> 8);
					usbsendbuf[20] = (u8)(sendcrc);
					
					for(i = 21; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}
			}
			
		}else if(usbbuf[1] == 0x10){
			free(crec);
			free(csend);
			if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x00)//设置时间
			{
				readcrc = usbbuf[15] << 8|usbbuf[16];
				creclen = 15;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = usbbuf[2];
//				crec[3] = usbbuf[3];
//				crec[4] = usbbuf[4];
//				crec[5] = usbbuf[5];
//				crec[6] = usbbuf[6];
//				crec[7] = usbbuf[7];
//				crec[8] = usbbuf[8];
//				crec[9] = usbbuf[9];
//				crec[10] = usbbuf[10];
//				crec[11] = usbbuf[11];
//				crec[12] = usbbuf[12];
//				crec[13] = usbbuf[13];
//				crec[14] = usbbuf[14];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = usbbuf[i];
				}
				
				crcwatch = CRC16(crec,creclen);
				if(CRC16(crec,creclen) == readcrc)
				{
					csendlen = 6;
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x10;
					usbsendbuf[2] = 0xC0;
					usbsendbuf[3] = 0x00;
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x00;
					
					RTC_DateTypeDef RTC_DateStructure;
					RTC_TimeTypeDef RTC_TimeStructure;
					
					YEAR = usbbuf[8];
					MONTH = usbbuf[9];
					DATE = usbbuf[10];
					HOURS =usbbuf[11];
					MINUTES = usbbuf[12];
					SECONDS = usbbuf[13];										
					
					RTC_DateStructure.RTC_Date = DATE;
					RTC_DateStructure.RTC_Month = MONTH;         
					RTC_DateStructure.RTC_Year = YEAR;					
					RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
					RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
					
					RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
					RTC_TimeStructure.RTC_Hours = HOURS;        
					RTC_TimeStructure.RTC_Minutes = MINUTES;      
					RTC_TimeStructure.RTC_Seconds = SECONDS;      
					RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
					RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[6] = (u8)(sendcrc >> 8);
					usbsendbuf[7] = (u8)(sendcrc);
					
					for(i = 8; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}
			}else if(usbbuf[2] == 0x01){//设置上下限
				readcrc = usbbuf[11] << 8|usbbuf[12];
				creclen = 11;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = usbbuf[2];
//				crec[3] = usbbuf[3];
//				crec[4] = usbbuf[4];
//				crec[5] = usbbuf[5];
//				crec[6] = usbbuf[6];
//				crec[7] = usbbuf[7];
//				crec[8] = usbbuf[8];
//				crec[9] = usbbuf[9];
//				crec[10] = usbbuf[10];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = usbbuf[i];
				}
				crcwatch = CRC16(crec,creclen);
				if(CRC16(crec,creclen) == readcrc)
				{
					csendlen = 6;
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x10;
					usbsendbuf[2] = 0x01;
					usbsendbuf[3] = 0x01;
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x02;
					
					TempHLimits[(usbbuf[3]-1)/2] = (float)(usbbuf[7] << 8 | usbbuf[8]);
					TempLLimits[(usbbuf[3]-1)/2] = (float)(usbbuf[9] << 8 | usbbuf[10]);
					Save_flag();
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[6] = (u8)(sendcrc >> 8);
					usbsendbuf[7] = (u8)(sendcrc);
					
					for(i = 8; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}
			}else if(usbbuf[2] == 0x02 && usbbuf[3] == 0x58){//设置单位
				readcrc = usbbuf[9] << 8|usbbuf[10];
				creclen = 9;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//初始化，每个元素都为零
//				crec[0] = 0x01;
//				crec[1] = 0x10;
//				crec[2] = usbbuf[2];
//				crec[3] = usbbuf[3];
//				crec[4] = usbbuf[4];
//				crec[5] = usbbuf[5];
//				crec[6] = usbbuf[6];
//				crec[7] = usbbuf[7];
//				crec[8] = usbbuf[8];
				for(i = 0; i < creclen;i ++)
				{
					crec[i] = usbbuf[i];
				}
				
				crcwatch = CRC16(crec,creclen);
				if(CRC16(crec,creclen) == readcrc)
				{
					csendlen = 6;
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//初始化，每个元素都为零
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x10;
					usbsendbuf[2] = 0x02;
					usbsendbuf[3] = 0x58;
					usbsendbuf[4] = 0x00;
					usbsendbuf[5] = 0x01;
					
					UNIT = usbbuf[8];
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[6] = (u8)(sendcrc >> 8);
					usbsendbuf[7] = (u8)(sendcrc);
					
					for(i = 8; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//数据回显
				}
			}
		}
	}
}

//U盘检测
u8 udisk_scan(void)
{
	static u8 res;
	
//	u8 i;
//	char str[64];
	if(usbstatus != UNCONNECTED)
	{
		res = CH376DiskConnect( );
		if( res != USB_INT_SUCCESS )/* 检查U盘是否连接,等待U盘插入,对于SD卡,可以由单片机直接查询SD卡座的插拔状态引脚 */
		{  
			DrawUdisk1();
			usbstatus = UNCONNECTED;
			return NO_CONNECTION;
		}
	}
//	Delay(200);
	if(usbstatus != CONNECTED)
	{		
		res = CH376DiskMount( );
		if(res == USB_INT_SUCCESS)
		{
			DrawUdisk2();
			usbstatus = CONNECTED;
			return UDISK_READY;
		}

	}
	return UDISK_NOTREADY;
}

//上下限监测
void Check_limits(u8 chn)
{
	if((ch_temp[chn-1] - Correction[chn-1]  < TempLLimits[chn-1] || ch_temp[chn-1] - Correction[chn-1] > TempHLimits[chn-1]) && FILTER == ft_on)
	{
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
	}else{
		LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
	}
}

//BCD转换
int hex_to_bcd(int data)
{
    unsigned char temp;
	unsigned int bcd_data;
	
	temp = data%100;
	bcd_data = ((unsigned int)data)/100<<8;
	bcd_data = bcd_data|temp/10<<4;
	bcd_data = bcd_data|temp%10;

//    temp = (((data/10)<<4) + (data%10));
    return bcd_data;
	
}

//flash全部擦除
void Erase_all(void)
{
	static u16 serec = 2;
	static u8 Check[4096];
	SPI_FLASH_SectorErase(serec*4096);
	Delay(500);
//	SPI_FLASH_BufferRead((void *)Check,serec*4096, sizeof(Check));
	serec ++;
}
/*********************************************END OF FILE**********************/

