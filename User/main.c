/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDCҺ����ʾ����(��ʾ�����С)
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
#include "flash_if.h"
#include "ff.h"
#include <string.h>
#include "sdio/bsp_sdio_sd.h"

/** @defgroup APP_HID_Private_Variables
  * @{
  */ 


void TempDisplay(void);
void TempDisHandle(u8 ch);
void UARTRECHANDLE(void);
void BEEPCHECK(void);
u8 usbbuf[0x40];
u8 usbsendbuf[0x40];
u8 savedata[80];
u16 usavetime[2];
u8 uinfo[64];
u8 trigtime[4];
u8 chcomp[40];
//u8 usbreadtime = 0;
u8 eqmtstatus;
u16 crcwatch;
uint16_t readcrc;
u8 *crec;
u8 *csend;
u16 datasize;
u8 usbstatus = UNKNOWN;
u16 watch;
u16 ureadcrc;
u8 *ucrc;
extern u8 uartflag;
u8 recordflag;
u8 sdstatus;
u16 ucount;
//SD_CardInfo SDINFO;
//u8 p1,p2,p3,p4,p5,p6,p7,p8;
//char filename[20];
//char foldername[20];
//FILINFO hisinfo;
//char dirname[10][13];
//DIR dir;
//FATFS fs;													/* FatFs�ļ�ϵͳ���� */
//FIL fnew;													/* �ļ����� */
//FRESULT res_sd;                /* �ļ�������� */
//UINT fnum;            					  /* �ļ��ɹ���д���� */
//BYTE ReadBuffer[1024]={0};        /* �������� */
//BYTE WriteBuffer[] =              /* д������*/
//"��ӭʹ�ý���ֳ��¶��� �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";  


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
       unsigned char  TC:4;    //�ȵ�ż����
       unsigned char  FT:1;    //��ѡ/�Ƚ�
       unsigned char  BP:1;    //Ѷ��
       unsigned char  UT:2;	   //��λ
   }BIT_FLAG;
}FLAG6;

union 
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

union 
{
   unsigned char FLAG_VAL8;
	
   struct 
   {
       unsigned char  BR:3;    //����
       unsigned char  SP:3;    //�ٶ�
       
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

//��ȡ��ID�洢λ��
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;


void Delay(__IO u32 nCount); 
void LCD_Test(void);
void Printf_Charater(void);
void AVGCAL(void);
int i;
u16 count = 0;

float ch_temp[40];
float avg_data[3];

void JumpBoot(u8 flag)
{
  	void (*pUserApp)(void);
  uint32_t JumpAddress;
	if(flag==55)
  {		
	__asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(BASIC_TIM, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_Cmd(TIM2,DISABLE);
		TIM_DeInit(BASIC_TIM);
		TIM_ITConfig(BASIC_TIM,TIM_IT_Update,DISABLE);
		TIM_Cmd(BASIC_TIM, DISABLE);	
		USART_DeInit(DEBUG_USART);
		USART_ITConfig(DEBUG_USART, USART_IT_RXNE, DISABLE);		
		USART_Cmd(DEBUG_USART,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//�ر�WAKE UP ��ʱ���ж�
		RTC_WakeUpCmd( DISABLE);//�ر�WAKE UP ��ʱ����
//		Disable_Extiint();
//		USBH_DeInit(&USB_OTG_Core,&USB_Host);
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
	}
}


/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */

int main(void)

{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00000);
	static u8 powerstat;
	uint8_t  buf[0x40];
//	static u8 ledstat;
	u8 test[9] = {0X01,0X03,0X02,0X58,0X00,0X01,0X02,0X00,0X05};
	 __IO uint32_t i = 0;
	static u8 reqtempcount;
//	static u16 ucount;
	static u8 urecount;
	static u16 usavecount;
	static u16 sendcount;
	static u8 touched;
//	u8 res;
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32fxxx_xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32fxxx.c file
  */  
		/*��ʼ��SDRAMģ��*/
	SDRAM_Init();
	 /*��ʼ��Һ����*/ 
    LCD_Init();

    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_BLACK);
	
	
	
	SysTick_Init();
	Delay(500);
	/* LED �˿ڳ�ʼ�� */
//	LED_GPIO_Config();	 
	
	/* 16M����flash W25Q128��ʼ�� */
	SPI_FLASH_Init();
	
		/*���ڳ�ʼ��*/
	Debug_USART_Config();
	 
	/* ��ȡ Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();
	Touch_GPIO_Config();
	Delay(200);
	  
	
	/*������ʼ��*/
	Key_GPIO_Config();
//	/* ��ʼ��ϵͳ�δ�ʱ�� */
	

	/*�������˿ڳ�ʼ�� */
	Beep_GPIO_Config();
	//����оƬ������
//	Flash_EnableReadProtection();
	
	//	  /* ��ʼ��ͨ�ö�ʱ����ʱ��20m+s����һ���ж� */
	TIMx_Configuration();
	
	/*CH376�������ó�ʼ��*/
	CH376_GPIO_Init();
    
	
//	Delay_ms(100);
	
	Init_CH376();
	

	
	/* RTC���ã�ѡ��ʱ��Դ������RTC_CLK�ķ�Ƶϵ�� */
	RTC_CLK_Config();
	
	
	if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		/* ����ʱ������� */
		RTC_TimeAndDate_Set();
	}
	else
	{
	/* ����Ƿ��Դ��λ */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		 printf("\r\n ������Դ��λ....\r\n");
	}
	/* ����Ƿ��ⲿ��λ */
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		 printf("\r\n �����ⲿ��λ....\r\n");
	}

	printf("\r\n ����Ҫ��������RTC....\r\n");
		
	/* ʹ�� PWR ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
	PWR_BackupAccessCmd(ENABLE);
	/* �ȴ� RTC APB �Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	}
	
	USBD_Init(&USB_OTG_dev,USB_OTG_HS_CORE_ID,
           &USR_desc,&USBD_HID_cb,&USR_cb);
		/* ��ȡ SPI Flash ID */
	
	
	FlashID = SPI_FLASH_ReadID();
	if (FlashID == sFLASH_ID) 
	{
//		Delay(500);
		Read_flag();
		Read_Sflag();
		Read_Flash_Init_Handle();
	}
	power_on();
//	watch = CRC16(test,9);
//	page_home();
//	watch = sizeof(TempHLimits);
	Touch_GPIO_Config();
	tp_dev.init();
	if(SD_Init() == SD_OK)//��ʼ��SD��
	{
		sdstatus = 1;
//		DrawSD2();
		Read_Block_Rec();
		Read_Index(BlockNum.Num[1]/40);
	}else{
//		DrawSD1();
	}
//	SD_GetCardInfo(&SDINFO);
	trigflag = 1;
//	BlockNum.Num[0] = 0;
//	BlockNum.Num[1] = 0;
	while(1)
	{

//		LED1_ON;
//		ledstat = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
//		watxiansch = GPIO_ReadInputDataBit(TOUCH_YPLUS_GPIO_PORT,TOUCH_YPLUS_GPIO_PIN);
		/* ��ʾʱ������� */	
		
		powerstat = GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_11);
		if(powerstat == 0 && page_flag != poweron && page_flag != poweroff)
		{
			PowerOffHandle();
		}else if(page_flag != factory){
			RTC_TimeAndDate_Show();
			/*����ɨ��*/		
			Key_Function();
			tp_dev.scan(0);
			if(tp_dev.sta == 0xc0)
			{
				
			}else{
				touched = 0;
			}
			if(tp_dev.sta&TP_PRES_DOWN && touched == 0)			//������������
			{
//				BEEP_ON;
				TouchHandle(tp_dev.x[0],tp_dev.y[0]);
				touched = 1;
			}
			
//			if(TOUCH == op_on)
//			{
//				if(touchflag == 1)
//				{
////					Delay(1000);
					
//				}
//			}
			if(page_flag != poweron && powerstat != 0)
			{
				if(usavecount == 100)
				{
					udisk_scan();
					usavecount = 0;
				}else{
					usavecount++;
				}
			}
//			USART_ITConfig(DEBUG_USART, USART_IT_RXNE, DISABLE);

	//		Touch_Scan();
	//		CH1TEMP = (RecBuff[21] * 256 + RecBuff[22])/10.0;
			DrawBattery(battery);
//			if(charge == 0x80){
//				DrawCharge();
//			}else{
//				DispBattery();
//			}
			if(uartflag == 1)
			{
				if(SPEED == fast)
				{
					UARTRECHANDLE();
					urecount = 0;
				}else if(SPEED == middle){
					if(urecount == 5)
					{
						UARTRECHANDLE();
						urecount = 0;
					}
				}else{
					if(urecount == 10)
					{
						UARTRECHANDLE();
						urecount = 0;
					}
				}
				if(usbstatus == CONNECTED && page_flag != hisgraph)
				{
					if(ucount == usavetime[0])
					{
						Utest();
						ucount = 0;
					}
//					else{
//						ucount ++;
//					}
				}else{
					ucount = 0;
					fileflag = 0;
				}
				uartflag = 0;
				urecount++;
			}
			TempDisplay();
			
			
			DCD_EP_PrepareRx(&USB_OTG_dev,HID_OUT_EP,usbbuf,64);//����PC����
			if(UsbHidReceiveComplete)                         //���յ�����
			{
				UsbDataHandle();
				UsbHidReceiveComplete=0;
			}
		}else{
			Key_Function();
		}
		

	
	}
}



