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
u8 touchflag;
u8 press;

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
	static u8 touchdelay;
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
		Delay(1000);
		Yconvert = ADC_ConvertedValue[1]/50;
//		if(j<5)
//		{
//			j++;
//		}else{
//			j = 0;
//		}
//		Yconvert = (ADY[1] + ADY[3])/2/50;
	}else{
		set_xx();
		Delay(1000);
		Xconvert = ADC_ConvertedValue[0]/50;
//		if(k<5)
//		{
//			k++;
//		}else{
//			k = 0;
//		}
//		Xconvert = (ADX[1] + ADX[3])/2/50;		
	}
	i++;
	
	
	
	if(touchflag == 1)
	{
		if(touchdelay > 5)
		{
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
		}else{
			touchdelay++;
		}
	}
	
	
	if(Xconvert <= 4 || Yconvert <= 3)
	{
		press = 0;
		touchflag = 0;
		touchdelay = 0;
	}else{
		touchflag = 1;
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

	if(touchflag == 1 && press == 0)
	{
		switch(page_flag)
		{
			case display:
			{
				if(x >= TCTYPEX1 && x <= TCTYPEX2 && y >= TCTYPEY1 && y <= TCTYPEY2)
				{
					DrawMenu();//重绘菜单栏
					LCD_SetTextColor(LCD_COLOR_YELLOW);
					LCD_SetBackColor(LCD_COLOR_BLACK);
					LCD_DisplayStringLine(450,35,"TC-T");
					LCD_DisplayStringLine(450,157,"TC-K");
					LCD_DisplayStringLine(450,295,"TC-J");
					LCD_DisplayStringLine(450,420,"TC-N");
					if(LANG == chs)
					{
						LCD_DisplayStringLine(450,550,"更多");
					}else{
						LCD_DisplayStringLine(450,550,"MORE");
					}
					if(LANG == chs)
					{
						DrawInstruction("热电偶型号选择");
					}else{
						DrawInstruction("Thermocouple model select");
					}
					op_flag = type_1;
					press = 1;
				}
				
				if(x >= MENU1X1 && x <= MENU1X2 && y >= MENUY1 && y <= MENUY2)
				{
					switch(op_flag)
					{
						case type_1:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);					
							LCD_DisplayStringLine(47,100,"TC-T");
							op_flag = home_type;
							TCTYPE = TCT;
							Save_flag();
							}break;
						case type_2:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-E");
							op_flag = home_type;
							TCTYPE = TCE;
							Save_flag();					
						}break;
						case type_3:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"PT100");
							op_flag = home_type;
							TCTYPE = PT100;
							Save_flag();
						}break;
						default:
						{
							page_home();
						}break;
					}
					press = 1;
				}
				if(x >= MENU2X1 && x <= MENU2X2 && y >= MENUY1 && y <= MENUY2)
				{
					switch(op_flag)
					{
						case type_1:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-K");
							op_flag = home_type;
							TCTYPE = TCK;
							Save_flag();
						}break;
						case type_2:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-S");
							op_flag = home_type;
							TCTYPE = TCS;
							Save_flag();
						}break;
						default:
						{
							page_graph();
						}break;
					}
					press = 1;
				}
				if(x >= MENU3X1 && x <= MENU3X2 && y >= MENUY1 && y <= MENUY2)
				{
					switch(op_flag)
					{
						case type_1:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-J");
							op_flag = home_type;
							TCTYPE = TCJ;
							Save_flag();
						}break;
						case type_2:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-R");
							op_flag = home_type;
							TCTYPE = TCR;
							Save_flag();
						}break;
						default:
						{
							page_sys();
						}break;
					}
					press = 1;
				}
				if(x >= MENU4X1 && x <= MENU4X2 && y >= MENUY1 && y <= MENUY2)
				{
					switch(op_flag)
					{
						case type_1:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-N");
							op_flag = home_type;
							TCTYPE = TCN;
							Save_flag();
						}break;
						case type_2:
						{
							DrawMenu();//重绘菜单栏
							Drawhomemenu();
							LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BLUE);
							LCD_DrawFullRect(96,47,100,31);
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BACK);
							LCD_DisplayStringLine(47,100,"TC-B");
							op_flag = home_type;
							TCTYPE = TCB;
							Save_flag();
						}break;
						default:
						{
							page_set();
						}break;
					}
					press = 1;
				}
				if(x >= MENU5X1 && x <= MENU5X2 && y >= MENUY1 && y <= MENUY2)
				{
					switch(op_flag)
					{
						case type_1:
						{
							DrawMenu();//重绘菜单栏
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BLACK);
							LCD_DisplayStringLine(450,35,"TC-E");
							LCD_DisplayStringLine(450,157,"TC-S");
							LCD_DisplayStringLine(450,295,"TC-R");
							LCD_DisplayStringLine(450,420,"TC-B");
							if(LANG == chs)
							{
								LCD_DisplayStringLine(450,550,"更多");
							}else{
								LCD_DisplayStringLine(450,550,"MORE");
							}
							op_flag = type_2;						
						}break;
						case type_2:
						{
							DrawMenu();//重绘菜单栏
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BLACK);
							LCD_DisplayStringLine(450,35,"PT100");
							if(LANG == chs)
							{
								LCD_DisplayStringLine(450,550,"更多");
							}else{
								LCD_DisplayStringLine(450,550,"MORE");
							}
							op_flag = type_3;						
						}break;
						case type_3:
						{
							DrawMenu();//重绘菜单栏
							LCD_SetTextColor(LCD_COLOR_YELLOW);
							LCD_SetBackColor(LCD_COLOR_BLACK);
							LCD_DisplayStringLine(450,35,"TC-T");
							LCD_DisplayStringLine(450,157,"TC-K");
							LCD_DisplayStringLine(450,295,"TC-J");
							LCD_DisplayStringLine(450,420,"TC-N");
							if(LANG == chs)
							{
								LCD_DisplayStringLine(450,550,"更多");
							}else{
								LCD_DisplayStringLine(450,550,"MORE");
							}
							op_flag = type_1;						
						}break;						
					}
					press = 1;
				}
				if(FONT == big)
				{
					if(x >= BIGCHLX1 && x <= BIGCHLX2 && y >= BIGCH1Y1 && y <= BIGCH1Y2)
					{
						
						if(ch_page == page1)
						{
							if(CH1_SW == ch_on)
							{
								CH1_SW = ch_off;
							}else{
								CH1_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH9_SW == ch_on)
							{
								CH9_SW = ch_off;
							}else{
								CH9_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH17_SW == ch_on)
							{
								CH17_SW = ch_off;
							}else{
								CH17_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH25_SW == ch_on)
							{
								CH25_SW = ch_off;
							}else{
								CH25_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH33_SW == ch_on)
							{
								CH33_SW = ch_off;
							}else{
								CH33_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHLX1 && x <= BIGCHLX2 && y >= BIGCH2Y1 && y <= BIGCH2Y2){
						if(ch_page == page1)
						{
							if(CH2_SW == ch_on)
							{
								CH2_SW = ch_off;
							}else{
								CH2_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH10_SW == ch_on)
							{
								CH10_SW = ch_off;
							}else{
								CH10_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH18_SW == ch_on)
							{
								CH18_SW = ch_off;
							}else{
								CH18_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH26_SW == ch_on)
							{
								CH26_SW = ch_off;
							}else{
								CH26_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH34_SW == ch_on)
							{
								CH34_SW = ch_off;
							}else{
								CH34_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHLX1 && x <= BIGCHLX2 && y >= BIGCH3Y1 && y <= BIGCH3Y2){
						if(ch_page == page1)
						{
							if(CH3_SW == ch_on)
							{
								CH3_SW = ch_off;
							}else{
								CH3_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH11_SW == ch_on)
							{
								CH11_SW = ch_off;
							}else{
								CH11_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH19_SW == ch_on)
							{
								CH19_SW = ch_off;
							}else{
								CH19_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH27_SW == ch_on)
							{
								CH27_SW = ch_off;
							}else{
								CH27_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH35_SW == ch_on)
							{
								CH35_SW = ch_off;
							}else{
								CH35_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHLX1 && x <= BIGCHLX2 && y >= BIGCH4Y1 && y <= BIGCH4Y2){
						if(ch_page == page1)
						{
							if(CH4_SW == ch_on)
							{
								CH4_SW = ch_off;
							}else{
								CH4_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH12_SW == ch_on)
							{
								CH12_SW = ch_off;
							}else{
								CH12_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH20_SW == ch_on)
							{
								CH20_SW = ch_off;
							}else{
								CH20_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH28_SW == ch_on)
							{
								CH28_SW = ch_off;
							}else{
								CH28_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH36_SW == ch_on)
							{
								CH36_SW = ch_off;
							}else{
								CH36_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHRX1 && x <= BIGCHRX2 && y >= BIGCH1Y1 && y <= BIGCH1Y2)
					{
						if(ch_page == page1)
						{
							if(CH5_SW == ch_on)
							{
								CH5_SW = ch_off;
							}else{
								CH5_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH13_SW == ch_on)
							{
								CH13_SW = ch_off;
							}else{
								CH13_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH21_SW == ch_on)
							{
								CH21_SW = ch_off;
							}else{
								CH21_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH29_SW == ch_on)
							{
								CH29_SW = ch_off;
							}else{
								CH29_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH37_SW == ch_on)
							{
								CH37_SW = ch_off;
							}else{
								CH37_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHRX1 && x <= BIGCHRX2 && y >= BIGCH2Y1 && y <= BIGCH2Y2){
						if(ch_page == page1)
						{
							if(CH6_SW == ch_on)
							{
								CH6_SW = ch_off;
							}else{
								CH6_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH14_SW == ch_on)
							{
								CH14_SW = ch_off;
							}else{
								CH14_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH22_SW == ch_on)
							{
								CH22_SW = ch_off;
							}else{
								CH22_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH30_SW == ch_on)
							{
								CH30_SW = ch_off;
							}else{
								CH30_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH38_SW == ch_on)
							{
								CH38_SW = ch_off;
							}else{
								CH38_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHRX1 && x <= BIGCHRX2 && y >= BIGCH3Y1 && y <= BIGCH3Y2){
						if(ch_page == page1)
						{
							if(CH7_SW == ch_on)
							{
								CH7_SW = ch_off;
							}else{
								CH7_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH15_SW == ch_on)
							{
								CH15_SW = ch_off;
							}else{
								CH15_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH23_SW == ch_on)
							{
								CH23_SW = ch_off;
							}else{
								CH23_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH31_SW == ch_on)
							{
								CH31_SW = ch_off;
							}else{
								CH31_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH39_SW == ch_on)
							{
								CH39_SW = ch_off;
							}else{
								CH39_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}else if(x >= BIGCHRX1 && x <= BIGCHRX2 && y >= BIGCH4Y1 && y <= BIGCH4Y2){
						if(ch_page == page1)
						{
							if(CH8_SW == ch_on)
							{
								CH8_SW = ch_off;
							}else{
								CH8_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH16_SW == ch_on)
							{
								CH16_SW = ch_off;
							}else{
								CH16_SW = ch_on;
							}
						}else if(ch_page == page3){
							if(CH24_SW == ch_on)
							{
								CH24_SW = ch_off;
							}else{
								CH24_SW = ch_on;
							}
						}else if(ch_page == page4){
							if(CH32_SW == ch_on)
							{
								CH32_SW = ch_off;
							}else{
								CH32_SW = ch_on;
							}
						}else if(ch_page == page5){
							if(CH40_SW == ch_on)
							{
								CH40_SW = ch_off;
							}else{
								CH40_SW = ch_on;
							}
						}
						ChannelSwtichBig();
					}
				}else if(FONT == middle){
					if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH1Y1 && y <= MIDCH1Y2)
					{
						
						if(ch_page == page1)
						{
							if(CH1_SW == ch_on)
							{
								CH1_SW = ch_off;
							}else{
								CH1_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH25_SW == ch_on)
							{
								CH25_SW = ch_off;
							}else{
								CH25_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH2Y1 && y <= MIDCH2Y2){
						if(ch_page == page1)
						{
							if(CH2_SW == ch_on)
							{
								CH2_SW = ch_off;
							}else{
								CH2_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH26_SW == ch_on)
							{
								CH26_SW = ch_off;
							}else{
								CH26_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH3Y1 && y <= MIDCH3Y2){
						if(ch_page == page1)
						{
							if(CH3_SW == ch_on)
							{
								CH3_SW = ch_off;
							}else{
								CH3_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH27_SW == ch_on)
							{
								CH27_SW = ch_off;
							}else{
								CH27_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH4Y1 && y <= MIDCH4Y2){
						if(ch_page == page1)
						{
							if(CH4_SW == ch_on)
							{
								CH4_SW = ch_off;
							}else{
								CH4_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH28_SW == ch_on)
							{
								CH28_SW = ch_off;
							}else{
								CH28_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH5Y1 && y <= MIDCH5Y2)
					{
						if(ch_page == page1)
						{
							if(CH5_SW == ch_on)
							{
								CH5_SW = ch_off;
							}else{
								CH5_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH29_SW == ch_on)
							{
								CH29_SW = ch_off;
							}else{
								CH29_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH6Y1 && y <= MIDCH6Y2){
						if(ch_page == page1)
						{
							if(CH6_SW == ch_on)
							{
								CH6_SW = ch_off;
							}else{
								CH6_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH30_SW == ch_on)
							{
								CH30_SW = ch_off;
							}else{
								CH30_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH7Y1 && y <= MIDCH7Y2){
						if(ch_page == page1)
						{
							if(CH7_SW == ch_on)
							{
								CH7_SW = ch_off;
							}else{
								CH7_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH31_SW == ch_on)
							{
								CH31_SW = ch_off;
							}else{
								CH31_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHLX1 && x <= MIDCHLX2 && y >= MIDCH8Y1 && y <= MIDCH8Y2){
						if(ch_page == page1)
						{
							if(CH8_SW == ch_on)
							{
								CH8_SW = ch_off;
							}else{
								CH8_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH32_SW == ch_on)
							{
								CH32_SW = ch_off;
							}else{
								CH32_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH1Y1 && y <= MIDCH1Y2)
					{
						
						if(ch_page == page1)
						{
							if(CH9_SW == ch_on)
							{
								CH9_SW = ch_off;
							}else{
								CH9_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH33_SW == ch_on)
							{
								CH33_SW = ch_off;
							}else{
								CH33_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH2Y1 && y <= MIDCH2Y2){
						if(ch_page == page1)
						{
							if(CH10_SW == ch_on)
							{
								CH10_SW = ch_off;
							}else{
								CH10_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH34_SW == ch_on)
							{
								CH34_SW = ch_off;
							}else{
								CH34_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH3Y1 && y <= MIDCH3Y2){
						if(ch_page == page1)
						{
							if(CH11_SW == ch_on)
							{
								CH11_SW = ch_off;
							}else{
								CH11_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH35_SW == ch_on)
							{
								CH35_SW = ch_off;
							}else{
								CH35_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH4Y1 && y <= MIDCH4Y2){
						if(ch_page == page1)
						{
							if(CH12_SW == ch_on)
							{
								CH12_SW = ch_off;
							}else{
								CH12_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH36_SW == ch_on)
							{
								CH36_SW = ch_off;
							}else{
								CH36_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH5Y1 && y <= MIDCH5Y2)
					{
						if(ch_page == page1)
						{
							if(CH13_SW == ch_on)
							{
								CH13_SW = ch_off;
							}else{
								CH13_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH37_SW == ch_on)
							{
								CH37_SW = ch_off;
							}else{
								CH37_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH6Y1 && y <= MIDCH6Y2){
						if(ch_page == page1)
						{
							if(CH14_SW == ch_on)
							{
								CH14_SW = ch_off;
							}else{
								CH14_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH38_SW == ch_on)
							{
								CH38_SW = ch_off;
							}else{
								CH38_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH7Y1 && y <= MIDCH7Y2){
						if(ch_page == page1)
						{
							if(CH15_SW == ch_on)
							{
								CH15_SW = ch_off;
							}else{
								CH15_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH39_SW == ch_on)
							{
								CH39_SW = ch_off;
							}else{
								CH39_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHMX1 && x <= MIDCHMX2 && y >= MIDCH8Y1 && y <= MIDCH8Y2){
						if(ch_page == page1)
						{
							if(CH16_SW == ch_on)
							{
								CH16_SW = ch_off;
							}else{
								CH16_SW = ch_on;
							}
						}else if(ch_page == page2){
							if(CH40_SW == ch_on)
							{
								CH40_SW = ch_off;
							}else{
								CH40_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH1Y1 && y <= MIDCH1Y2)
					{
						
						if(ch_page == page1)
						{
							if(CH17_SW == ch_on)
							{
								CH17_SW = ch_off;
							}else{
								CH17_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH2Y1 && y <= MIDCH2Y2){
						if(ch_page == page1)
						{
							if(CH18_SW == ch_on)
							{
								CH18_SW = ch_off;
							}else{
								CH18_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH3Y1 && y <= MIDCH3Y2){
						if(ch_page == page1)
						{
							if(CH19_SW == ch_on)
							{
								CH19_SW = ch_off;
							}else{
								CH19_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH4Y1 && y <= MIDCH4Y2){
						if(ch_page == page1)
						{
							if(CH20_SW == ch_on)
							{
								CH20_SW = ch_off;
							}else{
								CH20_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH5Y1 && y <= MIDCH5Y2)
					{
						if(ch_page == page1)
						{
							if(CH21_SW == ch_on)
							{
								CH21_SW = ch_off;
							}else{
								CH21_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH6Y1 && y <= MIDCH6Y2){
						if(ch_page == page1)
						{
							if(CH22_SW == ch_on)
							{
								CH22_SW = ch_off;
							}else{
								CH22_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH7Y1 && y <= MIDCH7Y2){
						if(ch_page == page1)
						{
							if(CH23_SW == ch_on)
							{
								CH23_SW = ch_off;
							}else{
								CH23_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}else if(x >= MIDCHRX1 && x <= MIDCHRX2 && y >= MIDCH8Y1 && y <= MIDCH8Y2){
						if(ch_page == page1)
						{
							if(CH24_SW == ch_on)
							{
								CH24_SW = ch_off;
							}else{
								CH24_SW = ch_on;
							}
						}
						ChannelSwtichMid();
					}
				}else if(FONT == small){
					if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH1Y1 && y <= SMLCH1Y2)
					{
						
						if(CH1_SW == ch_on)
						{
							CH1_SW = ch_off;
						}else{
							CH1_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH2Y1 && y <= SMLCH2Y2){
						if(CH2_SW == ch_on)
						{
							CH2_SW = ch_off;
						}else{
							CH2_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH3Y1 && y <= SMLCH3Y2){
						if(CH3_SW == ch_on)
						{
							CH3_SW = ch_off;
						}else{
							CH3_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH4Y1 && y <= SMLCH4Y2){
						if(CH4_SW == ch_on)
						{
							CH4_SW = ch_off;
						}else{
							CH4_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH5Y1 && y <= SMLCH5Y2)
					{
						if(CH5_SW == ch_on)
						{
							CH5_SW = ch_off;
						}else{
							CH5_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH6Y1 && y <= SMLCH6Y2){
						if(CH6_SW == ch_on)
						{
							CH6_SW = ch_off;
						}else{
							CH6_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX1 && x <= SMLCHLX2 && y >= SMLCH7Y1 && y <= SMLCH7Y2){
						if(CH7_SW == ch_on)
						{
							CH7_SW = ch_off;
						}else{
							CH7_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHLX2 && x <= SMLCH7Y1 && y >= SMLCH8Y1 && y <= SMLCH8Y2){
						if(CH8_SW == ch_on)
						{
							CH8_SW = ch_off;
						}else{
							CH8_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH1Y1 && y <= SMLCH1Y2)
					{
						
						if(CH9_SW == ch_on)
						{
							CH9_SW = ch_off;
						}else{
							CH9_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH2Y1 && y <= SMLCH2Y2){
						if(CH10_SW == ch_on)
						{
							CH10_SW = ch_off;
						}else{
							CH10_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH3Y1 && y <= SMLCH3Y2){
						if(CH11_SW == ch_on)
						{
							CH11_SW = ch_off;
						}else{
							CH11_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH4Y1 && y <= SMLCH4Y2){
						if(CH12_SW == ch_on)
						{
							CH12_SW = ch_off;
						}else{
							CH12_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH5Y1 && y <= SMLCH5Y2)
					{
						if(CH13_SW == ch_on)
						{
							CH13_SW = ch_off;
						}else{
							CH13_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH6Y1 && y <= SMLCH6Y2){
						if(CH14_SW == ch_on)
						{
							CH14_SW = ch_off;
						}else{
							CH14_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH7Y1 && y <= SMLCH7Y2){
						if(CH15_SW == ch_on)
						{
							CH15_SW = ch_off;
						}else{
							CH15_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM1X1 && x <= SMLCHM1X2 && y >= SMLCH8Y1 && y <= SMLCH8Y2){
						if(CH16_SW == ch_on)
						{
							CH16_SW = ch_off;
						}else{
							CH16_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH1Y1 && y <= SMLCH1Y2)
					{
						
						if(CH17_SW == ch_on)
						{
							CH17_SW = ch_off;
						}else{
							CH17_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH2Y1 && y <= SMLCH2Y2){
						if(CH18_SW == ch_on)
						{
							CH18_SW = ch_off;
						}else{
							CH18_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH3Y1 && y <= SMLCH3Y2){
						if(CH19_SW == ch_on)
						{
							CH19_SW = ch_off;
						}else{
							CH19_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH4Y1 && y <= SMLCH4Y2){
						if(CH20_SW == ch_on)
						{
							CH20_SW = ch_off;
						}else{
							CH20_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH5Y1 && y <= SMLCH5Y2)
					{
						if(CH21_SW == ch_on)
						{
							CH21_SW = ch_off;
						}else{
							CH21_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH6Y1 && y <= SMLCH6Y2){
						if(CH22_SW == ch_on)
						{
							CH22_SW = ch_off;
						}else{
							CH22_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH7Y1 && y <= SMLCH7Y2){
						if(CH23_SW == ch_on)
						{
							CH23_SW = ch_off;
						}else{
							CH23_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM2X1 && x <= SMLCHM2X2 && y >= SMLCH8Y1 && y <= SMLCH8Y2){
						if(CH24_SW == ch_on)
						{
							CH24_SW = ch_off;
						}else{
							CH24_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH1Y1 && y <= SMLCH1Y2)
					{
						
						if(CH25_SW == ch_on)
						{
							CH25_SW = ch_off;
						}else{
							CH25_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH2Y1 && y <= SMLCH2Y2){
						if(CH26_SW == ch_on)
						{
							CH26_SW = ch_off;
						}else{
							CH26_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH3Y1 && y <= SMLCH3Y2){
						if(CH27_SW == ch_on)
						{
							CH27_SW = ch_off;
						}else{
							CH27_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH4Y1 && y <= SMLCH4Y2){
						if(CH28_SW == ch_on)
						{
							CH28_SW = ch_off;
						}else{
							CH28_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH5Y1 && y <= SMLCH5Y2)
					{
						if(CH29_SW == ch_on)
						{
							CH29_SW = ch_off;
						}else{
							CH29_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH6Y1 && y <= SMLCH6Y2){
						if(CH30_SW == ch_on)
						{
							CH30_SW = ch_off;
						}else{
							CH30_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH7Y1 && y <= SMLCH7Y2){
						if(CH31_SW == ch_on)
						{
							CH31_SW = ch_off;
						}else{
							CH31_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHM3X1 && x <= SMLCHM3X2 && y >= SMLCH8Y1 && y <= SMLCH8Y2){
						if(CH32_SW == ch_on)
						{
							CH32_SW = ch_off;
						}else{
							CH32_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH1Y1 && y <= SMLCH1Y2)
					{
						
						if(CH33_SW == ch_on)
						{
							CH33_SW = ch_off;
						}else{
							CH33_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH2Y1 && y <= SMLCH2Y2){
						if(CH34_SW == ch_on)
						{
							CH34_SW = ch_off;
						}else{
							CH34_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH3Y1 && y <= SMLCH3Y2){
						if(CH35_SW == ch_on)
						{
							CH35_SW = ch_off;
						}else{
							CH35_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH4Y1 && y <= SMLCH4Y2){
						if(CH36_SW == ch_on)
						{
							CH36_SW = ch_off;
						}else{
							CH36_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH5Y1 && y <= SMLCH5Y2)
					{
						if(CH37_SW == ch_on)
						{
							CH37_SW = ch_off;
						}else{
							CH37_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH6Y1 && y <= SMLCH6Y2){
						if(CH38_SW == ch_on)
						{
							CH38_SW = ch_off;
						}else{
							CH38_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH7Y1 && y <= SMLCH7Y2){
						if(CH39_SW == ch_on)
						{
							CH39_SW = ch_off;
						}else{
							CH39_SW = ch_on;
						}
						ChannelSwtichSml();
					}else if(x >= SMLCHRX1 && x <= SMLCHRX2 && y >= SMLCH8Y1 && y <= SMLCH8Y2){
						if(CH40_SW == ch_on)
						{
							CH40_SW = ch_off;
						}else{
							CH40_SW = ch_on;
						}
						ChannelSwtichSml();
					}
				}
				
			}break;
		}
		

	}
}

void DrawTC(void)
{
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_SetBackColor(LCD_COLOR_BACK);
	
	
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
	}	
}


void ChannelSwtichBig(void)
{
	if(ch_page == page1)
	{
		if(CH1_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
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
			LCD_DisplayStringLine_48(170,10,"002");
		}else if(CH2_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,10,"002");
		}
		
		if(CH3_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,10,"003");
		}else if(CH3_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,10,"003");
		}
		
		if(CH4_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,10,"004");
		}else if(CH4_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,10,"004");
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
			LCD_DisplayStringLine_48(170,330,"006");
		}else if(CH6_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,330,"006");
		}
		
		if(CH7_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,330,"007");
		}else if(CH7_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,330,"007");
		}
		
		if(CH8_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,330,"008");
		}else if(CH8_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,330,"008");
		}
	}else if(ch_page == page2){
		if(CH9_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,10,"009");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,10,"009");
		}
		
		if(CH10_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,10,"010");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,10,"010");
		}
		
		if(CH11_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,10,"011");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,10,"011");
		}
		
		if(CH12_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,10,"012");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,10,"012");
		}
		
		if(CH13_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,330,"013");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,330,"013");
		}
		
		if(CH14_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,330,"014");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,330,"014");
		}
		
		if(CH15_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,330,"015");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,330,"015");
		}
		
		if(CH16_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,330,"016");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,330,"016");
		}
	}else if(ch_page == page3){
		if(CH17_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,10,"017");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,10,"017");
		}
		
		if(CH18_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,10,"018");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,10,"018");
		}
		
		if(CH19_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,10,"019");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,10,"019");
		}
		
		if(CH20_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,10,"020");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,10,"020");
		}
		
		if(CH21_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,330,"021");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,330,"021");
		}
		
		if(CH22_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,330,"022");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,330,"022");
		}
		
		if(CH23_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,330,"023");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,330,"023");
		}
		
		if(CH24_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,330,"024");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,330,"024");
		}
	}else if(ch_page == page4){
		if(CH25_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,10,"025");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,10,"025");
		}
		
		if(CH26_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,10,"026");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,10,"026");
		}
		
		if(CH27_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,10,"027");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,10,"027");
		}
		
		if(CH28_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,10,"028");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,10,"028");
		}
		
		if(CH29_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,330,"029");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,330,"029");
		}
	
		if(CH30_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,330,"030");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,330,"030");
		}
		
		if(CH31_SW == ch_off){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,330,"031");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,330,"031");
		}
		
		if(CH32_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,330,"032");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,330,"032");
		}
	}else if(ch_page == page5){
		if(CH33_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,10,"033");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,10,"033");
		}
		
		if(CH34_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,10,"034");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,10,"034");
		}
		
		if(CH35_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,10,"035");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,10,"035");
		}
			
		if(CH36_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,10,"036");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,10,"036");
		}
			
		if(CH37_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(100,330,"037");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(100,330,"037");
		}
			
		if(CH38_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(170,330,"038");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(170,330,"038");
		}
			
		if(CH39_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(240,330,"039");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(240,330,"039");
		}
			
		if(CH40_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			LCD_DisplayStringLine_48(310,330,"040");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			LCD_DisplayStringLine_48(310,330,"040");
		}
	}
	press = 1;
	if(LANG == chs)
	{
		DrawInstruction("通道设置");
	}else{
		DrawInstruction("Channel set");
	}
	Save_flag();
}

void ChannelSwtichMid(void)
{
	if(ch_page == page1)
	{
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
		
		if(CH9_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(100,210,"009");
		}else if(CH9_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(100,210,"009");
		}
		
		if(CH10_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(140,210,"010");
		}else if(CH10_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(140,210,"010");
		}
		
		if(CH11_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(180,210,"011");
		}else if(CH11_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(180,210,"011");
		}
		
		if(CH12_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(220,210,"012");
		}else if(CH12_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(220,210,"012");
		}
		
		if(CH13_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(260,210,"013");
		}else if(CH13_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(260,210,"013");
		}
		
		if(CH14_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(300,210,"014");
		}else if(CH14_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(300,210,"014");
		}
		
		if(CH15_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(340,210,"015");
		}else if(CH15_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(340,210,"015");
		}
		
		if(CH16_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(380,210,"016");
		}else if(CH16_SW == ch_on){
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(380,210,"016");
		}
		
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
	}else if(ch_page == page2){
		if(CH25_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(100,10,"025");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(100,10,"025");
		}
		
		if(CH26_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(140,10,"026");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(140,10,"026");
		}
		
		if(CH27_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(180,10,"027");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(180,10,"027");
		}
		
		if(CH28_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(220,10,"028");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(220,10,"028");
		}
		
		if(CH29_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(260,10,"029");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(260,10,"029");
		}
		
		if(CH30_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(300,10,"030");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(300,10,"030");
		}
		
		if(CH31_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(340,10,"031");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(340,10,"031");
		}
		
		if(CH32_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(380,10,"032");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(380,10,"032");
		}
		
		if(CH33_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(100,210,"033");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(100,210,"033");
		}
		
		if(CH34_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(140,210,"034");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(140,210,"034");
		}
		
		if(CH35_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(180,210,"035");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(180,210,"035");
		}
		
		if(CH36_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(220,210,"036");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(220,210,"036");
		}
		
		if(CH37_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(260,210,"037");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(260,210,"037");
		}
		
		if(CH38_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(300,210,"038");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(300,210,"038");
		}
		
		if(CH39_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(340,210,"039");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(340,210,"039");
		}
		
		if(CH40_SW == ch_off)
		{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_LIGHTGREY);
			DISP_CNL_M(380,210,"040");
		}else{
			LCD_SetBackColor(LCD_COLOR_BACK);
			LCD_SetTextColor(LCD_COLOR_HLT);
			DISP_CNL_M(380,210,"040");
		}
	}
	
	press = 1;
	if(LANG == chs)
	{
		DrawInstruction("通道设置");
	}else{
		DrawInstruction("Channel set");
	}
	Save_flag();
}

void ChannelSwtichSml(void)
{
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
	press = 1;
	if(LANG == chs)
	{
		DrawInstruction("通道设置");
	}else{
		DrawInstruction("Channel set");
	}
	Save_flag();
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