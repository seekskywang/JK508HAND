/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   RTC����
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
#include "./RTC/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include "jk508.h"

u8 time_buf[10][7];
u32 rec_num = 0;
u8 usbreadtime[7];
u8 trigflag;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;
u8 oldhour;
/**
  * @brief  ����ʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Set(void)
{
//	RTC_TimeTypeDef RTC_TimeStructure;
//	RTC_DateTypeDef RTC_DateStructure;
	
	// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
	RTC_TimeStructure.RTC_Hours = HOURS;        
	RTC_TimeStructure.RTC_Minutes = MINUTES;    
	RTC_TimeStructure.RTC_Seconds = SECONDS;      
	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = WEEKDAY;       
	RTC_DateStructure.RTC_Date = DATE;         
	RTC_DateStructure.RTC_Month = MONTH;         
	RTC_DateStructure.RTC_Year = YEAR;        
	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
}

/**
  * @brief  ��ʾʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Show(void)
{
	
	char LCDTemp[100];
	
	uint8_t Rtctmp;
	if(RTC_TimeStructure.RTC_Seconds != 0)
	{
		Rtctmp=0;
	}
	
	// ��ȡ����
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	
	//���³�ʼ��ʱ��
	YEAR = RTC_DateStructure.RTC_Year;
	MONTH =RTC_DateStructure.RTC_Month;
	DATE = RTC_DateStructure.RTC_Date;
	
	HOURS = RTC_TimeStructure.RTC_Hours;
	MINUTES = RTC_TimeStructure.RTC_Minutes;
	SECONDS = RTC_TimeStructure.RTC_Seconds;
//	if(oldhour != HOURS)
//	{
//		oldhour = HOURS;
//		indexflag = 1;
//	}
	if(TIMETIRG == trig_off)
	{
//		trigflag = 1;
	}else{
		if((HOURS*60 + MINUTES >= STARTH*60 + STARTM) && (HOURS*60 + MINUTES < ENDH*60 + ENDM)){
			recordflag = 1;
		}else{
			recordflag = 0;
		}
	}
	// ÿ���ӡһ��
  	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
    {					
		if(page_flag != poweron && page_flag != touchcal && GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_11) == 1)
		{
			//Һ����ʾʱ��
			sprintf(LCDTemp,"%0.2d:%0.2d:%0.2d", 
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds);
			
			LCD_SetTextColor(LCD_COLOR_YELLOW);  		
			LCD_SetBackColor(LCD_COLOR_BACK);				
			LCD_DisplayStringLine(5,510,(uint8_t *)LCDTemp);
		}
		
		usbreadtime[0] = (u8)((2000 + RTC_DateStructure.RTC_Year) >> 8);//���λ
		usbreadtime[1] = (u8)(RTC_DateStructure.RTC_Year);//���λ
		usbreadtime[2] = (u8)(RTC_DateStructure.RTC_Month);//��
		usbreadtime[3] = (u8)(RTC_DateStructure.RTC_Date);//��
		usbreadtime[4] = (u8)(RTC_TimeStructure.RTC_Hours);//ʱ
		usbreadtime[5] = (u8)(RTC_TimeStructure.RTC_Minutes);//��
		usbreadtime[6] = (u8)(RTC_TimeStructure.RTC_Seconds);//��

		if(trigflag == 1)
		{

			if(count == 0)
			{
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 50){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 100){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 150){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 200){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 250){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 300){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 350){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 400){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
			}else if(count == 450){
				time_buf[count/50][0] = 20;
				time_buf[count/50][1] = RTC_DateStructure.RTC_Year;
				time_buf[count/50][2] = RTC_DateStructure.RTC_Month;
				time_buf[count/50][3] = RTC_DateStructure.RTC_Date;
				time_buf[count/50][4] = RTC_TimeStructure.RTC_Hours;
				time_buf[count/50][5] = RTC_TimeStructure.RTC_Minutes;
				time_buf[count/50][6] = RTC_TimeStructure.RTC_Seconds;
				
			}
		}else{
			
		}
		
		if(page_flag == sysset)
		{
			//Һ����ʾ����
			//�Ȱ�Ҫ��ʾ��������sprintf����ת��Ϊ�ַ�����Ȼ�������Һ����ʾ������ʾ
			sprintf(LCDTemp,"20%0.2d-%0.2d-%0.2d",
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month,
			RTC_DateStructure.RTC_Date);
			if(op_flag == set_date)
			{
				LCD_SetTextColor(LCD_COLOR_BLACK);  		
				LCD_SetBackColor(LCD_COLOR_YELLOW);				
			}else{
				LCD_SetTextColor(LCD_COLOR_YELLOW);  		
				LCD_SetBackColor(LCD_COLOR_BACK);
			}
			LCD_DisplayStringLine(90,170+30,(uint8_t *)LCDTemp);


			//Һ����ʾʱ��
			sprintf(LCDTemp,"%0.2d:%0.2d:%0.2d",
			RTC_TimeStructure.RTC_Hours,
			RTC_TimeStructure.RTC_Minutes,
			RTC_TimeStructure.RTC_Seconds);
			
			if(op_flag == set_time)
			{
				LCD_SetTextColor(LCD_COLOR_BLACK);  		
				LCD_SetBackColor(LCD_COLOR_YELLOW);				
			}else{
				LCD_SetTextColor(LCD_COLOR_YELLOW);  		
				LCD_SetBackColor(LCD_COLOR_BACK);
			}
			LCD_DisplayStringLine(90,400,(uint8_t *)LCDTemp);
		}
		
		(void)RTC->DR;
    }
    Rtctmp = RTC_TimeStructure.RTC_Seconds;
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
void RTC_CLK_Config(void)
{  
	RTC_InitTypeDef RTC_InitStructure;
	
	/*ʹ�� PWR ʱ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* ʹ��LSI��ΪRTCʱ��Դ������� 
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
  /* ʹ��LSI */ 
  RCC_LSICmd(ENABLE);
  /* �ȴ�LSI�ȶ� */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  /* ѡ��LSI��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE)

  /* ʹ��LSE */ 
  RCC_LSEConfig(RCC_LSE_ON);
   /* �ȴ�LSE�ȶ� */   
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* ѡ��LSE��ΪRTC��ʱ��Դ */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    

