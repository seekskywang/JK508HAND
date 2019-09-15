#include "stm32f4xx.h"
#include "ff.h"
#include "jk508.h"
#include "./lcd/bsp_lcd.h"

char filename[20];
char foldername[20];
FILINFO hisinfo;
char dirname[10][13];
DIR dir;
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
//BYTE WriteBuffer[] =              /* 写缓冲区*/



//读取历史文件列表
void DISP_HIS(void)
{
	static u8 i;
	char buf[5];
	res_sd = f_mount(&fs,"0:",1);
	if(res_sd == FR_NO_FILESYSTEM)
	{
		DrawInstruction("加载文件系统失败，请重新格式化");
	}else{
		DrawInstruction("加载文件系统成功");
	}
	
	res_sd = f_opendir(&dir, "");
	if(res_sd == FR_OK)
	{
		res_sd = f_readdir(&dir,&hisinfo);
		if(res_sd==FR_OK)
		{
			for(i=0;i<10;i++)
			{
				res_sd = f_readdir(&dir,&hisinfo);
				if(hisinfo.fname[0] == 0)
				{
					break;
				}
				sprintf(buf,"%d",i+1);
				LCD_DisplayStringLine(40+i*35,10,(uint8_t *)buf);
				LCD_DisplayStringLine(40+i*35,50,(uint8_t *)".");
				LCD_DisplayStringLine(40+i*35,70,(uint8_t *)hisinfo.fname);
			}
		}
		DrawInstruction("读取历史数据成功");
	}else{
		DrawInstruction("读取历史数据失败");
	}
	i = 0;
}
//显示文件夹列表
void DISP_HIS_FOLDER(void)
{
	static u8 i;
	char buf[5];
	res_sd = f_mount(&fs,"0:",1);
	if(res_sd == FR_NO_FILESYSTEM)
	{
		DrawInstruction("加载文件系统失败，请重新格式化");
	}
	
	res_sd = f_opendir(&dir, "");
	if(res_sd == FR_OK)
	{
		if(res_sd==FR_OK)
		{
			while(1)
			{
				res_sd = f_readdir(&dir,&hisinfo);
				if(res_sd == FR_OK)
				{
					if(hisinfo.fname[0]==0)break;
					
					if(hisinfo.fattrib & AM_DIR)
					{
						sprintf(buf,"%d",i+1);
						LCD_DisplayStringLine(40+i*35,10,(uint8_t *)buf);
						LCD_DisplayStringLine(40+i*35,50,(uint8_t *)".");
						LCD_DisplayStringLine(40+i*35,70,(uint8_t *)hisinfo.fname);
						strncpy(dirname[i],hisinfo.fname,13);
						i++;
					}
				}
			}
		}
		DrawInstruction("读取目录成功");
	}else{
		DrawInstruction("读取目录失败");
	}
	i = 0;
}

//建立文件存储数据
void Create_His_File(void)
{
	static char namebuf[10];
	static u8 i;
	
	memcpy ((void *)filename,"0:",2);
	sprintf(namebuf,"%02d%02d%02d%d", 
			usbreadtime[4],
			usbreadtime[5],
			usbreadtime[6],
			i);
	strcat((char *)filename,(char *)namebuf);
	strcat((char *)filename,(char *)".xls");	
	res_sd = f_open(&fnew, (char *)filename,FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_sd == FR_OK )
	{
		DrawInstruction("创建文件成功");
	}
	else
	{	
		DrawInstruction("创建失败");
	}
}

//文件系统返回值提示
void DISP_FS_RES(FRESULT res)
{
//	switch(res)
//	{
//		 
//	}
}


//新建文件夹
void Creat_New_Folder(void)
{
	res_sd = f_mount(&fs,"0:",1);
	memset(filename,0,100);
	static u8  testcount;
	if(res_sd == FR_NO_FILESYSTEM)
	{
		DrawInstruction("加载文件系统失败，请重新格式化");
	}else{
//		DrawInstruction("加载文件系统成功");
	}
	sprintf(foldername,"0:/%02d%02d%02d%d", 
			usbreadtime[1],
			usbreadtime[2],
			usbreadtime[3],
			testcount);
	res_sd = f_mkdir((char *)foldername);
	if(res_sd == FR_OK)
	{
		DrawInstruction("成功");
	}else if(res_sd == FR_EXIST){
		DrawInstruction("目录已存在");
	}else{
		DrawInstruction("失败");
	}
	testcount++;
}

//打开指定文件夹
void Open_Dir(void)
{
	memset(filename,0,100);
	
	sprintf(foldername,"0:/%02d%02d%02d", 
			usbreadtime[1],
			usbreadtime[2],
			usbreadtime[3]);
	res_sd = f_opendir(&dir,foldername);
}

//删除指定文件夹
void Delete_file(char name)
{
	f_unlink(&name);
	
}

