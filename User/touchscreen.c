/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   触摸屏驱动
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
#include "./RTC/bsp_rtc.h"
#include "./usart/bsp_debug_usart.h"
#include "./lcd/bsp_lcd.h"
#include "jk508.h"
#include "touchscreen.h"

__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};
u16 Xconvert;
u16 Yconvert;
u32 corpara[20];
u16 XCOOR;
u16 YCOOR;

void Touch_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (TOUCH_XPLUS_GPIO_CLK|TOUCH_XMINUS_GPIO_CLK|TOUCH_YPLUS_GPIO_CLK|
	TOUCH_YMINUS_GPIO_CLK,ENABLE); 

	GPIO_InitStructure.GPIO_Pin = TOUCH_YPLUS_GPIO_PIN|TOUCH_YMINUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输入模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_YPLUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XMINUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_XMINUS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = TOUCH_XPLUS_GPIO_PIN;/*选择引脚*/     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;/*设置引脚为输出模式*/   
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;/*设置引脚的输出类型为推挽输出*/    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; /*设置引脚为上拉模式*/	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;/*设置引脚速率为2MHz */    
	GPIO_Init(TOUCH_XPLUS_GPIO_PORT, &GPIO_InitStructure);
	
	
//	GPIO_ResetBits(TOUCH_XMINUS_GPIO_PORT,TOUCH_XMINUS_GPIO_PIN);
}

static void AD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================通道1======================*/	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(ADC_GPIO_CLK0,ENABLE);		
	// 配置 IO
    GPIO_InitStructure.GPIO_Pin = ADC_GPIO_PIN0|ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    //不上拉不下拉	
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(ADC_GPIO_PORT0, &GPIO_InitStructure);

}

static void Rheostat_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
  // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);
  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 扫描模式，多通道采集需要	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //使用软件触发，外部触发不用配置，注释掉即可
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL;                                    
  ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序和采样时间周期
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL0, 1, 
	                         ADC_SampleTime_144Cycles);
  ADC_RegularChannelConfig(RHEOSTAT_ADC, ADC_CHANNEL1, 2, 
	                         ADC_SampleTime_144Cycles); 

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// 使能ADC
  ADC_Cmd(RHEOSTAT_ADC, ENABLE);  
  //开始adc转换，软件触发
  ADC_SoftwareStartConv(RHEOSTAT_ADC);
}

void Touch_Init(void)
{
	AD_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}

static void set_yz(void){	X_P1;	X_N1;	Y_P0;	Y_N0;}
static void set_yy(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}
static void set_xx(void){	X_P1;	X_N0;	Y_P0;	Y_N1;}
static void set_mid(void){	X_P0;	X_N1;	Y_P1;	Y_N0;}


void Touch_Scan(void)
{
	static u8 i,j,k;
	static u16 ADX[5];
	static u16 ADY[5];
	u32 var32;
//	for(i=0;i<100;i++)
//	{
//		if((i%2) == 0)
//		{
//			set_xx();
//			ADX[a++] = ADC_ConvertedValue[i];
//		}else if((i%2) == 1){
//			set_yy();
//			ADY[b++] = ADC_ConvertedValue[i];
//		}
//	}
	if(i%4 < 2)
	{
		set_yy();
		ADY[j] = ADC_ConvertedValue[1];
		if(j<5)
		{
			j++;
		}else{
			j = 0;
		}
	}else{
		set_xx();
		ADX[k] = ADC_ConvertedValue[0];
		if(k<5)
		{
			k++;
		}else{
			k = 0;
		}
	}
	i++;
	Xconvert = (ADX[1] + ADX[3])/2/50;
	Yconvert = (ADY[1] + ADY[3])/2/50;
	

	if(Xconvert <= 4)
	{
		XCOOR = 0;
	}else{
		XCOOR = (int)(8.49*(float)Xconvert-15.48);
	}
	
	if(Yconvert <= 3)
	{
		YCOOR = 0;
	}else{
		YCOOR = 480 - (int)(6.38*(float)Yconvert-9.16);
	}
	
	
}

void TouchCal(void)
{
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_WHITE);
	LCD_DrawFullCircle(10,10,10);
	LCD_DrawFullCircle(10,470,10);
	LCD_DrawFullCircle(630,10,10);
	LCD_DrawFullCircle(630,470,10);
	page_flag = touchcal;
}

