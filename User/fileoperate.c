#include "stm32f4xx.h"
#include "ff.h"
#include "jk508.h"
#include "./lcd/bsp_lcd.h"
#include "sdio/bsp_sdio_sd.h"


//char filename[20];
//char foldername[20];
//FILINFO hisinfo;
//DIR dir;
//FATFS fs;													/* FatFs文件系统对象 */
//FIL fnew;													/* 文件对象 */
//FRESULT res_sd;                /* 文件操作结果 */
//UINT fnum;            					/* 文件成功读写数量 */
//u8 WriteBuffer[512];        /* 读缓冲区 */
////u8 ReadBuffer[512];        /* 读缓冲区 */
//char Dirname[MAXDIR][13];
//char Filelist[MAXFILE][13];
//u8 maxhispage,dirflag,maxfilepage;
u16 foldernum;
//BYTE WriteBuffer[] =              /* 写缓冲区*/
BLOCK_REC BlockNum;
SAVE_INDEX HisIndex;
SAVE_SD SaveBuffer;
SAVE_SD ReadBuffer;
uint64_t sizewatch;
u8 infoflag;
u8 indexflag;
char scomp1[8];
char scomp2[8];
char indexname[20];
////加载SD卡
//void Mount_SD(void)
//{
//	res_sd = f_mount(&fs,"0:",1);
//	if(res_sd == FR_NO_FILESYSTEM)
//	{
//		DrawInstruction("加载文件系统失败，请重新格式化");
//	}else{
//		DrawInstruction("加载文件系统成功");
//	}
//}

//////////////////////////////////////////////////////////////////
void Disp_Search_Info(u8 info)
{
	LCD_SetColors(LCD_COLOR_GREY,LCD_COLOR_GREY);
	LCD_DrawFullRect(269-30,199,160,34);
	LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_WHITE);
	LCD_DrawFullRect(270-30,200,162,32);
	LCD_SetColors(LCD_COLOR_RED,LCD_COLOR_WHITE);
	if(info == 0)
	{		
		LCD_DisplayStringLine(203,280-30,"正在搜索");
		LCD_DisplayStringLine(200,380-30,"...");
	}else if(info == 1){
		LCD_DisplayStringLine(203,280,"无结果");
	}
}


void Read_Block_Rec(void)
{
	SD_ReadBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
}

void Write_Block_Rec(void)
{
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
}

void Write_His_Data(void)
{
	u16 i;
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
	SD_WriteMultiBlocks((uint8_t *)&SaveBuffer,61952+sizeof(SaveBuffer)*BlockNum.Num[0],512,sizeof(SaveBuffer)/512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);	
//	if(indexflag == 1)
//	{
//		strcpy(HisIndex.Date[BlockNum.Num[1]%40],SaveBuffer.Time[495]);
//		HisIndex.Index[BlockNum.Num[1]%40] = BlockNum.Num[0];
////		for(i=0;i<8;i++)
////		{
////			HisIndex.Date[BlockNum.Num[1]][i]=SaveBuffer.Time[0][i]; 
////		}
//		SD_WriteBlock((uint8_t *)&HisIndex,512+512*(BlockNum.Num[1]/40),512);
//		SD_WaitWriteOperation();	
//		while(SD_GetStatus() != SD_TRANSFER_OK);
//		BlockNum.Num[1] ++;//目录计数
//		indexflag = 0;
//	}
	BlockNum.Num[0]++;
	if(BlockNum.Num[1] > 4800)
	{
		BlockNum.Num[0] = 0;
	}
	if(BlockNum.Num[0] > SD_MAX_BLOCK)
	{
		BlockNum.Num[0] = 0;
	}
	memset(SaveBuffer.Time,0,sizeof(SaveBuffer.Time));
	memset(SaveBuffer.Temp,0,sizeof(SaveBuffer.Temp));
}

void Write_His_Data_Man(void)
{
	u16 i;
	SD_WriteBlock((uint8_t *)&BlockNum,0,512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);
	SD_WriteMultiBlocks((uint8_t *)&SaveBuffer,61952+sizeof(SaveBuffer)*BlockNum.Num[0],512,sizeof(SaveBuffer)/512);
	SD_WaitWriteOperation();	
	while(SD_GetStatus() != SD_TRANSFER_OK);	
	if(indexflag == 1)
	{
		strcpy(HisIndex.Date[BlockNum.Num[1]%40],SaveBuffer.Time[count - 1]);
		HisIndex.Index[BlockNum.Num[1]%40] = BlockNum.Num[0];
//		for(i=0;i<8;i++)
//		{
//			HisIndex.Date[BlockNum.Num[1]][i]=SaveBuffer.Time[0][i]; 
//		}
		SD_WriteBlock((uint8_t *)&HisIndex,512+512*(BlockNum.Num[1]/40),512);
		SD_WaitWriteOperation();	
		while(SD_GetStatus() != SD_TRANSFER_OK);
		BlockNum.Num[1] ++;
		indexflag = 0;
	}
	BlockNum.Num[0]++;
	if(BlockNum.Num[1] > 4800)
	{
		BlockNum.Num[0] = 0;
	}
	if(BlockNum.Num[0] > SD_MAX_BLOCK)
	{
		BlockNum.Num[0] = 0;
	}
	memset(SaveBuffer.Time,0,sizeof(SaveBuffer.Time));
	memset(SaveBuffer.Temp,0,sizeof(SaveBuffer.Temp));
}

