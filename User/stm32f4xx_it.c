/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
#include "./usart/bsp_debug_usart.h"
#include "jk508.h"
#include "usb_core.h"
#include "usbd_core.h"
//#include "usbd_hid_core.h"
#include "./lcd/bsp_lcd.h"
#include "usb_conf.h"
#include "./flash/bsp_spi_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CURSOR_STEP     10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t remote_wakeup =0;
extern __IO uint32_t FlashID;
/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
//static uint8_t *USBD_HID_GetPos (void);
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
extern uint32_t USBD_OTG_EP1IN_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern uint32_t USBD_OTG_EP1OUT_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
#endif

u8 RecBuff[40];
u16 ureadcrc;
u8 *ucrc;
u8 charge;
u8 battery;
u8 reflag;
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		LED_RED;
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 
//void P6_IRQHandler(void)
//{
//  //确保是否产生了EXTI Line中断
//	if(EXTI_GetITStatus(P6_EXTI_LINE) != RESET) 
//	{
//			
//		key_value = Key_Scan();
//    //清除中断标志位
//		EXTI_ClearITPendingBit(P6_EXTI_LINE);     
	
//	}
//}	

void DEBUG_USART_IRQHandler(void)
{
	uint8_t ucTemp;
	static u8 ReCount=0;
	static u8 Total_Len = 0;
	static u8 uinitflag = 0;
	static u8 multicount = 0;
	static u8 bcount;
	static u16 btbuff;
	static u8 recflag = 0;
	static float graphbuf[16];
	

	u8 i;
	char buf[10];
	
//	u16 ureadcrc;
//	u8 *ucrc;
	u8 ucrclen;
	if(uinitflag == 0)
	{
		LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_BLACK);
		LCD_DrawFullRect(0,0,640,480);
		uinitflag = 1;
	}
	if(USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET)
	{
		
		ucTemp = USART_ReceiveData( DEBUG_USART );
		if(ReCount == 0)
		{
			if(ucTemp == 0x01)//	0x01
			{
				ReCount=1;
				RecBuff[0]=ucTemp;
			}
		}else if(ReCount == 1){
			if(ucTemp == 0x03)
			{
				RecBuff[1] = ucTemp;
				Total_Len = 23;
				ReCount ++;
			}
		}else if(ReCount == 2){
			if(ucTemp == 0x22)
			{
				RecBuff[2] = ucTemp;
				Total_Len = 39;
				
				ReCount ++;
			}
		}else if(ReCount > 2){
			RecBuff[ReCount] = ucTemp;
			ReCount ++;
			if(ReCount == Total_Len)
			{
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
//					if(bcount == 3)
//					{
//						battery = btbuff/3;
//						DrawBattery(battery);
//						btbuff = 0;
//						bcount = 0;
//					}else{
//						btbuff += RecBuff[4];
//						bcount++;
//					}
					for(i=0;i<16;i++)
					{
						ch_temp[i] = (RecBuff[2*(i+1)+3] * 256 + RecBuff[2*(i+1)+4])/10.0;
						if(count == 0 && page_flag == poweron)
						{
							InitBrt();//开机亮度
							LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
							DISP_INS(5+i*20,5,"Initializing Channel");
							sprintf(buf,"%03d",i+1);
							DISP_INS(5+i*20,336,(uint8_t*)buf);
							DISP_INS(5+i*20,384,"...");
							Delay(0x3fffff);
						}
						
					}
					
					
					
					if(multicount == 1 && page_flag == poweron)
					{
						LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
						DISP_INS(325,5,"Done!");
						Delay(0xffffff);
						page_home();
					}
					
					if(multicount%(int)MULTI == 0 && multicount != 0)
					{
						for(i=0;i<16;i++)
						{
							G_Data[i][count] = graphbuf[i]/MULTI - Correction[i];							
						}
						if(page_flag == graph)
						{
							Draw_graph();
						}
//						if(page_flag != history)
//						{
//							for(i=0;i<16;i++)
//							{
//								Data_buf[i][count%16] = graphbuf[i]/MULTI;
//							}
//							if(count > 0 && count%16 == 0)
//							{
//								recflag = 1;
//								Save_history(SECTOR_REC);
//								SECTOR_REC++;
//							}
//						}
						if(count > 498)
						{						
							count = 0;
							memcpy(hisdata,G_Data,sizeof(G_Data));
							memcpy(histime,time_buf,sizeof(time_buf));
						}else{
							count++;
						}
						multicount=0;
						
						
						for(i = 0;i<16;i++)
						{
							graphbuf[i] = 0;
						}
						for(i=0;i<16;i++)
						{
							graphbuf[i] += ch_temp[i];							
						}
						multicount++;
					}else{
						for(i=0;i<16;i++)
						{
							graphbuf[i] += ch_temp[i];							
						}
						multicount++;
					}
					
					
					ReCount = 0;
					
				}
				
			}
		}

	}
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS  
void OTG_HS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
#endif

#ifdef USE_USB_OTG_FS  
void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
#endif
#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED 
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);
}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