#endif /* RTC_CLOCK_SOURCE_LSI */

  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);

  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
   
/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
#define LSE_STARTUP_TIMEOUT     ((uint16_t)0x05000)
void RTC_CLK_Config_Backup(void)
{  
  __IO uint16_t StartUpCounter = 0;
	FlagStatus LSEStatus = RESET;	
	RTC_InitTypeDef RTC_InitStructure;
	
	/* ʹ�� PWR ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);
	
/*=========================ѡ��RTCʱ��Դ==============================*/
/* Ĭ��ʹ��LSE�����LSE��������ʹ��LSI */
  /* ʹ��LSE */
  RCC_LSEConfig(RCC_LSE_ON);	
	
	/* �ȴ�LSE�����ȶ��������ʱ���˳� */
  do
  {
    LSEStatus = RCC_GetFlagStatus(RCC_FLAG_LSERDY);
    StartUpCounter++;
  }while((LSEStatus == RESET) && (StartUpCounter != LSE_STARTUP_TIMEOUT));
	
	
	if(LSEStatus == SET )
  {
		/* ѡ��LSE��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  }
	else
	{
		
		/* ʹ��LSI */	
		RCC_LSICmd(ENABLE);
		/* �ȴ�LSI�ȶ� */ 
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
		{			
		}
		
		printf("\n\r LSI �����ɹ� \r\n");
		/* ѡ��LSI��ΪRTC��ʱ��Դ */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	}
	
  /* ʹ�� RTC ʱ�� */
  RCC_RTCCLKCmd(ENABLE);
  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();

/*=====================��ʼ��ͬ��/�첽Ԥ��Ƶ����ֵ======================*/
	/* ����������ʱ��ck_spare = LSE/[(255+1)*(127+1)] = 1HZ */
	
	/* �����첽Ԥ��Ƶ����ֵΪ127 */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	/* ����ͬ��Ԥ��Ƶ����ֵΪ255 */
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;	
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("\n\r RTC ʱ�ӳ�ʼ��ʧ�� \r\n");
	}	
}

void DrawTime(void)
{
	char LCDTemp[100];
	
	sprintf(LCDTemp,"%0.2d:%0.2d:%0.2d",
			usbreadtime[4],
			usbreadtime[5],
			usbreadtime[6]);
			
	if(count == 0)
	{
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(402,80-80,(uint8_t *)LCDTemp);
	}else if(count == 50){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(420,130-80,(uint8_t *)LCDTemp);
	}else if(count == 100){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(402,180-80,(uint8_t *)LCDTemp);
	}else if(count == 150){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(420,230-80,(uint8_t *)LCDTemp);
	}else if(count == 200){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(402,280-80,(uint8_t *)LCDTemp);
	}else if(count == 250){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);				
		DISP_CNL_S(420,330-80,(uint8_t *)LCDTemp);
	}else if(count == 300){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(402,380-80,(uint8_t *)LCDTemp);
	}else if(count == 350){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(420,430-80,(uint8_t *)LCDTemp);
	}else if(count == 400){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(402,480-80,(uint8_t *)LCDTemp);
	}else if(count == 450){
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_SetBackColor(LCD_COLOR_BACK);
		DISP_CNL_S(420,530-80,(uint8_t *)LCDTemp);
	}else if(count == 495){
		rec_num ++;
//				Save_flag();
		LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
		LCD_DrawFullRect(0,402,600,34);
		DrawGridLine();
	}
}

/**********************************END OF FILE*************************************/