void Read_Index(u32 indexnum)
{
	SD_ReadBlock((uint8_t *)&HisIndex,512+(512*indexnum),512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
}

void Read_His_Data(u32 block)
{	
	sizewatch =(sizeof(SaveBuffer)*block);
	SD_ReadMultiBlocks((uint8_t *)&ReadBuffer,61952+(sizeof(SaveBuffer)*block),512,sizeof(SaveBuffer)/512);
	SD_WaitWriteOperation();
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
}

void Draw_His_Index(u8 page)
{
	u16 i,j;
	char buf[14],num[5];
	Read_Index(page/4);
	
	LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
	LCD_DrawFullRect(10,40,400,400);
	if(page < BlockNum.Num[1]/10)
	{
		foldernum = 10;
	}else{
		foldernum = BlockNum.Num[1]%10;
	}
	hispage = HisIndex.Index[op_flag+10*page-1]+1;
	hispagestart = hispage;
	hispageend = HisIndex.Index[op_flag+10*page];
	for(i=0;i<foldernum;i++)
	{
		
		sprintf(indexname,"%0.2d%0.2d-%0.2d-%0.2d-%0.2d:%0.2d:%0.2d",	  HisIndex.Date[i+10*page][0],
																		  HisIndex.Date[i+10*page][1],
																		  HisIndex.Date[i+10*page][2],
																		  HisIndex.Date[i+10*page][3],
																		  HisIndex.Date[i+10*page][4],
																		  HisIndex.Date[i+10*page][5],
																		  HisIndex.Date[i+10*page][6]);
		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
		sprintf(num,"%d",i+1);
		LCD_DisplayStringLine(40+i*35,10,(uint8_t *)num);
		LCD_DisplayStringLine(40+i*35,40,(uint8_t *)".");
		if(i == op_flag)
		{
			LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_YELLOW);
		}else{
			LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
		}
		LCD_DisplayStringLine(40+i*35,60,(uint8_t *)indexname);
	}
}