////sd������
//void sdtest(void)
//{
//	static u8 i;
//	static char nametime[10];
//	static char readname[100];
//	memset(filename,0,100);
////	  printf("\r\n****** ����һ��SD�� �ļ�ϵͳʵ�� ******\r\n");
//  
//	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
//	res_sd = f_mount(&fs,"0:",1);
//	
///*----------------------- ��ʽ������ ---------------------------*/  
//	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
//	if(res_sd == FR_NO_FILESYSTEM)
//	{
//		LCD_DisplayStringLine(10,10,"NO FILE SYSTEM");
//    /* ��ʽ�� */
//		res_sd=f_mkfs("0:",0,0);							
//		
//		if(res_sd == FR_OK)
//		{
//			LCD_DisplayStringLine(40,10,"SD CARD ERASED");
////			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
//      /* ��ʽ������ȡ������ */
//			res_sd = f_mount(NULL,"0:",1);			
//      /* ���¹���	*/			
//			res_sd = f_mount(&fs,"0:",1);
//		}
//		else
//		{
//			LCD_DisplayStringLine(40,10,"ERASE FAILED");
////			printf("������ʽ��ʧ�ܡ�����\r\n");
////			while(1);
//		}
//	}
//  else if(res_sd!=FR_OK)
//  {
//	  LCD_DisplayStringLine(70,10,"FILE SYSTEM FAILED");
////    printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
////    printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
////		while(1);
//  }
//  else
//  {
//	  LCD_DisplayStringLine(70,10,"FILE SYSTEM OK");
//    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
//  }
//  
///*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
//	/* ���ļ�������ļ��������򴴽��� */
////	printf("\r\n****** ���������ļ�д�����... ******\r\n");
//    memcpy ((void *)filename,"0:",2);
//    sprintf(nametime,"%02d%02d%02d%d", 
//			usbreadtime[1],
//			usbreadtime[2],
//			usbreadtime[3],
//			i);
//	strcat((char *)filename,(char *)nametime);
//	strcat((char *)filename,(char *)".xls");
////	sprintf(filename,"0:20%0.2d/%0.2d/%0.2d.txt", 
////			usbreadtime[1],
////			usbreadtime[2],
////			usbreadtime[3]);
////	filename = "0:20190909.txt";
//	res_sd = f_open(&fnew, (char *)filename,FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_sd == FR_OK )
//	{
////		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
//    /* ��ָ���洢������д�뵽�ļ��� */
//		LCD_DisplayStringLine(100,10,"OPEN OK,WRITE DATA");
//		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
//    if(res_sd==FR_OK)
//    {
//		LCD_DisplayStringLine(130,10,"WRITE OK");
////      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
////      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
//    }
//    else
//    {
////      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
//		LCD_DisplayStringLine(130,10,"WRITE NG");
//    }    
//		/* ���ٶ�д���ر��ļ� */
//    f_close(&fnew);
//	}
//	else
//	{	
//		LCD_DisplayStringLine(100,10,"OPEN FAILED");
////		printf("������/�����ļ�ʧ�ܡ�\r\n");
//	}
//	
///*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
////	printf("****** ���������ļ���ȡ����... ******\r\n");
////	res_sd = f_open(&fnew, (char *)filename, FA_OPEN_EXISTING | FA_READ); 	 
////	if(res_sd == FR_OK)
////	{

////		printf("�����ļ��ɹ���\r\n");
////		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
////    if(res_sd==FR_OK)
////    {
////      printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
////      printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
////    }
////    else
////    {
////      printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
////    }		
////	}
////	else
////	{
////	
////		printf("�������ļ�ʧ�ܡ�\r\n");
////	}
////	/* ���ٶ�д���ر��ļ� */
////	f_close(&fnew);	
////  
////	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
////	f_mount(NULL,"0:",1);
//	res_sd = f_opendir(&dir, "");
//	do
//	{
//		
//		res_sd = f_readdir(&dir,&hisinfo);
//		if(res_sd==FR_OK)
//		{
////			strncpy(readname,hisinfo.fname,13); 
//			
//			LCD_DisplayStringLine(160+i*30,10,(uint8_t *)hisinfo.fname);
//		}
//		
//	}while(hisinfo.fname[0] != 0);
//	i++;
//	
//}



//���ڽ��մ���
void UARTRECHANDLE(void)
{
	static uint8_t ucTemp;
	static u8 Total_Len = 0;
	static u8 uinitflag = 0;
	static u8 multicount = 0;
//	static float graphbuf[16];
	static u32 graphbuf[16][2];
	static u32 hisbuf[16][2];
	static u16 hisconv;
	static u16 corconv;
	u8 i;
	char buf[10];
	static int16_t tempbuf;
	static u8 usave;
	u8 ucrclen;
	
	free(ucrc);
	ureadcrc = RecBuff[38] << 8|RecBuff[37];
	ucrclen = 37;
	ucrc = (u8*)malloc(sizeof(u8) * ucrclen);
	for(i=0;i<ucrclen;i++)
	{
		ucrc[i] = RecBuff[i];
	}
	if(CRC16(ucrc,ucrclen) == ureadcrc)
	{
		charge = RecBuff[3];
		battery = RecBuff[4];
		
		for(i=0;i<16;i++)
		{
			tempbuf = RecBuff[2*(i+1)+3]<<8;
			tempbuf = tempbuf + RecBuff[2*(i+1)+4];
			if(tempbuf < 0)
			{
				ch_temp[i] = (float)tempbuf/10;
			}else{
				ch_temp[i] = (float)tempbuf/10;
			}
//						ch_temp[i] = (RecBuff[2*(i+1)+3] * 256 + RecBuff[2*(i+1)+4])/10.0;
			if(count == 0 && page_flag == poweron)
			{
				if(i==0)
				{
					LCD_Clear(LCD_COLOR_BLACK);
				}
				InitBrt();//��������
				if(i < CHNUM)
				{
					LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
					DISP_INS(5+i*20,5,"Initializing Channel");
					sprintf(buf,"%03d",i+1);
					DISP_INS(5+i*20,336,(uint8_t*)buf);
					DISP_INS(5+i*20,384,"...");
					Delay(0x3fffff);
				}
			}
		}
		
		
		if(multicount == 1 && page_flag == poweron)
		{
			LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
			if(CHNUM == 16)
			{
				DISP_INS(325,5,"Done!");
			}else if(CHNUM == 8){
				DISP_INS(165,5,"Done!");
			}
			Delay(0xffffff);
			page_home();
			multicount = 0;
		}
		
		if(multicount%(int)MULTI == 0 && multicount != 0 && page_flag != poweron)
		{
			for(i=0;i<16;i++)
			{
				G_Data[i][count] = ch_temp[i] - Correction[i];
				if(recordflag == 1)
				{
					SaveBuffer.Temp[i][count] = G_Data[i][count];
				}
			}
			if(page_flag == graph)
			{
				Draw_graph();
				DrawTime();
			}
			if(recordflag == 1)
			{
				SaveBuffer.Time[count][0]=20;
				SaveBuffer.Time[count][1] = usbreadtime[1];
				SaveBuffer.Time[count][2] = usbreadtime[2];
				SaveBuffer.Time[count][3] = usbreadtime[3];
				SaveBuffer.Time[count][4] = usbreadtime[4];
				SaveBuffer.Time[count][5] = usbreadtime[5];
				SaveBuffer.Time[count][6] = usbreadtime[6];
			}
//						if(page_flag != history)
//						{
//			for(i=0;i<16;i++)
//			{
////								savebuf = hex_to_bcd((int)(graphbuf[i]/MULTI * 10));
//				hisconv = (u16)(hisbuf[i][0]/MULTI)<<8;
//				hisconv = hisconv + hisbuf[i][1]/MULTI;
//				corconv = (u16)(Correction[i]*10);
////								Data_buf[i][count%8 * 2] = hisbuf[i][0]/MULTI;
////								Data_buf[i][count%8 * 2 + 1] = hisbuf[i][1]/MULTI;
//				Data_buf[i][count%8 * 2] = (u8)((hisconv - corconv)>>8);
//				Data_buf[i][count%8 * 2 + 1] = (u8)(hisconv - corconv);
//			}
////							Save_history(1);
////			if(count > 0 && (count + 1) % 8 == 0)
////			{
//////								recflag = 1;
////				if(SECTOR_REC < 62000)
////				{
////					SECTOR_REC ++;
////					Save_history(SECTOR_REC);								
////					Save_Sflag();									
////				}else{
////					SECTOR_REC = 0;
////				}
////			}
////						}
//			if(count == 450)
//			{
////				if(TIME_REC < 1000)
////				{
////					TIME_REC++;
////					Save_time(TIME_REC);
////					Save_Sflag();
////				}else{
////					TIME_REC = 0;
////				}
//			}
			if(count > 494)
			{
				count = 0;
				if(recordflag == 1)
				{
					Write_His_Data();
				}
//							memcpy(hisdata,G_Data,sizeof(G_Data));
//							memcpy(histime,time_buf,sizeof(time_buf));
			}else{
				count++;
			}
			multicount=0;
			
			
			for(i = 0;i<16;i++)
			{
				hisbuf[i][0] = 0;
				hisbuf[i][1] = 0;
//							graphbuf[i] = 0;
				graphbuf[i][0] = 0;
				graphbuf[i][1] = 0;
			}
			for(i=0;i<16;i++)
			{
				hisbuf[i][0] += RecBuff[2*(i+1)+3];
				hisbuf[i][1] += RecBuff[2*(i+1)+4];
//							graphbuf[i] += ch_temp[i];
				graphbuf[i][0] += RecBuff[2*(i+1)+3];
				graphbuf[i][1] += RecBuff[2*(i+1)+4];
			}
			multicount++;
		}else{
			if(page_flag != poweron)
			{
				for(i=0;i<16;i++)
				{
					hisbuf[i][0] += RecBuff[2*(i+1)+3];
					hisbuf[i][1] += RecBuff[2*(i+1)+4];
	//							graphbuf[i] += ch_temp[i];
					graphbuf[i][0] += RecBuff[2*(i+1)+3];
					graphbuf[i][1] += RecBuff[2*(i+1)+4];
				}
				
			}
			multicount++;
		}

	}
}


/*GERNERAL CODES*/
void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