void TouchHandle(u16 x,u16 y)
{
	switch(page_flag)
	{
		case display:
		{
			if(FONT == big)
			{
				if(x >= BIGCH1X1 && x < BIGCH1X2 && y > BIGCH1Y1 && y < BIGCH1Y2 )
				{
					if(ch_page == page1)
					{
						focus_on(CH1_SW);
						LCD_DisplayStringLine_48(100,10,"001");
					}else if(ch_page == page2){
						focus_on(CH9_SW);
						LCD_DisplayStringLine_48(100,10,"009");
					}else if(ch_page == page3){
						focus_on(CH17_SW);
						LCD_DisplayStringLine_48(100,10,"017");
					}else if(ch_page == page4){
						focus_on(CH25_SW);
						LCD_DisplayStringLine_48(100,10,"025");
					}else if(ch_page == page5){
						focus_on(CH33_SW);
						LCD_DisplayStringLine_48(100,10,"033");
					}
					if(LANG == chs)
					{
						DrawInstruction("通道设置");
					}else{
						DrawInstruction("Channel set");
					}
					op_flag = home_ch1;
				}			
			}
		}break;
	}
}

void XYCAL(u8 step)
{
	static vu32 Modify_A_READ;
	static vu32 Modify_C_READ;
	static vu32 Modify_A_ACT;
		
	static vu32 Modify_B_READ;
	static vu32 Modify_D_READ;
	static vu32 Modify_B_ACT;
	
    if(step == 1)
    {
        Modify_A_READ += Xconvert;//测量电压值		
    }else if(step == 2){
		Modify_A_READ += Xconvert;
		Modify_A_READ = Modify_A_READ/2;
		Modify_A_ACT = 0x0A;//读取低段
	}else if(step == 3){
		Modify_B_READ += Xconvert;
	}else if(step == 4){
        vu16 var16;
        vu32 var32a;
        vu32 var32b;
        
        vu16 var16a;
        vu32 var32c;
        vu32 var32d;
        Modify_B_READ +=Xconvert;//测量电压值
		Modify_B_READ = Modify_B_READ/2;

        Modify_B_ACT = 0x0276;//读取高段
//        if(flag_OverV==1)//只有当有数据写入时才能将校准数据写入FLASH
//        {
            var32a = Modify_B_ACT;
            var32a = var32a - Modify_A_ACT;
            var32a = var32a << 12;
            var16 = Modify_B_READ - Modify_A_READ;
            var32a = var32a / var16;
            XCOR = var32a;
            var32a=0;
            var32a = Modify_B_ACT;
            var32a = var32a << 12;
            var32b = Modify_B_READ;
            var32b = var32b * XCOR;
            if (var32a < var32b)
            {
                var32b = var32b - var32a;
                XOffset = var32b;
                Polar1 |= 0x01;
            }
            else 
            {
                var32a = var32a - var32b;
                XOffset = var32a;
                Polar1 &= ~0x01;
            }
//        }
		Save_flag();
		Modify_A_READ = 0;
		Modify_C_READ = 0;
		Modify_A_ACT = 0;
		Modify_B_READ = 0;
		Modify_D_READ = 0;
		Modify_B_ACT = 0;
		
    }
	
	if(step == 5)
    {
        Modify_A_READ += Yconvert;//测量电压值		
    }else if(step == 6){
		Modify_A_READ += Yconvert;
		Modify_A_READ = Modify_A_READ/2;
		Modify_A_ACT = 0x0A;//读取低段
	}else if(step == 7){
		Modify_B_READ += Yconvert;
	}else if(step == 8){
        vu16 var16;
        vu32 var32a;
        vu32 var32b;
        
        vu16 var16a;
        vu32 var32c;
        vu32 var32d;
        Modify_B_READ +=Yconvert;//测量电压值
		Modify_B_READ = Modify_B_READ/2;

        Modify_B_ACT = 0x01D6;//读取高段
//        if(flag_OverV==1)//只有当有数据写入时才能将校准数据写入FLASH
//        {
            var32a = Modify_B_ACT;
            var32a = var32a - Modify_A_ACT;
            var32a = var32a << 12;
            var16 = Modify_B_READ - Modify_A_READ;
            var32a = var32a / var16;
            YCOR = var32a;
            var32a=0;
            var32a = Modify_B_ACT;
            var32a = var32a << 12;
            var32b = Modify_B_READ;
            var32b = var32b * YCOR;
            if (var32a < var32b)
            {
                var32b = var32b - var32a;
                YOffset = var32b;
                Polar2 |= 0x01;
            }
            else 
            {
                var32a = var32a - var32b;
                YOffset = var32a;
                Polar2 &= ~0x01;
            }
//        }
		Save_flag();
		Modify_A_READ = 0;
		Modify_C_READ = 0;
		Modify_A_ACT = 0;
		Modify_B_READ = 0;
		Modify_D_READ = 0;
		Modify_B_ACT = 0;
		
    }
}