void Search_Handle(void)
{
	u16 i,j;
	Disp_Search_Info(0);
	for(i=0;i<BlockNum.Num[1]/40+1;i++)
	{
		Read_Index(i);
		for(j=0;j<40;j++)
		{
			sprintf(scomp1,"%0.2d%0.2d%0.2d%0.2d",HisIndex.Date[j][1],
												  HisIndex.Date[j][2],
												  HisIndex.Date[j][3],
												  HisIndex.Date[j][4]);
			if(strcmp(SearchBuffer,scomp1) == 0)
			{
				hispage = HisIndex.Index[j];
				infoflag = 0;
				break;
			}else{
				infoflag = 1;
			}
		}
//		SD_WaitWriteOperation();
//		while(SD_GetStatus() != SD_TRANSFER_OK);
		
		
	}
	page_his();
	if(infoflag == 1)
	{
		Disp_Search_Info(1);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////



////读取历史文件列表
//void READ_HIS(void)
//{
//	static u8 i;
//	char buf[5];
//	
//	res_sd = f_opendir(&dir,Dirname[(hispage-1)*10+(dirflag-1)]);
////	res_sd = f_opendir(&dir, "");
//	if(res_sd == FR_OK)
//	{
//		for(i=0;i<100;i++)
//		{
//			res_sd = f_readdir(&dir,&hisinfo);
//			if(hisinfo.fname[0] == 0)
//			{		
//				maxfilepage = i/10+1;
//				DISP_HIS_FILE();
//				filenum = i;
//				break;
//			}
//			strncpy(Filelist[i],hisinfo.fname,13);
//		}	
//		DrawInstruction("读取历史数据成功");
//	}else{
//		DrawInstruction("读取历史数据失败");
//	}
//	DISP_HIS_FILE();
//	i = 0;
//}

////读取文件夹列表
//void READ_HIS_FOLDER(void)
//{
//	static u16 i;	
//	char buf[5];

//	
//	res_sd = f_opendir(&dir, "");
//	if(res_sd == FR_OK)
//	{					
//		for(i=0;i<300;i++)
//		{
//			res_sd = f_readdir(&dir,&hisinfo);
//			if(hisinfo.fname[0] == 0)
//			{
//				maxhispage = i/10+1;
//				DISP_HIS_FOLDER();
//				foldernum = i;
//				break;
//			}
//			if(hisinfo.fattrib&AM_DIR)
//			{
//				strncpy(Dirname[i],hisinfo.fname,13);
//			}
//		}
//		DrawInstruction("读取目录成功");
//	}else{
//		DrawInstruction("读取目录失败");
//	}
//	i = 0;
//	Drawhishmenu();
//	page_flag = history;
//}

////显示历史文件
//void DISP_HIS_FILE(void)
//{
//	char buf[10],num[5];
//	u8 i;
//	
////	LCD_SetColors(LCD_COLOR_BACK,LCD_COLO R_BACK);
////	LCD_DrawFullRect(10,40,100,400);
//	
//	for(i=0;i<10;i++)
//	{
//		LCD_SetColors(LCD_COLOR_BACK,LCD_COLOR_BACK);
//		LCD_DrawFullRect(70,40+i*35,200,32);
//		LCD_SetColors(LCD_COLOR_LIGHTBLUE,LCD_COLOR_BACK);
//		sprintf(num,"%d",i+1);
//		LCD_DisplayStringLine(40+i*35,10,(uint8_t *)num);
//		LCD_DisplayStringLine(40+i*35,50,(uint8_t *)".");
//		if(i+1 == dirflag)
//		{
//			LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_YELLOW);			
//		}else{
//			LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		}
//		LCD_DisplayStringLine(40+i*35,70,(uint8_t *)Filelist[i+10*(hispage-1)]);
//	}
//	LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//	sprintf(buf,"%d/%d",hispage,maxfilepage);
//	LCD_DisplayStringLine(400,500,(uint8_t *)buf);
//	page_flag = hisfile;
//	Drawfilemenu();
//}

////显示历史数据文件夹
//void DISP_HIS_FOLDER(void)
//{
//	char buf[10],num[5];
//	u8 i;
//	
//	for(i=0;i<10;i++)
//	{

//		LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		sprintf(num,"%d",i+1);
//		LCD_DisplayStringLine(40+i*35,10,(uint8_t *)num);
//		LCD_DisplayStringLine(40+i*35,50,(uint8_t *)".");
//		if(i+1 == dirflag)
//		{
//			LCD_SetColors(LCD_COLOR_BLACK,LCD_COLOR_YELLOW);			
//		}else{
//			LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//		}
//		LCD_DisplayStringLine(40+i*35,70,(uint8_t *)Dirname[i+10*(hispage-1)]);
//	}
//	LCD_SetColors(LCD_COLOR_YELLOW,LCD_COLOR_BACK);
//	sprintf(buf,"%d/%d",hispage,maxhispage);
//	LCD_DisplayStringLine(400,500,(uint8_t *)buf);
//}


////历史数据下一页
//void His_Npage(void)
//{
//	if(hispage < maxhispage)
//	{
//		hispage ++;
//	}
//	dirflag = 1;
//	DISP_HIS_FOLDER();	
//}

////历史数据下一页
//void His_Ppage(void)
//{
//	if(hispage > 1)
//	{
//		hispage --;
//	}
//	dirflag = 1;
//	DISP_HIS_FOLDER();
//}

////建立文件存储数据
//void Create_His_File(void)
//{
//	static char namebuf[10];
//	static u8 i;
//	
//	memcpy ((void *)filename,"0:",2);
//	strcat((char *)filename,(char *)Dirname[(hispage-1)*10+(dirflag-1)]);
//	sprintf(namebuf,"/%02d%02d%02d%d", 
//			usbreadtime[4],
//			usbreadtime[5],
//			usbreadtime[6],
//			i);
//	strcat((char *)filename,(char *)namebuf);
//	strcat((char *)filename,(char *)".xls");	
//	res_sd = f_open(&fnew, (char *)filename,FA_CREATE_ALWAYS | FA_WRITE );
//	if ( res_sd == FR_OK )
//	{
//		DrawInstruction("创建文件成功");
//	}
//	else
//	{
//		DrawInstruction("创建失败");
//	}
//	READ_HIS();
//}

////格式化SD卡
//void Format_SD(void)
//{
//	f_mkfs("0:",0,0);
//}


////新建文件夹	
//void Creat_New_Folder(void)
//{
////	res_sd = f_mount(&fs,"0:",1);
//	memset(filename,0,100);
//	static u8  testcount;
//	if(res_sd == FR_NO_FILESYSTEM)
//	{
//		DrawInstruction("加载文件系统失败，请重新格式化");
//	}else{
////		DrawInstruction("加载文件系统成功");
//	}
//	sprintf(foldername,"0:/%02d%02d%02d", 
//			usbreadtime[1],
//			usbreadtime[2],
//			usbreadtime[3]);
//	res_sd = f_mkdir((char *)foldername);
//	if(res_sd == FR_OK)
//	{
//		DrawInstruction("成功");
//	}else if(res_sd == FR_EXIST){
//		DrawInstruction("目录已存在");
//	}else{
//		DrawInstruction("失败");
//	}
//	testcount++;
//	READ_HIS_FOLDER();
//}

////自动存储数据
//void SD_Save_His(void)
//{
//	
//}
//////打开指定文件夹
////void Open_Dir(void)
////{
////	memset(filename,0,100);
////	
////	sprintf(foldername,"0:/%02d%02d%02d", 
////			usbreadtime[1],
////			usbreadtime[2],
////			usbreadtime[3]);
////	res_sd = f_opendir(&dir,foldername);
////}

////删除指定文件夹
//void Delete_file(char name)
//{
//	f_unlink(&name);
//	
//}