uint16_t CRC16(uint8_t *puchMsg, uint8_t Len)
{
	uint8_t t, m,n,p;
	uint8_t uchCRCHi=0xFF; /* ��CRC�ֽڳ�ʼ��*/ 
	uint8_t uchCRCLo =0xFF; /* ��CRC �ֽڳ�ʼ��*/ 
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
//��������
void InitBrt(void)
{
	if(BRTS == L0)
	{
		brightness = 5;
	}else if(BRTS == L1){
		brightness = 15;
	}else if(BRTS == L2){
		brightness = 30;
	}else if(BRTS == L3){
		brightness = 45;
	}else if(BRTS == L1){
		brightness = 60;
	}
	TIM_PWMOUTPUT_Config(brightness);
}
//�¶���ʾ
void TempDisplay(void)
{
	char buf[10];
	static u8 eqmtstatus,dataupdate;
//	u8 i;
	if((page_flag == display || page_flag == graph) && recordflag == 1)
	{
		if(eqmtstatus < 60)
		{
			if(LANG == chs)
			{
				LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
				LCD_DisplayStringLine(2,180,"SD");
				LCD_DisplayStringLine(5,180+32,"����¼��");
			}else if(LANG == eng){
				LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
				DISP_INS(10,130,"SD card recording");
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
	}else if(recordflag == 0 && page_flag == display){
		if(LANG == chs)
		{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(180,5,130,32);
		}else if(LANG == eng){
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(130,10,235,25);
		}
	}
	if(page_flag == display)
	{		
//		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		sprintf(buf,"%03d",charge);
//		LCD_DisplayStringLine_48(47,200,(uint8_t*)buf);
//		sprintf(buf,"%03d",battery);
//		LCD_DisplayStringLine_48(47,260,(uint8_t*)buf);
		if(dataupdate == 100)
		{
			
			//��ʾƽ��ֵ
			LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
	//		TEMPAVG = 1000.1;
			AVGCAL();//����ƽ�������Сֵ
			sprintf(buf,"%.1f",TEMPAVG);
			if(TEMPAVG >= 0)
			{
				if(TEMPAVG < 10)
				{
					strcat(buf,"   ");
				}else if(TEMPAVG < 100){
					strcat(buf,"  ");
				}else if(TEMPAVG < 1000){
					strcat(buf," ");
				}
			}else{
				if(TEMPAVG > -10)
				{
					strcat(buf,"  ");
				}else if(TEMPAVG > -100){
					strcat(buf," ");
				}
			}
			if(TEMPAVG < 3276)
			{
				DISP_TEMP_S(419,421,(uint8_t*)buf,0);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(419,420,48,20);
			}
			
			sprintf(buf,"%.1f",TEMPMAX);
			if(TEMPMAX >= 0)
			{
				if(TEMPMAX < 10)
				{
					strcat(buf,"   ");
				}else if(TEMPMAX < 100){
					strcat(buf,"  ");
				}else if(TEMPMAX < 1000){
					strcat(buf," ");
				}
			}else{
				if(TEMPMAX > -10)
				{
					strcat(buf,"  ");
				}else if(TEMPMAX > -100){
					strcat(buf," ");
				}
			}
			if(TEMPMAX < 3276)
			{
				DISP_TEMP_S(419,501,(uint8_t*)buf,0);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(495,420,48,20);
			}
			
			sprintf(buf,"%.1f",TEMPMIN);
			if(TEMPMIN >= 0)
			{
				if(TEMPMIN < 10)
				{
					strcat(buf,"   ");
				}else if(TEMPMIN < 100){
					strcat(buf,"  ");
				}else if(TEMPMIN < 1000){
					strcat(buf," ");
				}
			}else{
				if(TEMPMIN > -10)
				{
					strcat(buf,"  ");
				}else if(TEMPMIN > -100){
					strcat(buf," ");
				}
			}
			if(TEMPMIN < 3276)
			{
				DISP_TEMP_S(419,581,(uint8_t*)buf,0);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(579,420,48,20);
			}
			
			dataupdate = 0;
		}else{
			dataupdate++;
		}
////////////////////////////////////////////////////////		
		//��ʾ�¶�		
		if(FONT == big)
		{					
			if(ch_page == page1)
			{							
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(1);
				sprintf(buf,"%.1f",CH1TEMP - COR1);
				if(CH1TEMP - COR1 >= 0)
				{
					if(CH1TEMP - COR1 < 10)
					{
						strcat(buf,"   ");
					}else if(CH1TEMP - COR1 < 100){
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH1TEMP - COR1 > -10)
					{
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 > -100){
						strcat(buf," ");
					}
				}
				
				
//				DISP_TEMP_L(95,120,(uint8_t*)buf,CH1_SW);
				if(CH1TEMP < 3276 || CH1TEMP < -200)
				{
					DISP_TEMP_L(95,120,(uint8_t*)buf,CH1_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,95,144,40);
				}
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP - COR2 >= 0)
				{
					if(CH2TEMP - COR2 < 10)
					{
						strcat(buf,"   ");
					}else if(CH2TEMP - COR2 < 100){
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH2TEMP - COR2 > -10)
					{
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 > -100){
						strcat(buf," ");
					}
				}
				if(CH2TEMP < 3276)
				{
					DISP_TEMP_L(165+10,120,(uint8_t*)buf,CH2_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,165+10,144,40);
				}
				
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP - COR3 >= 0)
				{
					if(CH3TEMP - COR3 < 10)
					{
						strcat(buf,"   ");
					}else if(CH3TEMP - COR3 < 100){
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH3TEMP - COR3 > -10)
					{
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 > -100){
						strcat(buf," ");
					}
				}
				if(CH3TEMP < 3276)
				{
					DISP_TEMP_L(235+20,120,(uint8_t*)buf,CH3_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,235+20,144,40);
				}				
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP - COR4 >= 0)
				{
					if(CH4TEMP - COR4 < 10)
					{
						strcat(buf,"   ");
					}else if(CH4TEMP - COR4 < 100){
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH4TEMP - COR4 > -10)
					{
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 > -100){
						strcat(buf," ");
					}
				}
				if(CH4TEMP < 3276)
				{
					DISP_TEMP_L(305+30,120,(uint8_t*)buf,CH4_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,305+30,144,40);
				}				
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP - COR5 >= 0)
				{
					if(CH5TEMP - COR5 < 10)
					{
						strcat(buf,"   ");
					}else if(CH5TEMP - COR5 < 100){
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH5TEMP - COR5 > -10)
					{
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 > -100){
						strcat(buf," ");
					}
				}
				if(CH5TEMP < 3276)
				{
					DISP_TEMP_L(95,440,(uint8_t*)buf,CH5_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,95,144,40);
				}					
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP - COR6 >= 0)
				{
					if(CH6TEMP - COR6 < 10)
					{
						strcat(buf,"   ");
					}else if(CH6TEMP - COR6 < 100){
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH6TEMP - COR6 > -10)
					{
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 > -100){
						strcat(buf," ");
					}
				}
				if(CH6TEMP < 3276)
				{
					DISP_TEMP_L(165+10,440,(uint8_t*)buf,CH6_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,165+10,144,40);
				}					
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP - COR7 >= 0)
				{
					if(CH7TEMP - COR7 < 10)
					{
						strcat(buf,"   ");
					}else if(CH7TEMP - COR7 < 100){
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH7TEMP - COR7 > -10)
					{
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 > -100){
						strcat(buf," ");
					}
				}
				if(CH7TEMP < 3276)
				{
					DISP_TEMP_L(235+20,440,(uint8_t*)buf,CH7_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,235+20,144,40);
				}				
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR8);
				if(CH8TEMP - COR8 >= 0)
				{
					if(CH8TEMP - COR8 < 10)
					{
						strcat(buf,"   ");
					}else if(CH8TEMP - COR8 < 100){
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH8TEMP - COR8 > -10)
					{
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 > -100){
						strcat(buf," ");
					}
				}
				if(CH8TEMP - COR8 < 3276)
				{
					DISP_TEMP_L(305+30,440,(uint8_t*)buf,CH8_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,305+30,144,40);
				}							
			}else if(ch_page == page2){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				Check_limits(9);
				sprintf(buf,"%.1f",CH9TEMP - COR9);
				if(CH9TEMP >= 0)
				{
					if(CH9TEMP - COR9 < 10)
					{
						strcat(buf,"   ");
					}else if(CH9TEMP - COR9 < 100){
						strcat(buf,"  ");
					}else if(CH9TEMP - COR9 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH9TEMP - COR9 > -10)
					{
						strcat(buf,"  ");
					}else if(CH9TEMP - COR9 > -100){
						strcat(buf," ");
					}
				}
				if(CH9TEMP < 3276)
				{
					DISP_TEMP_L(95,120,(uint8_t*)buf,CH9_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,95,144,40);
				}
				
				Check_limits(10);
				sprintf(buf,"%.1f",CH10TEMP - COR10);
				if(CH10TEMP >= 0)
				{
					if(CH10TEMP - COR10 < 10)
					{
						strcat(buf,"   ");
					}else if(CH10TEMP - COR10 < 100){
						strcat(buf,"  ");
					}else if(CH10TEMP - COR10 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH10TEMP - COR10 > -10)
					{
						strcat(buf,"  ");
					}else if(CH10TEMP - COR10 > -100){
						strcat(buf," ");
					}
				}
				if(CH10TEMP < 3276)
				{
					DISP_TEMP_L(165+10,120,(uint8_t*)buf,CH10_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,165+10,144,40);
				}
				
				Check_limits(11);
				sprintf(buf,"%.1f",CH11TEMP - COR11);
				if(CH11TEMP - COR11 >= 0)
				{
					if(CH11TEMP - COR11 < 10)
					{
						strcat(buf,"   ");
					}else if(CH11TEMP - COR11 < 100){
						strcat(buf,"  ");
					}else if(CH11TEMP - COR11 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH11TEMP > -10)
					{
						strcat(buf,"  ");
					}else if(CH11TEMP > -100){
						strcat(buf," ");
					}
				}
				if(CH11TEMP < 3276)
				{
					DISP_TEMP_L(235+20,120,(uint8_t*)buf,CH11_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,235+20,144,40);
				}
				
				Check_limits(12);
				sprintf(buf,"%.1f",CH12TEMP - COR12);
				if(CH12TEMP - COR12 >= 0)
				{
					if(CH12TEMP - COR12 < 10)
					{
						strcat(buf,"   ");
					}else if(CH12TEMP - COR12 < 100){
						strcat(buf,"  ");
					}else if(CH12TEMP - COR12 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH12TEMP - COR12 > -10)
					{
						strcat(buf,"  ");
					}else if(CH12TEMP - COR12 > -100){
						strcat(buf," ");
					}
				}
				if(CH12TEMP < 3276)
				{
					DISP_TEMP_L(305+30,120,(uint8_t*)buf,CH12_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(120,305+30,144,40);
				}
				
				Check_limits(13);
				sprintf(buf,"%.1f",CH13TEMP - COR13);
				if(CH13TEMP - COR13 >= 0)
				{
					if(CH13TEMP - COR13 < 10)
					{
						strcat(buf,"   ");
					}else if(CH13TEMP - COR13 < 100){
						strcat(buf,"  ");
					}else if(CH13TEMP - COR13 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH13TEMP - COR13 > -10)
					{
						strcat(buf,"  ");
					}else if(CH13TEMP - COR13 > -100){
						strcat(buf," ");
					}
				}
				if(CH13TEMP < 3276)
				{
					DISP_TEMP_L(95,440,(uint8_t*)buf,CH13_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,95,144,40);
				}
				
				Check_limits(14);
				sprintf(buf,"%.1f",CH14TEMP - COR14);
				if(CH14TEMP >= 0)
				{
					if(CH14TEMP - COR14 < 10)
					{
						strcat(buf,"   ");
					}else if(CH14TEMP - COR14 < 100){
						strcat(buf,"  ");
					}else if(CH14TEMP - COR14 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH14TEMP - COR14 > -10)
					{
						strcat(buf,"  ");
					}else if(CH14TEMP - COR14 > -100){
						strcat(buf," ");
					}
				}
				if(CH14TEMP < 3276)
				{
					DISP_TEMP_L(165+10,440,(uint8_t*)buf,CH14_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,165+10,144,40);
				}
				
				Check_limits(15);
				sprintf(buf,"%.1f",CH15TEMP - COR15);
				if(CH15TEMP - COR15 >= 0)
				{
					if(CH15TEMP - COR15 < 10)
					{
						strcat(buf,"   ");
					}else if(CH15TEMP - COR15 < 100){
						strcat(buf,"  ");
					}else if(CH15TEMP - COR15 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH15TEMP - COR15 > -10)
					{
						strcat(buf,"  ");
					}else if(CH15TEMP - COR15 > -100){
						strcat(buf," ");
					}
				}
				if(CH15TEMP < 3276)
				{
					DISP_TEMP_L(235+20,440,(uint8_t*)buf,CH15_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,235+20,144,40);
				}
				
				Check_limits(16);
				sprintf(buf,"%.1f",CH16TEMP - COR16);
				if(CH16TEMP - COR16 >= 0)
				{
					if(CH16TEMP - COR16 < 10)
					{
						strcat(buf,"   ");
					}else if(CH16TEMP - COR16 < 100){
						strcat(buf,"  ");
					}else if(CH16TEMP - COR16 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH16TEMP - COR16 > -10)
					{
						strcat(buf,"  ");
					}else if(CH16TEMP - COR16 > -100){
						strcat(buf," ");
					}
				}
				if(CH16TEMP < 3276)
				{
					DISP_TEMP_L(305+30,440,(uint8_t*)buf,CH16_SW); 
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(440,305+30,144,40);
				}
				
			}else if(ch_page == page3){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH17TEMP);
				DISP_TEMP_L(95,120,(uint8_t*)buf,CH17_SW);
				sprintf(buf,"%.1f",CH18TEMP);
				DISP_TEMP_L(165,120,(uint8_t*)buf,CH18_SW);
				sprintf(buf,"%.1f",CH19TEMP);
				DISP_TEMP_L(235,120,(uint8_t*)buf,CH19_SW);
				sprintf(buf,"%.1f",CH20TEMP);
				DISP_TEMP_L(305,120,(uint8_t*)buf,CH20_SW);
				sprintf(buf,"%.1f",CH21TEMP);
				DISP_TEMP_L(95,440,(uint8_t*)buf,CH21_SW);
				sprintf(buf,"%.1f",CH22TEMP);
				DISP_TEMP_L(165+10,440,(uint8_t*)buf,CH22_SW);
				sprintf(buf,"%.1f",CH23TEMP);
				DISP_TEMP_L(235+20,440,(uint8_t*)buf,CH23_SW);
				sprintf(buf,"%.1f",CH24TEMP);
				DISP_TEMP_L(305+30,440,(uint8_t*)buf,CH24_SW); 
			}else if(ch_page == page4){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH25TEMP);
				DISP_TEMP_L(95,120,(uint8_t*)buf,CH25_SW);
				sprintf(buf,"%.1f",CH26TEMP);
				DISP_TEMP_L(165,120,(uint8_t*)buf,CH26_SW);
				sprintf(buf,"%.1f",CH27TEMP);
				DISP_TEMP_L(235,120,(uint8_t*)buf,CH27_SW);
				sprintf(buf,"%.1f",CH28TEMP);
				DISP_TEMP_L(305,120,(uint8_t*)buf,CH28_SW);
				sprintf(buf,"%.1f",CH29TEMP);
				DISP_TEMP_L(95,440,(uint8_t*)buf,CH29_SW);
				sprintf(buf,"%.1f",CH30TEMP);
				DISP_TEMP_L(165+10,440,(uint8_t*)buf,CH30_SW);
				sprintf(buf,"%.1f",CH31TEMP);
				DISP_TEMP_L(235+20,440,(uint8_t*)buf,CH31_SW);
				sprintf(buf,"%.1f",CH32TEMP);
				DISP_TEMP_L(305+30,440,(uint8_t*)buf,CH32_SW); 
			}else if(ch_page == page5){
//				LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
				sprintf(buf,"%.1f",CH33TEMP);
				DISP_TEMP_L(95,120,(uint8_t*)buf,CH33_SW);
				sprintf(buf,"%.1f",CH34TEMP);
				DISP_TEMP_L(165,120,(uint8_t*)buf,CH34_SW);
				sprintf(buf,"%.1f",CH35TEMP);
				DISP_TEMP_L(235,120,(uint8_t*)buf,CH35_SW);
				sprintf(buf,"%.1f",CH36TEMP);
				DISP_TEMP_L(305,120,(uint8_t*)buf,CH36_SW);
				sprintf(buf,"%.1f",CH37TEMP);
				DISP_TEMP_L(95,440,(uint8_t*)buf,CH37_SW);
				sprintf(buf,"%.1f",CH38TEMP);
				DISP_TEMP_L(165+10,440,(uint8_t*)buf,CH38_SW);
				sprintf(buf,"%.1f",CH39TEMP);
				DISP_TEMP_L(235+20,440,(uint8_t*)buf,CH39_SW);
				sprintf(buf,"%.1f",CH40TEMP);
				DISP_TEMP_L(305+30,440,(uint8_t*)buf,CH40_SW); 
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
				
				if(CH1TEMP - COR1 >= 0)
				{
					if(CH1TEMP - COR1 < 10)
					{
						strcat(buf,"   ");
					}else if(CH1TEMP - COR1 < 100){
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH1TEMP - COR1 > -10)
					{
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 > -100){
						strcat(buf," ");
					}
				}
				if(CH1TEMP < 3276)
				{
					DISP_TEMP_M(100,90,(uint8_t*)buf,CH1_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,100,96,28);
				}
				
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP - COR2 >= 0)
				{
					if(CH2TEMP - COR2 < 10)
					{
						strcat(buf,"   ");
					}else if(CH2TEMP - COR2 < 100){
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH2TEMP - COR2 > -10)
					{
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 > -100){
						strcat(buf," ");
					}
				}
				if(CH2TEMP < 3276)
				{
					DISP_TEMP_M(140,90,(uint8_t*)buf,CH2_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,140,96,28);
				}
				
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP - COR3 >= 0)
				{
					if(CH3TEMP - COR3 < 10)
					{
						strcat(buf,"   ");
					}else if(CH3TEMP - COR3 < 100){
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH3TEMP - COR3 > -10)
					{
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 > -100){
						strcat(buf," ");
					}
				}
				if(CH3TEMP < 3276)
				{
					DISP_TEMP_M(180,90,(uint8_t*)buf,CH3_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,180,96,28);
				}
				
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP - COR4 >= 0)
				{
					if(CH4TEMP - COR4 < 10)
					{
						strcat(buf,"   ");
					}else if(CH4TEMP - COR4 < 100){
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH4TEMP - COR4 > -10)
					{
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 > -100){
						strcat(buf," ");
					}
				}
				if(CH4TEMP < 3276)
				{
					DISP_TEMP_M(220,90,(uint8_t*)buf,CH4_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,220,96,28);
				}
				
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP - COR5 >= 0)
				{
					if(CH5TEMP - COR5 < 10)
					{
						strcat(buf,"   ");
					}else if(CH5TEMP - COR5 < 100){
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH5TEMP - COR5 > -10)
					{
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 > -100){
						strcat(buf," ");
					}
				}
				if(CH5TEMP < 3276)
				{
					DISP_TEMP_M(260,90,(uint8_t*)buf,CH5_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,260,96,28);
				}
				
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP - COR6 >= 0)
				{
					if(CH6TEMP - COR6 < 10)
					{
						strcat(buf,"   ");
					}else if(CH6TEMP - COR6 < 100){
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH6TEMP - COR6 > -10)
					{
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 > -100){
						strcat(buf," ");
					}
				}
				if(CH6TEMP < 3276)
				{
					DISP_TEMP_M(300,90,(uint8_t*)buf,CH6_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,300,96,28);
				}
				
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP - COR7 >= 0)
				{
					if(CH7TEMP - COR7 < 10)
					{
						strcat(buf,"   ");
					}else if(CH7TEMP - COR7 < 100){
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH7TEMP - COR7 > -10)
					{
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 > -100){
						strcat(buf," ");
					}
				}
				if(CH7TEMP < 3276)
				{
					DISP_TEMP_M(340,90,(uint8_t*)buf,CH7_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,340,96,28);
				}
				
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR8);
				if(CH8TEMP - COR8 >= 0)
				{
					if(CH8TEMP - COR8 < 10)
					{
						strcat(buf,"   ");
					}else if(CH8TEMP - COR8 < 100){
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH8TEMP - COR8 > -10)
					{
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 > -100){
						strcat(buf," ");
					}
				}
				if(CH8TEMP < 3276)
				{
					DISP_TEMP_M(380,90,(uint8_t*)buf,CH8_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(90,380,96,28);
				}
				
				if(CHNUM > 8)
				{
					Check_limits(9);
					sprintf(buf,"%.1f",CH9TEMP - COR9);
					if(CH9TEMP - COR9 >= 0)
					{
						if(CH9TEMP - COR9 < 10)
						{
							strcat(buf,"   ");
						}else if(CH9TEMP - COR9 < 100){
							strcat(buf,"  ");
						}else if(CH9TEMP - COR9 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH9TEMP - COR9 > -10)
						{
							strcat(buf,"  ");
						}else if(CH9TEMP - COR9 > -100){
							strcat(buf," ");
						}
					}
					if(CH9TEMP < 3276)
					{
						DISP_TEMP_M(100,290+120,(uint8_t*)buf,CH9_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,100,96,28);
					}					
					Check_limits(10);
					sprintf(buf,"%.1f",CH10TEMP - COR10);
					if(CH10TEMP - COR10 >= 0)
					{
						if(CH10TEMP - COR10 < 10)
						{
							strcat(buf,"   ");
						}else if(CH10TEMP - COR10 < 100){
							strcat(buf,"  ");
						}else if(CH10TEMP - COR10 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH10TEMP - COR10 > -10)
						{
							strcat(buf,"  ");
						}else if(CH10TEMP - COR10 > -100){
							strcat(buf," ");
						}
					}
					if(CH10TEMP < 3276)
					{
						DISP_TEMP_M(140,290+120,(uint8_t*)buf,CH10_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,140,96,28);
					}
					
					Check_limits(11);
					sprintf(buf,"%.1f",CH11TEMP - COR11);
					if(CH11TEMP - COR11 >= 0)
					{
						if(CH11TEMP - COR11 < 10)
						{
							strcat(buf,"   ");
						}else if(CH11TEMP - COR11 < 100){
							strcat(buf,"  ");
						}else if(CH11TEMP - COR11 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH11TEMP - COR11 > -10)
						{
							strcat(buf,"  ");
						}else if(CH11TEMP - COR11 > -100){
							strcat(buf," ");
						}
					}
					if(CH11TEMP < 3276)
					{
						DISP_TEMP_M(180,290+120,(uint8_t*)buf,CH11_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,180,96,28);
					}
					
					Check_limits(12);
					sprintf(buf,"%.1f",CH12TEMP - COR12);
					if(CH12TEMP - COR12 >= 0)
					{
						if(CH12TEMP - COR12 < 10)
						{
							strcat(buf,"   ");
						}else if(CH12TEMP - COR12 < 100){
							strcat(buf,"  ");
						}else if(CH12TEMP - COR12 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH12TEMP - COR12 > -10)
						{
							strcat(buf,"  ");
						}else if(CH12TEMP - COR12 > -100){
							strcat(buf," ");
						}
					}
					if(CH12TEMP < 3276)
					{
						DISP_TEMP_M(220,290+120,(uint8_t*)buf,CH12_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,220,96,28);
					}
					
					Check_limits(13);
					sprintf(buf,"%.1f",CH13TEMP - COR13);
					if(CH13TEMP - COR13 >= 0)
					{
						if(CH13TEMP - COR13 < 10)
						{
							strcat(buf,"   ");
						}else if(CH13TEMP - COR13 < 100){
							strcat(buf,"  ");
						}else if(CH13TEMP - COR13 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH13TEMP - COR13 > -10)
						{
							strcat(buf,"  ");
						}else if(CH13TEMP - COR13 > -100){
							strcat(buf," ");
						}
					}
					if(CH13TEMP < 3276)
					{
						DISP_TEMP_M(260,290+120,(uint8_t*)buf,CH13_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,260,96,28);
					}
					
					Check_limits(14);
					sprintf(buf,"%.1f",CH14TEMP - COR14);
					if(CH14TEMP - COR14 >= 0)
					{
						if(CH14TEMP - COR14 < 10)
						{
							strcat(buf,"   ");
						}else if(CH14TEMP - COR14 < 100){
							strcat(buf,"  ");
						}else if(CH14TEMP - COR14 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH14TEMP - COR14 > -10)
						{
							strcat(buf,"  ");
						}else if(CH14TEMP - COR14 > -100){
							strcat(buf," ");
						}
					}
					if(CH14TEMP < 3276)
					{
						DISP_TEMP_M(300,290+120,(uint8_t*)buf,CH14_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,300,96,28);
					}
					
					Check_limits(15);
					sprintf(buf,"%.1f",CH15TEMP - COR15);
					if(CH15TEMP - COR15 >= 0)
					{
						if(CH15TEMP - COR15 < 10)
						{
							strcat(buf,"   ");
						}else if(CH15TEMP - COR15 < 100){
							strcat(buf,"  ");
						}else if(CH15TEMP - COR15 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH15TEMP - COR15 > -10)
						{
							strcat(buf,"  ");
						}else if(CH15TEMP - COR15 > -100){
							strcat(buf," ");
						}
					}
					if(CH15TEMP < 3276)
					{
						DISP_TEMP_M(340,290+120,(uint8_t*)buf,CH15_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,340,96,28);
					}
					
					Check_limits(16);
					sprintf(buf,"%.1f",CH16TEMP - COR16);
					if(CH16TEMP - COR16 >= 0)
					{
						if(CH16TEMP - COR16 < 10)
						{
							strcat(buf,"   ");
						}else if(CH16TEMP - COR16 < 100){
							strcat(buf,"  ");
						}else if(CH16TEMP - COR16 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH16TEMP - COR16 > -10)
						{
							strcat(buf,"  ");
						}else if(CH16TEMP - COR16 > -100){
							strcat(buf," ");
						}
					}
					if(CH16TEMP < 3276)
					{
						DISP_TEMP_M(380,290+120,(uint8_t*)buf,CH16_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(290+120,380,96,28);
					}
					if(CHNUM > 16)
					{
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
					}
				}
				
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
				if(CH1TEMP - COR1 >= 0)
				{
					if(CH1TEMP - COR1 < 10)
					{
						strcat(buf,"   ");
					}else if(CH1TEMP - COR1 < 100){
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH1TEMP - COR1 > -10)
					{
						strcat(buf,"  ");
					}else if(CH1TEMP - COR1 > -100){
						strcat(buf," ");
					}
				}
				if(CH1TEMP < 3276)
				{
					DISP_TEMP_S(100,60,(uint8_t*)buf,CH1_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,100,48,20);
				}
				Check_limits(2);
				sprintf(buf,"%.1f",CH2TEMP - COR2);
				if(CH2TEMP - COR2 >= 0)
				{
					if(CH2TEMP - COR2 < 10)
					{
						strcat(buf,"   ");
					}else if(CH2TEMP - COR2 < 100){
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH2TEMP - COR2 > -10)
					{
						strcat(buf,"  ");
					}else if(CH2TEMP - COR2 > -100){
						strcat(buf," ");
					}
				}
				if(CH2TEMP < 3276)
				{
					DISP_TEMP_S(140,60,(uint8_t*)buf,CH2_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,140,48,20);
				}
				Check_limits(3);
				sprintf(buf,"%.1f",CH3TEMP - COR3);
				if(CH3TEMP - COR3 >= 0)
				{
					if(CH3TEMP - COR3 < 10)
					{
						strcat(buf,"   ");
					}else if(CH3TEMP - COR3 < 100){
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH3TEMP - COR3 > -10)
					{
						strcat(buf,"  ");
					}else if(CH3TEMP - COR3 > -100){
						strcat(buf," ");
					}
				}
				if(CH3TEMP < 3276)
				{
					DISP_TEMP_S(180,60,(uint8_t*)buf,CH3_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,180,48,20);
				}
				Check_limits(4);
				sprintf(buf,"%.1f",CH4TEMP - COR4);
				if(CH4TEMP - COR4 >= 0)
				{
					if(CH4TEMP - COR4 < 10)
					{
						strcat(buf,"   ");
					}else if(CH4TEMP - COR4 < 100){
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH4TEMP - COR4 > -10)
					{
						strcat(buf,"  ");
					}else if(CH4TEMP - COR4 > -100){
						strcat(buf," ");
					}
				}
				if(CH4TEMP < 3276)
				{
					DISP_TEMP_S(220,60,(uint8_t*)buf,CH4_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,220,48,20);
				}
				Check_limits(5);
				sprintf(buf,"%.1f",CH5TEMP - COR5);
				if(CH5TEMP - COR5 >= 0)
				{
					if(CH5TEMP - COR5 < 10)
					{
						strcat(buf,"   ");
					}else if(CH5TEMP - COR5 < 100){
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH5TEMP - COR5 > -10)
					{
						strcat(buf,"  ");
					}else if(CH5TEMP - COR5 > -100){
						strcat(buf," ");
					}
				}
				if(CH5TEMP < 3276)
				{
					DISP_TEMP_S(260,60,(uint8_t*)buf,CH5_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,260,48,20);
				}
				Check_limits(6);
				sprintf(buf,"%.1f",CH6TEMP - COR6);
				if(CH6TEMP - COR6 >= 0)
				{
					if(CH6TEMP - COR6 < 10)
					{
						strcat(buf,"   ");
					}else if(CH6TEMP - COR6 < 100){
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH6TEMP - COR6 > -10)
					{
						strcat(buf,"  ");
					}else if(CH6TEMP - COR6 > -100){
						strcat(buf," ");
					}
				}
				if(CH6TEMP < 3276)
				{
					DISP_TEMP_S(300,60,(uint8_t*)buf,CH6_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,300,48,20);
				}
				Check_limits(7);
				sprintf(buf,"%.1f",CH7TEMP - COR7);
				if(CH7TEMP - COR7 >= 0)
				{
					if(CH7TEMP - COR7 < 10)
					{
						strcat(buf,"   ");
					}else if(CH7TEMP - COR7 < 100){
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH7TEMP - COR7 > -10)
					{
						strcat(buf,"  ");
					}else if(CH7TEMP - COR7 > -100){
						strcat(buf," ");
					}
				}
				if(CH7TEMP < 3276)
				{
					DISP_TEMP_S(340,60,(uint8_t*)buf,CH7_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,340,48,20);
				}
				Check_limits(8);
				sprintf(buf,"%.1f",CH8TEMP - COR8);
				if(CH8TEMP - COR8 >= 0)
				{
					if(CH8TEMP - COR8 < 10)
					{
						strcat(buf,"   ");
					}else if(CH8TEMP - COR8 < 100){
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 < 1000){
						strcat(buf," ");
					}
				}else{
					if(CH8TEMP - COR8 > -10)
					{
						strcat(buf,"  ");
					}else if(CH8TEMP - COR8 > -100){
						strcat(buf," ");
					}
				}	
				if(CH8TEMP < 3276)
				{
					DISP_TEMP_S(380,60,(uint8_t*)buf,CH8_SW);
				}else{
					LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
					LCD_DrawFullRect(60,380,48,20);
				}
				if(CHNUM > 8)
				{
					Check_limits(9);
					sprintf(buf,"%.1f",CH9TEMP - COR9);
					if(CH9TEMP - COR9 >= 0)
					{
						if(CH9TEMP - COR9 < 10)
						{
							strcat(buf,"   ");
						}else if(CH9TEMP - COR9 < 100){
							strcat(buf,"  ");
						}else if(CH9TEMP - COR9 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH9TEMP - COR9 > -10)
						{
							strcat(buf,"  ");
						}else if(CH9TEMP - COR9 > -100){
							strcat(buf," ");
						}
					}
					if(CH9TEMP < 3276)
					{
						DISP_TEMP_S(100,184,(uint8_t*)buf,CH9_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,100,48,20);
					}
					
					Check_limits(10);
					sprintf(buf,"%.1f",CH10TEMP - COR10);
					if(CH10TEMP - COR10 >= 0)
					{
						if(CH10TEMP - COR10 < 10)
						{
							strcat(buf,"   ");
						}else if(CH10TEMP - COR10 < 100){
							strcat(buf,"  ");
						}else if(CH10TEMP - COR10 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH10TEMP - COR10 > -10)
						{
							strcat(buf,"  ");
						}else if(CH10TEMP - COR10 > -100){
							strcat(buf," ");
						}
					}
					if(CH10TEMP < 3276)
					{
						DISP_TEMP_S(140,184,(uint8_t*)buf,CH10_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,140,48,20);
					}
					
					Check_limits(11);
					sprintf(buf,"%.1f",CH11TEMP - COR11);
					if(CH11TEMP - COR11 >= 0)
					{
						if(CH11TEMP - COR11 < 10)
						{
							strcat(buf,"   ");
						}else if(CH11TEMP - COR11 < 100){
							strcat(buf,"  ");
						}else if(CH11TEMP - COR11 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH11TEMP - COR11 > -10)
						{
							strcat(buf,"  ");
						}else if(CH11TEMP - COR11 > -100){
							strcat(buf," ");
						}
					}
					if(CH11TEMP < 3276)
					{
						DISP_TEMP_S(180,184,(uint8_t*)buf,CH11_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,180,48,20);
					}
					
					Check_limits(12);
					sprintf(buf,"%.1f",CH12TEMP - COR12);
					if(CH12TEMP - COR12 >= 0)
					{
						if(CH12TEMP - COR12 < 10)
						{
							strcat(buf,"   ");
						}else if(CH12TEMP - COR12 < 100){
							strcat(buf,"  ");
						}else if(CH12TEMP - COR12 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH12TEMP - COR12 > -10)
						{
							strcat(buf,"  ");
						}else if(CH12TEMP - COR12 > -100){
							strcat(buf," ");
						}
					}
					if(CH12TEMP < 3276)
					{
						DISP_TEMP_S(220,184,(uint8_t*)buf,CH12_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,220,48,20);
					}
					
					Check_limits(13);
					sprintf(buf,"%.1f",CH13TEMP - COR13);
					if(CH13TEMP - COR13 >= 0)
					{
						if(CH13TEMP - COR13 < 10)
						{
							strcat(buf,"   ");
						}else if(CH13TEMP - COR13 < 100){
							strcat(buf,"  ");
						}else if(CH13TEMP - COR13 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH13TEMP - COR13 > -10)
						{
							strcat(buf,"  ");
						}else if(CH13TEMP - COR13 > -100){
							strcat(buf," ");
						}
					}
					if(CH13TEMP < 3276)
					{
						DISP_TEMP_S(260,184,(uint8_t*)buf,CH13_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,260,48,20);
					}
					
					Check_limits(14);
					sprintf(buf,"%.1f",CH14TEMP - COR14);
					if(CH14TEMP - COR14 >= 0)
					{
						if(CH14TEMP - COR14 < 10)
						{
							strcat(buf,"   ");
						}else if(CH14TEMP - COR14 < 100){
							strcat(buf,"  ");
						}else if(CH14TEMP - COR14 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH14TEMP - COR14 > -10)
						{
							strcat(buf,"  ");
						}else if(CH14TEMP - COR14 > -100){
							strcat(buf," ");
						}
					}
					if(CH14TEMP < 3276)
					{
						DISP_TEMP_S(300,184,(uint8_t*)buf,CH14_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,300,48,20);
					}
					
					Check_limits(15);
					sprintf(buf,"%.1f",CH15TEMP - COR15);
					if(CH15TEMP - COR15 >= 0)
					{
						if(CH15TEMP - COR15 < 10)
						{
							strcat(buf,"   ");
						}else if(CH15TEMP - COR15 < 100){
							strcat(buf,"  ");
						}else if(CH15TEMP - COR15 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH15TEMP - COR15 > -10)
						{
							strcat(buf,"  ");
						}else if(CH15TEMP - COR15 > -100){
							strcat(buf," ");
						}
					}
					if(CH15TEMP < 3276)
					{
						DISP_TEMP_S(340,184,(uint8_t*)buf,CH15_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,340,48,20);
					}
					
					Check_limits(16);
					sprintf(buf,"%.1f",CH16TEMP - COR16);
					if(CH16TEMP - COR16 >= 0)
					{
						if(CH16TEMP - COR16 < 10)
						{
							strcat(buf,"   ");
						}else if(CH16TEMP - COR16 < 100){
							strcat(buf,"  ");
						}else if(CH16TEMP - COR16 < 1000){
							strcat(buf," ");
						}
					}else{
						if(CH16TEMP - COR16 > -10)
						{
							strcat(buf,"  ");
						}else if(CH16TEMP - COR16 > -100){
							strcat(buf," ");
						}
					}
					if(CH16TEMP < 3276)
					{
						DISP_TEMP_S(380,184,(uint8_t*)buf,CH16_SW);
					}else{
						LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
						LCD_DrawFullRect(184,380,48,20);
					}
					if(CHNUM > 16)
					{
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
				}
		}
	}else if(page_flag == graph){
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
		sprintf(buf,"%.1f",CH1TEMP - COR1);
		if(CH1TEMP - COR1 >= 0)
		{
			if(CH1TEMP - COR1 < 10)
			{
				strcat(buf,"   ");
			}else if(CH1TEMP - COR1 < 100){
				strcat(buf,"  ");
			}else if(CH1TEMP - COR1 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH1TEMP - COR1 > -10)
			{
				strcat(buf,"  ");
			}else if(CH1TEMP - COR1 > -100){
				strcat(buf," ");
			}
		}
		if(CH1TEMP < 3276)
		{
			DISP_CNL_S(40+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,40+20,48,16);
		}
		
		
		LCD_SetColors(LCD_COLOR_GREEN,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH2TEMP - COR2);
		if(CH2TEMP - COR2 >= 0)
		{
			if(CH2TEMP - COR2 < 10)
			{
				strcat(buf,"   ");
			}else if(CH2TEMP - COR2 < 100){
				strcat(buf,"  ");
			}else if(CH2TEMP - COR2 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH2TEMP - COR2 > -10)
			{
				strcat(buf,"  ");
			}else if(CH2TEMP - COR2 > -100){
				strcat(buf," ");
			}
		}
		if(CH2TEMP < 3276)
		{
			DISP_CNL_S(60+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,60+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_MAGENTA,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH3TEMP - COR3);
		if(CH3TEMP - COR3 >= 0)
		{
			if(CH3TEMP - COR3 < 10)
			{
				strcat(buf,"   ");
			}else if(CH3TEMP - COR3 < 100){
				strcat(buf,"  ");
			}else if(CH3TEMP - COR3 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH3TEMP - COR3 > -10)
			{
				strcat(buf,"  ");
			}else if(CH3TEMP - COR3 > -100){
				strcat(buf," ");
			}
		}
		if(CH3TEMP < 3276)
		{
			DISP_CNL_S(80+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,80+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_CYAN,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH4TEMP - COR4);
		if(CH4TEMP - COR4 >= 0)
		{
			if(CH4TEMP - COR4 < 10)
			{
				strcat(buf,"   ");
			}else if(CH4TEMP - COR4 < 100){
				strcat(buf,"  ");
			}else if(CH4TEMP - COR4 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH4TEMP - COR4 > -10)
			{
				strcat(buf,"  ");
			}else if(CH4TEMP - COR4 > -100){
				strcat(buf," ");
			}
		}
		if(CH4TEMP < 3276)
		{
			DISP_CNL_S(100+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,100+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH5TEMP - COR5);
		if(CH5TEMP - COR5 >= 0)
		{
			if(CH5TEMP - COR5 < 10)
			{
				strcat(buf,"   ");
			}else if(CH5TEMP - COR5 < 100){
				strcat(buf,"  ");
			}else if(CH5TEMP - COR5 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH5TEMP - COR5 > -10)
			{
				strcat(buf,"  ");
			}else if(CH5TEMP - COR5 > -100){
				strcat(buf," ");
			}
		}
		if(CH5TEMP < 3276)
		{
			DISP_CNL_S(120+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,120+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_HLT,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH6TEMP - COR6);
		if(CH6TEMP - COR6 >= 0)
		{
			if(CH6TEMP - COR6 < 10)
			{
				strcat(buf,"   ");
			}else if(CH6TEMP - COR6 < 100){
				strcat(buf,"  ");
			}else if(CH6TEMP - COR6 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH6TEMP - COR6 > -10)
			{
				strcat(buf,"  ");
			}else if(CH6TEMP - COR6 > -100){
				strcat(buf," ");
			}
		}
		if(CH6TEMP < 3276)
		{
			DISP_CNL_S(140+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,140+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_BT,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH7TEMP - COR7);
		if(CH7TEMP - COR7 >= 0)
		{
			if(CH7TEMP - COR7 < 10)
			{
				strcat(buf,"   ");
			}else if(CH7TEMP - COR7 < 100){
				strcat(buf,"  ");
			}else if(CH7TEMP - COR7 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH7TEMP - COR7 > -10)
			{
				strcat(buf,"  ");
			}else if(CH7TEMP - COR7 > -100){
				strcat(buf," ");
			}
		}
		if(CH7TEMP < 3276)
		{
			DISP_CNL_S(160+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,160+20,48,16);
		}
		
		LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BACK);		
		sprintf(buf,"%.1f",CH8TEMP - COR8);
		if(CH8TEMP - COR8 >= 0)
		{
			if(CH8TEMP - COR8 < 10)
			{
				strcat(buf,"   ");
			}else if(CH8TEMP - COR8 < 100){
				strcat(buf,"  ");
			}else if(CH8TEMP - COR8 < 1000){
				strcat(buf," ");
			}
		}else{
			if(CH8TEMP - COR8 > -10)
			{
				strcat(buf,"  ");
			}else if(CH8TEMP - COR8 > -100){
				strcat(buf," ");
			}
		}
		if(CH8TEMP < 3276)
		{
			DISP_CNL_S(180+20,585,(uint8_t*)buf);
		}else{
			LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
			LCD_DrawFullRect(585,180+20,48,16);
		}
		
		if(CHNUM > 8)
		{
			LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH9TEMP - COR9);
			if(CH9TEMP - COR9 >= 0)
			{
				if(CH9TEMP - COR9 < 10)
				{
					strcat(buf,"   ");
				}else if(CH9TEMP - COR9 < 100){
					strcat(buf,"  ");
				}else if(CH9TEMP - COR9 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH9TEMP - COR9 > -10)
				{
					strcat(buf,"  ");
				}else if(CH9TEMP - COR9 > -100){
					strcat(buf," ");
				}
			}
			if(CH9TEMP < 3276)
			{
				DISP_CNL_S(200+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,200+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR1,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH10TEMP - COR10);
			if(CH10TEMP - COR10 >= 0)
			{
				if(CH10TEMP - COR10 < 10)
				{
					strcat(buf,"   ");
				}else if(CH10TEMP - COR10 < 100){
					strcat(buf,"  ");
				}else if(CH10TEMP - COR10 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH10TEMP - COR10 > -10)
				{
					strcat(buf,"  ");
				}else if(CH10TEMP - COR10 > -100){
					strcat(buf," ");
				}
			}
			if(CH10TEMP < 3276)
			{
				DISP_CNL_S(220+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,220+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR2,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH11TEMP - COR11);
			if(CH11TEMP - COR11 >= 0)
			{
				if(CH11TEMP - COR11 < 10)
				{
					strcat(buf,"   ");
				}else if(CH11TEMP - COR11 < 100){
					strcat(buf,"  ");
				}else if(CH11TEMP - COR11 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH11TEMP - COR11 > -10)
				{
					strcat(buf,"  ");
				}else if(CH11TEMP - COR11 > -100){
					strcat(buf," ");
				}
			}
			if(CH11TEMP < 3276)
			{
				DISP_CNL_S(240+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,240+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR3,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH12TEMP - COR12);
			if(CH12TEMP - COR12 >= 0)
			{
				if(CH12TEMP - COR12 < 10)
				{
					strcat(buf,"   ");
				}else if(CH12TEMP - COR12 < 100){
					strcat(buf,"  ");
				}else if(CH12TEMP - COR12 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH12TEMP - COR12 > -10)
				{
					strcat(buf,"  ");
				}else if(CH12TEMP - COR12 > -100){
					strcat(buf," ");
				}
			}
			if(CH12TEMP < 3276)
			{
				DISP_CNL_S(260+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,260+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR4,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH13TEMP - COR13);
			if(CH13TEMP - COR13 >= 0)
			{
				if(CH13TEMP - COR13 < 10)
				{
					strcat(buf,"   ");
				}else if(CH13TEMP - COR13 < 100){
					strcat(buf,"  ");
				}else if(CH13TEMP - COR13 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH13TEMP - COR13 > -10)
				{
					strcat(buf,"  ");
				}else if(CH13TEMP - COR13 > -100){
					strcat(buf," ");
				}
			}
			if(CH13TEMP < 3276)
			{
				DISP_CNL_S(280+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,280+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR5,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH14TEMP - COR14);
			if(CH14TEMP - COR14 >= 0)
			{
				if(CH14TEMP - COR14 < 10)
				{
					strcat(buf,"   ");
				}else if(CH14TEMP - COR14 < 100){
					strcat(buf,"  ");
				}else if(CH14TEMP - COR14 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH14TEMP - COR14 > -10)
				{
					strcat(buf,"  ");
				}else if(CH14TEMP - COR14 > -100){
					strcat(buf," ");
				}
			}
			if(CH14TEMP < 3276)
			{
				DISP_CNL_S(300+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,300+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR6,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH15TEMP - COR15);
			if(CH15TEMP - COR15 >= 0)
			{
				if(CH15TEMP - COR15 < 10)
				{
					strcat(buf,"   ");
				}else if(CH15TEMP - COR15 < 100){
					strcat(buf,"  ");
				}else if(CH15TEMP - COR15 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH15TEMP - COR15 > -10)
				{
					strcat(buf,"  ");
				}else if(CH15TEMP - COR15 > -100){
					strcat(buf," ");
				}
			}
			if(CH15TEMP < 3276)
			{
				DISP_CNL_S(320+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,320+20,48,16);
			}
			
			LCD_SetColors(LCD_COLOR_GR7,LCD_COLOR_BACK);		
			sprintf(buf,"%.1f",CH16TEMP - COR16);
			if(CH16TEMP - COR16 >= 0)
			{
				if(CH16TEMP - COR16 < 10)
				{
					strcat(buf,"   ");
				}else if(CH16TEMP - COR16 < 100){
					strcat(buf,"  ");
				}else if(CH16TEMP - COR16 < 1000){
					strcat(buf," ");
				}
			}else{
				if(CH16TEMP - COR16 > -10)
				{
					strcat(buf,"  ");
				}else if(CH16TEMP - COR16 > -100){
					strcat(buf," ");
				}
			}
			if(CH16TEMP < 3276)
			{
				DISP_CNL_S(340+20,585,(uint8_t*)buf);
			}else{
				LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
				LCD_DrawFullRect(585,320+20,48,16);
			}
			
		}
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
	
	BEEPCHECK();
}

void Save_flag(void)
{
	Touch_save_Typedef *ptt;
	ptt=&Touch_save;
	
	SPI_FLASH_SectorErase(0);
	SPI_FLASH_BufferWrite((void*)savedata,SPI_FLASH_PageSize*0, sizeof(savedata));
	SPI_FLASH_BufferWrite((void*)TempHLimits,SPI_FLASH_PageSize*1, sizeof(TempHLimits));
	SPI_FLASH_BufferWrite((void*)TempLLimits,SPI_FLASH_PageSize*2, sizeof(TempLLimits));
	SPI_FLASH_BufferWrite((void*)YLIMIT,SPI_FLASH_PageSize*3, sizeof(YLIMIT));
	SPI_FLASH_BufferWrite((void*)Correction,SPI_FLASH_PageSize*4, sizeof(Correction));
	SPI_FLASH_BufferWrite((void*)corpara,SPI_FLASH_PageSize*5, sizeof(corpara));
	SPI_FLASH_BufferWrite((void*)SN,SPI_FLASH_PageSize*6, sizeof(SN));
	SPI_FLASH_BufferWrite((u8 *)ptt,SPI_FLASH_PageSize*7, sizeof(Touch_save));
	SPI_FLASH_BufferWrite((void*)usavetime,SPI_FLASH_PageSize*8, sizeof(usavetime));
//	Save_Sflag();
}

void Read_flag(void)
{
	Touch_save_Typedef *ptt;
	ptt=&Touch_save;
	
	SPI_FLASH_BufferRead((void *)savedata,SPI_FLASH_PageSize*0, sizeof(savedata));
	SPI_FLASH_BufferRead((void *)TempHLimits,SPI_FLASH_PageSize*1, sizeof(TempHLimits));
	SPI_FLASH_BufferRead((void *)TempLLimits,SPI_FLASH_PageSize*2, sizeof(TempLLimits));
	SPI_FLASH_BufferRead((void *)YLIMIT,SPI_FLASH_PageSize*3, sizeof(YLIMIT));
	SPI_FLASH_BufferRead((void*)Correction,SPI_FLASH_PageSize*4, sizeof(Correction));
	SPI_FLASH_BufferRead((void*)corpara,SPI_FLASH_PageSize*5, sizeof(corpara));
	SPI_FLASH_BufferRead((void*)SN,SPI_FLASH_PageSize*6, sizeof(SN));
	SPI_FLASH_BufferRead((u8 *)ptt,SPI_FLASH_PageSize*7, sizeof(Touch_save));
	SPI_FLASH_BufferRead((void*)usavetime,SPI_FLASH_PageSize*8, sizeof(usavetime));
	Read_Sflag();
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
//	u8 j;
	uint16_t sendcrc;
	u8 creclen;
	u8 csendlen;

//	u16 voltage;//��ѹ
//	u16 current;
//	u32 power;  //����
//	u16 frequancy;
//	u16 PF;//��������
	
	if(usbbuf[0] == 0x01)
	{
		if(usbbuf[1] == 0x03)//������
		{
			free(crec);
			free(csend);
			if(usbbuf[2] == 0 && usbbuf[3] == 0)
			{
				readcrc = usbbuf[4] << 8|usbbuf[5];
				creclen = 4;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
				for(i = 0;i < 4;i++)
				{
					crec[i] = usbbuf[i];
				}
			}else{
				readcrc = usbbuf[6] << 8|usbbuf[7];
				creclen = 6;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
				for(i = 0;i < 6;i++)
				{
					crec[i] = usbbuf[i];
				}				
			}				
			crcwatch = CRC16(crec,creclen);
			if(CRC16(crec,creclen) == readcrc)//CRCУ��
			{
				
				if(usbbuf[2] == 0 && usbbuf[3] == 0)//��ʵʱ����
				{
					csendlen = 38;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
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
							watch = (((u16)RecBuff[5+usbbuf[3]+i*2] << 8) + RecBuff[6+usbbuf[3]+i*2] - (int)(Correction[usbbuf[3] + i] * 10));
							if((((u16)RecBuff[5+usbbuf[3]+i*2] << 8) + RecBuff[6+usbbuf[3]+i*2] - (int)(Correction[usbbuf[3] + i] * 10)) == 0x7FFF)
							{
								usbsendbuf[6+i*2] = 0x7F;
								usbsendbuf[7+i*2] = 0xFF;
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
						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���						
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
//						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
//					}
					
//					for(i = 0;i < 8;i++)
//					{
//						csendlen = 23;				
//						csend = (u8*)malloc(sizeof(u8) * csendlen);				
//						memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
//						USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
//					}
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x00){//��ȡʱ��
					
					csendlen = 15;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);		
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��

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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x20){//��ȡ����������
					
					csendlen = 47;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x28;
					for(i=0;i<40;i++)
					{
						if(i < CHNUM)
						{
							usbsendbuf[i+7]=savedata[60+i];
						}else{
							usbsendbuf[i+7] = 0;
						}
					}

//					usbsendbuf[8] = TCTYPE;
					
					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[47] = (u8)(sendcrc >> 8);
					usbsendbuf[48] = (u8)(sendcrc);
//					for(i = 11; i < 64 ; i++)
//					{
//						usbsendbuf[i] = 0;
//					}
					
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}else if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x10){//��ȡ����״̬
					
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
					
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}else if(usbbuf[2] == 0x01 && usbbuf[3] == 0x01){//��ȡ������
					
					csendlen = 11;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}else if(usbbuf[2] == 0x02 && usbbuf[3] == 0x58){//��ȡ��λ
					free(csend);
					csendlen = 9;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
					
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}else if(usbbuf[2] == 0xEE && usbbuf[3] == 0xEE){//�������״̬
					
					csendlen = 18;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
					
//					usbsendbuf[0] = 0x00;
					usbsendbuf[0] = 0x01;
					usbsendbuf[1] = 0x03;
					usbsendbuf[2] = usbbuf[2];
					usbsendbuf[3] = usbbuf[3];
					usbsendbuf[4] = usbbuf[4];
					usbsendbuf[5] = usbbuf[5];
					usbsendbuf[6] = 0x4A;
					usbsendbuf[7] = 0x4B;
					usbsendbuf[8] = 0x35;
					usbsendbuf[9] = 0x30;
					usbsendbuf[10] = 0x38;
					usbsendbuf[11] = 0x00;
					usbsendbuf[12] = 0x00;
					usbsendbuf[13] = CHNUM;
					usbsendbuf[14] = 0x00;
					usbsendbuf[15] = 0x00;
					usbsendbuf[16] = 0x00;
					usbsendbuf[17] = 0x00;

					for(i = 0;i < csendlen; i++)
					{
						csend[i] = usbsendbuf[i];
					}
					sendcrc = CRC16(csend,csendlen);
					usbsendbuf[18] = (u8)(sendcrc >> 8);
					usbsendbuf[19] = (u8)(sendcrc);
					for(i = 20; i < 64 ; i++)
					{
						usbsendbuf[i] = 0;
					}
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
					
				}else if(usbbuf[2] == 0x80 && usbbuf[3] == 0x10){//��ȡ�����
					
					csendlen = 19;				
					csend = (u8*)malloc(sizeof(u8) * csendlen);				
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					//��������CRCУ�鳤��
					
					
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
					usbsendbuf[7] = 0xFF;
					usbsendbuf[8] = 0xFF;
					usbsendbuf[9] = 0xFF;
					usbsendbuf[10] = 0xFF;
					usbsendbuf[11] = 0xFF;
					usbsendbuf[12] = 0xFF;
					usbsendbuf[13] = 0xFF;
					usbsendbuf[14] = 0xFF;
					usbsendbuf[15] = 0xFF;
					usbsendbuf[16] = 0xFF;
					usbsendbuf[17] = 0xFF;
					usbsendbuf[18] = 0xFF;
					
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
					
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}
			}
			
		}else if(usbbuf[1] == 0x10){
			free(crec);
			free(csend);
			if(usbbuf[2] == 0xC0 && usbbuf[3] == 0x00)//����ʱ��
			{
				readcrc = usbbuf[15] << 8|usbbuf[16];
				creclen = 15;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
					
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}
			}else if(usbbuf[2] == 0x01){//����������
				readcrc = usbbuf[11] << 8|usbbuf[12];
				creclen = 11;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}
			}else if(usbbuf[2] == 0x02 && usbbuf[3] == 0x58){//���õ�λ
				readcrc = usbbuf[9] << 8|usbbuf[10];
				creclen = 9;
				crec = (u8 *)malloc(sizeof(u8) * creclen);
				memset(crec, 0, creclen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
					memset(csend, 0, csendlen);//��ʼ����ÿ��Ԫ�ض�Ϊ��
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
					USBD_HID_SendReport(&USB_OTG_dev,usbsendbuf,64);//���ݻ���
				}
			}
		}
	}
}

//U�̼��
u8 udisk_scan(void)
{
	static u8 res;
	
//	u8 i;
//	char str[64];
	if(usbstatus != UNCONNECTED)
	{
		res = CH376DiskConnect( );
		if( res != USB_INT_SUCCESS )/* ���U���Ƿ�����,�ȴ�U�̲���,����SD��,�����ɵ�Ƭ��ֱ�Ӳ�ѯSD�����Ĳ��״̬���� */
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
		if(res == USB_INT_SUCCESS && page_flag != touchcal)
		{
			DrawUdisk2();
			usbstatus = CONNECTED;
			return UDISK_READY;
		}

	}
	return UDISK_NOTREADY;
}

//�����޼��
void Check_limits(u8 chn)
{
	if((ch_temp[chn-1] - Correction[chn-1]  < TempLLimits[chn-1] || ch_temp[chn-1] - Correction[chn-1] > TempHLimits[chn-1]) && FILTER == ft_on)
	{
		LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_BACK);
		chcomp[chn] = 1;
	}else{
		LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BACK);
		chcomp[chn] = 0;
	}
}

//BCDת��
int hex_to_bcd(int data)
{
//   int binaryInput = 0x202; 
   int bcdResult = 0;
   int shift = 0;

//   printf("Binary: 0x%x (dec: %d)\n", binaryInput , binaryInput );

   while (data > 0) {
      bcdResult |= (data % 10) << (shift++ << 2);
      data /= 10;
   }

//   printf("BCD: 0x%x (dec: %d)\n", bcdResult , bcdResult );
   return bcdResult;
}

//flashȫ������
void Erase_all(void)
{
	static u16 serec = 2;
//	static u8 Check[4096];
	SPI_FLASH_SectorErase(serec*4096);
	SPI_FLASH_SectorErase(15880192+(serec-2)*4096);
	Delay(500);
//	SPI_FLASH_BufferRead((void *)Check,serec*4096, sizeof(Check));
	serec ++;
	SECTOR_REC = 0;
	TIME_REC = 0;
	Save_Sflag();
}

//��flash��ʼֵ����
void Read_Flash_Init_Handle(void)
{
	u16 i;
	for(i=0;i<40;i++)
	{
		if(Correction[i] < -1800)
		{
			Correction[i] = 0;
		}
		if(TempHLimits[i] < -1800)
		{
			TempHLimits[i] = 0;
		}
		if(TempLLimits[i] < -1800)
		{
			TempLLimits[i] = 0;
		}
	}
	
	for(i=0;i<3;i++)
	{
		if(YLIMIT[i] < -1800)
		{
			YLIMIT[i] = 1;
		}
	}
	for(i=60;i<75;i++)
	{
		if(savedata[i] > 20)
		{
			savedata[i] = TCT;
		}
	}
	for(i=0;i<39;i++)
	{
		if(savedata[i] > 1)
		{
			savedata[i] = 0;
		}
	}
	for(i=41;i<59;i++)
	{
		if(savedata[i] == 0xff)
		{
			savedata[i] = 0;
		}
	}
	if(savedata[51] != 16 && savedata[51] != 8)
	{
		savedata[51] = 8;
	}
	if(JKFLAG != 1 && JKFLAG != 0)
		JKFLAG=1;
}
//�ػ����
u8 PowerOffDetect(void)
{
	return GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_11);
}
//��⵽�ػ���Ĵ���
void PowerOffHandle(void)
{
	page_flag = poweroff;
	static u8 offflag;
	DrawPowOff();
	if(recordflag == 1)
	{
		Write_His_Data_Man();
		Write_Block_Rec();
	}
//	if(offflag == 0)
//	{
//		TIME_REC++;
//		SECTOR_REC = TIME_REC * 62;
//		Save_history(SECTOR_REC);								
//		Save_time(TIME_REC);
//		Save_Sflag();
//	}
	
}	
//�����¶�ƽ��ֵ
void AVGCAL(void)
{
	u8 i;
	u8 chnum = 0;
	float temp = 0;
	u8 extflag;
	TEMPMAX = -200;
	TEMPMIN = 3276;
	for(i=0;i<CHNUM;i++)
	{
		if(savedata[i] == ch_on && ch_temp[i] < 3276)
		{
			extflag = 1;
			break;
		}
	}
	if(extflag == 1)
	{
		for(i=0;i<CHNUM;i++)
		{
			if(savedata[i] == ch_on && ch_temp[i] < 3276)
			{
				temp += ch_temp[i] - Correction[i];	
				chnum ++;
				if((ch_temp[i] - Correction[i]) > TEMPMAX)
				{
					TEMPMAX = ch_temp[i] - Correction[i];
				}
				if((ch_temp[i] - Correction[i]) < TEMPMIN)
				{
					TEMPMIN = ch_temp[i] - Correction[i];
				}
			}else{
				
			}
		}
		TEMPAVG = temp/chnum;
	}else{
		TEMPAVG = 0;
		TEMPMAX = 0;
		TEMPMIN = 0;
		
	}
}

void BEEPCHECK(void)
{
	u8 i,sw;
	sw = 0;
	for(i=0;i<40;i++)
	{
		if(chcomp[i] == 1)
		{
			sw = 1;
		}
	}
	if(sw == 1 && BEEP == beep_on && page_flag == display)
	{
		BEEP_ON;
	}else{
		BEEP_OFF;
	}
}

/*********************************************END OF FILE**********************/

