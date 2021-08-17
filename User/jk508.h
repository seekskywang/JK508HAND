#include "stm32f4xx.h"
#include "stdlib.h"
#include "ff.h"
#include <string.h>

#ifndef _jk508_h_
#define _jk508_h_
typedef struct{
	char Date[8];
	char Time[500][8];
	float Temp[16][500];
	char add[344];
}SAVE_SD;

typedef struct{
	char Date[40][8];
	u32 Index[40];
	u32 ext[8];
}SAVE_INDEX;


typedef struct{
	u32 Num[128];
}BLOCK_REC;

extern u8 op_flag;
extern u8 page_flag;
extern u8 key_value;
extern u8 op_sw;
extern u8 ch_page;
extern u8 bit_flag;
extern u8 tc_type;
extern float ch1_temp;
extern float offset[40];
extern u16 count;
extern u8 time_buf[10][7];
extern u8 histime[10][7];
extern float range;
extern float enrate;
extern float G_Data[16][500];//�¶���������
extern float hisdata[16][500];
extern u8 Data_buf[16][16];
extern u32 rec_num;
extern uint8_t USB_Recive_Buffer[65];
extern uint16_t USB_Received_Count;
extern float TempHLimits[40];
extern float TempLLimits[40];
extern float YLIMIT[3];
extern float Correction[40];
extern float ch_temp[40];
extern float avg_data[3];
extern u8 trigtime[4];
extern u8 RecBuff[39];
extern u8 usbsendbuf[0x40];
extern u8 savedata[80];
extern u8 tempreq[9];
extern u8 usbreadtime[7];
extern u16 his_config[10];
extern u8 reqcode;
extern u32 corpara[20];
//extern u8 usbreadtime;
extern u8 eqmtstatus;
extern u8 cor_page;
extern u8 brightness;
extern u8 charge;
extern u8 battery;
extern u8 usbstatus;
extern __IO uint32_t FlashID;
extern u16 graphcount;
extern u8 reflag;
extern char UsbHidReceiveComplete;
extern u16 hpage;
extern u16 XCOOR;
extern u16 YCOOR;
extern u8 touchflag;
extern u8 fileflag;
extern u8 sendflag;
extern u8 date_page;
extern u8 time_page;
extern u8 press;
extern u8 spt_page;
extern u8 cor_page;
extern u8 indexpage;
extern u8 trigflag;
extern u8 tcpage;
extern u8 tcflag;
extern u8 passverify;
extern char SN[8];
extern char SearchBuffer[8];
extern char data[17];
extern u16 hispage;
extern u16 hispageend;
extern u16 hispagestart;
extern u8 dirflag;
extern RTC_TimeTypeDef RTC_TimeStructure;
extern RTC_DateTypeDef RTC_DateStructure;
extern u16 foldernum,filenum;
extern FRESULT res_sd; 
extern SAVE_SD SaveBuffer;
extern SAVE_SD ReadBuffer;
extern SAVE_INDEX HisIndex;
extern u16 hiscursor;
extern u8 indexflag;
extern u8 recordflag;
extern u8 sdstatus;
extern u16 count;
extern u16 usavetime[2];

void page_home(void);
void page_set(void);
void page_sys(void);
void page_sep(void);
void page_sysinfo(void);
void page_graph(void);
void page_his(void);
void back_his(void);
void page_gset(void);
void page_cal(void);
void power_on(void);
void Save_flag(void);
void Read_flag(void);
void graph_his(void);
void Save_history(u16 rec);
void Read_history(u16 rec);
void Save_time(u16 rec);
void Read_time(u16 rec);
void Read_Flash_Init_Handle(void);
void Erase_all(void);
u8 PowerOffDetect(void);
void PowerOffHandle(void);
void ButtonSound(void);
void EXPSDDATA(void);
void Draw_His_Index(u8 page);
void Draw_His_Graph(void);

u8 udisk_scan(void);
void Delay(__IO u32 nCount);
void minfocus_on(u8 c,float lmt);
void minfocus_off(u8 c,float lmt);
void maxfocus_on(u8 c,float lmt);
void maxfocus_off(u8 c,float lmt);
void yhfocus_on(float lmt);
void yhfocus_off(float lmt);
void ylfocus_on(float lmt);
void ylfocus_off(float lmt);
void multifocus_on(int lmt);
void multifocus_off(int lmt);
void cal_disp(u8 c,float lmt);
void Draw_graph(void);
void InitBrt(void);
void TempDisplay(void);
void Sprint_temp(float temp);
void Check_limits(u8 chn);
void UsbDataHandle(void);
void Save_Sflag(void);
void Read_Sflag(void);
int hex_to_bcd(int data);
void DrawTime(void);
//void hispage(u16 page);
void TouchCal(void);
void XYCAL(u8 step);
void TouchHandle(u16 x,u16 y);
void ChannelSwtichBig(void);
void ChannelSwtichMid(void);
void ChannelSwtichSml(void);
void SetTctype(u8 type);
void DrawLogo(u16 x,u16 y);
void dim_set(u8 key);
void stimefocus_on(int lmt);
void stimefocus_off(int lmt);
void Disp_Type(void);
void SetSTctype(void);
void SetUnit(void);
void JumpBoot(u8 flag);
void Disp_Factory(void);
void sdtest(void);
void READ_HIS(void);
void Creat_New_Folder(void);
void DISP_HIS_FOLDER(void);
void Create_His_File(void);
void Mount_SD(void);
void Disp_hispagenum(void);
void READ_HIS_FOLDER(void);
void His_Npage(void);
void His_Ppage(void);
void Open_Dir(void);
void Format_SD(void);
void DISP_HIS_FILE(void);
void Write_His_Data(void);
void Write_His_Data_Man(void);
void Read_His_Data(u32 block);
void Read_Block_Rec(void);
void Write_Block_Rec(void);
void Read_Index(u32 indexnum);

extern u8 usbbuf[0x40];
extern BLOCK_REC BlockNum;
uint16_t CRC16(uint8_t *puchMsg, uint8_t Len);
uint32_t Flash_EnableReadProtection(void);
uint32_t Flash_DisableReadProtection(void);

typedef struct
{
	u8 save_flag;
	u16 xoff;
	u16 yoff;
	float x_pos;
	float y_pos;
	
} Touch_save_Typedef;



extern Touch_save_Typedef    Touch_save;
//����ʱ���
//#define STARTH					   trigtime[0]
//#define STARTM					   trigtime[1]
//#define ENDH					   trigtime[2]
//#define ENDM					   trigtime[3]
//#define LOGO			1
//�ļ�ϵͳ
#define MAXDIR					   300//����ļ��и���
#define MAXFILE					   100//����ļ�����

//ƽ�������Сֵ
#define TEMPAVG					   avg_data[0]
#define TEMPMAX					   avg_data[1]
#define TEMPMIN					   avg_data[2]
//��ʷ���ݴ洢����
#define SECTOR_REC					his_config[0]
#define TIME_REC					his_config[1]
//������������
#define CODETEMP					0

//USB�����Ӧ
#define NO_CONNECTION				0
#define UDISK_READY					1
#define UDISK_NOTREADY				2

//SD�����
#define SD_MAX_BLOCK				200000

//����ֵ
#define COR1					   Correction[0]
#define COR2					   Correction[1] 
#define COR3					   Correction[2] 
#define COR4					   Correction[3] 
#define COR5					   Correction[4] 
#define COR6					   Correction[5] 
#define COR7					   Correction[6] 
#define COR8					   Correction[7] 
#define COR9					   Correction[8] 
#define COR10					   Correction[9] 
#define COR11					   Correction[10]
#define COR12					   Correction[11]
#define COR13					   Correction[12] 
#define COR14					   Correction[13] 
#define COR15					   Correction[14] 
#define COR16					   Correction[15] 
#define COR17					   Correction[16] 
#define COR18					   Correction[17] 
#define COR19					   Correction[18] 
#define COR20					   Correction[19] 
#define COR21					   Correction[20]
#define COR22					   Correction[21] 
#define COR23					   Correction[22] 
#define COR24					   Correction[23] 
#define COR25					   Correction[24] 
#define COR26					   Correction[25] 
#define COR27					   Correction[26] 
#define COR28					   Correction[27] 
#define COR29					   Correction[28] 
#define COR30					   Correction[29] 
#define COR31					   Correction[30]
#define COR32					   Correction[31]
#define COR33					   Correction[32] 
#define COR34					   Correction[33] 
#define COR35					   Correction[34] 
#define COR36					   Correction[35] 
#define COR37					   Correction[36] 
#define COR38					   Correction[37] 
#define COR39					   Correction[38] 
#define COR40					   Correction[39] 

//��ʾ�¶�ֵ
#define CH1TEMP						ch_temp[0]
#define CH2TEMP						ch_temp[1]
#define CH3TEMP						ch_temp[2]
#define CH4TEMP						ch_temp[3]
#define CH5TEMP						ch_temp[4]
#define CH6TEMP						ch_temp[5]
#define CH7TEMP						ch_temp[6]
#define CH8TEMP						ch_temp[7]
#define CH9TEMP						ch_temp[8]
#define CH10TEMP					ch_temp[9]
#define CH11TEMP					ch_temp[10]
#define CH12TEMP					ch_temp[11]
#define CH13TEMP					ch_temp[12]
#define CH14TEMP					ch_temp[13]
#define CH15TEMP					ch_temp[14]
#define CH16TEMP					ch_temp[15]
#define CH17TEMP					ch_temp[16]
#define CH18TEMP					ch_temp[17]
#define CH19TEMP					ch_temp[18]
#define CH20TEMP					ch_temp[19]
#define CH21TEMP					ch_temp[20]
#define CH22TEMP					ch_temp[21]
#define CH23TEMP					ch_temp[22]
#define CH24TEMP					ch_temp[23]
#define CH25TEMP					ch_temp[24]
#define CH26TEMP					ch_temp[25]
#define CH27TEMP					ch_temp[26]
#define CH28TEMP					ch_temp[27]
#define CH29TEMP					ch_temp[28]
#define CH30TEMP					ch_temp[29]
#define CH31TEMP					ch_temp[30]
#define CH32TEMP					ch_temp[31]
#define CH33TEMP					ch_temp[32]
#define CH34TEMP					ch_temp[33]
#define CH35TEMP					ch_temp[34]
#define CH36TEMP					ch_temp[35]
#define CH37TEMP					ch_temp[36]
#define CH38TEMP					ch_temp[37]
#define CH39TEMP					ch_temp[38]
#define CH40TEMP					ch_temp[39]



//ͨ������������
#define   CH1MIN                    TempLLimits[0]
#define   CH2MIN                    TempLLimits[1]
#define   CH3MIN                    TempLLimits[2]
#define   CH4MIN                    TempLLimits[3]
#define   CH5MIN                    TempLLimits[4]
#define   CH6MIN                    TempLLimits[5]
#define   CH7MIN                    TempLLimits[6]
#define   CH8MIN                    TempLLimits[7]
#define   CH9MIN                    TempLLimits[8]
#define   CH10MIN                   TempLLimits[9]
#define   CH11MIN                   TempLLimits[10]
#define   CH12MIN                   TempLLimits[11]
#define   CH13MIN                   TempLLimits[12]
#define   CH14MIN                   TempLLimits[13]
#define   CH15MIN                   TempLLimits[14]
#define   CH16MIN                   TempLLimits[15]
#define   CH17MIN                   TempLLimits[16]
#define   CH18MIN                   TempLLimits[17]
#define   CH19MIN                   TempLLimits[18]
#define   CH20MIN                   TempLLimits[19]
#define   CH21MIN                   TempLLimits[20]
#define   CH22MIN                   TempLLimits[21]
#define   CH23MIN                   TempLLimits[22]
#define   CH24MIN                   TempLLimits[23]
#define   CH25MIN                   TempLLimits[24]
#define   CH26MIN                   TempLLimits[25]
#define   CH27MIN                   TempLLimits[26]
#define   CH28MIN                   TempLLimits[27]
#define   CH29MIN                   TempLLimits[28]
#define   CH30MIN                   TempLLimits[29]
#define   CH31MIN                   TempLLimits[30]
#define   CH32MIN                   TempLLimits[31]
#define   CH33MIN                   TempLLimits[32]
#define   CH34MIN                   TempLLimits[33]
#define   CH35MIN                   TempLLimits[34]
#define   CH36MIN                   TempLLimits[35]
#define   CH37MIN                   TempLLimits[36]
#define   CH38MIN                   TempLLimits[37]
#define   CH39MIN                   TempLLimits[38]
#define   CH40MIN                   TempLLimits[39]
#define   CH1MAX                    TempHLimits[0]
#define   CH2MAX                    TempHLimits[1]
#define   CH3MAX                    TempHLimits[2]
#define   CH4MAX                    TempHLimits[3]
#define   CH5MAX                    TempHLimits[4]
#define   CH6MAX                    TempHLimits[5]
#define   CH7MAX                    TempHLimits[6]
#define   CH8MAX                    TempHLimits[7]
#define   CH9MAX                    TempHLimits[8]
#define   CH10MAX                   TempHLimits[9]
#define   CH11MAX                   TempHLimits[10]
#define   CH12MAX                   TempHLimits[11]
#define   CH13MAX                   TempHLimits[12]
#define   CH14MAX                   TempHLimits[13]
#define   CH15MAX                   TempHLimits[14]
#define   CH16MAX                   TempHLimits[15]
#define   CH17MAX                   TempHLimits[16]
#define   CH18MAX                   TempHLimits[17]
#define   CH19MAX                   TempHLimits[18]
#define   CH20MAX                   TempHLimits[19]
#define   CH21MAX                   TempHLimits[20]
#define   CH22MAX                   TempHLimits[21]
#define   CH23MAX                   TempHLimits[22]
#define   CH24MAX                   TempHLimits[23]
#define   CH25MAX                   TempHLimits[24]
#define   CH26MAX                   TempHLimits[25]
#define   CH27MAX                   TempHLimits[26]
#define   CH28MAX                   TempHLimits[27]
#define   CH29MAX                   TempHLimits[28]
#define   CH30MAX                   TempHLimits[29]
#define   CH31MAX                   TempHLimits[30]
#define   CH32MAX                   TempHLimits[31]
#define   CH33MAX                   TempHLimits[32]
#define   CH34MAX                   TempHLimits[33]
#define   CH35MAX                   TempHLimits[34]
#define   CH36MAX                   TempHLimits[35]
#define   CH37MAX                   TempHLimits[36]
#define   CH38MAX                   TempHLimits[37]
#define   CH39MAX                   TempHLimits[38]
#define   CH40MAX                   TempHLimits[39]
#define   YHLIMIT                   YLIMIT[0]
#define   YLLIMIT                   YLIMIT[1]
#define   MULTI						YLIMIT[2]


////��Ҫ����ı�־λ
//#define   CH1_SW             		(FLAG1.BIT_FLAG.SW1)//ͨ��1���ر�־λ 
//#define   CH2_SW             		(FLAG1.BIT_FLAG.SW2)//ͨ��2���ر�־λ 
//#define   CH3_SW 					(FLAG1.BIT_FLAG.SW3)//ͨ��3���ر�־λ 
//#define   CH4_SW 					(FLAG1.BIT_FLAG.SW4)//ͨ��4���ر�־λ 
//#define   CH5_SW 					(FLAG1.BIT_FLAG.SW5)//ͨ��5���ر�־λ 
//#define   CH6_SW 					(FLAG1.BIT_FLAG.SW6)//ͨ��6���ر�־λ 
//#define   CH7_SW       				(FLAG1.BIT_FLAG.SW7)//ͨ��7���ر�־λ 
//#define   CH8_SW       		    	(FLAG1.BIT_FLAG.SW8)//ͨ��8���ر�־λ 

//#define   CH9_SW             		(FLAG2.BIT_FLAG.SW9)//ͨ��9���ر�־λ 
//#define   CH10_SW             		(FLAG2.BIT_FLAG.SW10)//ͨ��10���ر�־λ 
//#define   CH11_SW 					(FLAG2.BIT_FLAG.SW11)//ͨ��11���ر�־λ 
//#define   CH12_SW 					(FLAG2.BIT_FLAG.SW12)//ͨ��12���ر�־λ 
//#define   CH13_SW 					(FLAG2.BIT_FLAG.SW13)//ͨ��13���ر�־λ 
//#define   CH14_SW 					(FLAG2.BIT_FLAG.SW14)//ͨ��14���ر�־λ 
//#define   CH15_SW       			(FLAG2.BIT_FLAG.SW15)//ͨ��15���ر�־λ 
//#define   CH16_SW       		    (FLAG2.BIT_FLAG.SW16)//ͨ��16���ر�־λ 

//#define   CH17_SW             		(FLAG3.BIT_FLAG.SW17)//ͨ��17���ر�־λ 
//#define   CH18_SW             		(FLAG3.BIT_FLAG.SW18)//ͨ��18���ر�־λ 
//#define   CH19_SW 					(FLAG3.BIT_FLAG.SW19)//ͨ��19���ر�־λ 
//#define   CH20_SW 					(FLAG3.BIT_FLAG.SW20)//ͨ��20���ر�־λ 
//#define   CH21_SW 					(FLAG3.BIT_FLAG.SW21)//ͨ��21���ر�־λ 
//#define   CH22_SW 					(FLAG3.BIT_FLAG.SW22)//ͨ��22���ر�־λ 
//#define   CH23_SW       			(FLAG3.BIT_FLAG.SW23)//ͨ��23���ر�־λ 
//#define   CH24_SW       		    (FLAG3.BIT_FLAG.SW24)//ͨ��24���ر�־λ 

//#define   CH25_SW             		(FLAG4.BIT_FLAG.SW25)//ͨ��25���ر�־λ 
//#define   CH26_SW             		(FLAG4.BIT_FLAG.SW26)//ͨ��26���ر�־λ 
//#define   CH27_SW 					(FLAG4.BIT_FLAG.SW27)//ͨ��27���ر�־λ 
//#define   CH28_SW 					(FLAG4.BIT_FLAG.SW28)//ͨ��28���ر�־λ 
//#define   CH29_SW 					(FLAG4.BIT_FLAG.SW29)//ͨ��29���ر�־λ 
//#define   CH30_SW 					(FLAG4.BIT_FLAG.SW30)//ͨ��30���ر�־λ 
//#define   CH31_SW       			(FLAG4.BIT_FLAG.SW31)//ͨ��31���ر�־λ 
//#define   CH32_SW       		    (FLAG4.BIT_FLAG.SW32)//ͨ��32���ر�־λ

//#define   CH33_SW             		(FLAG5.BIT_FLAG.SW33)//ͨ��33���ر�־λ 
//#define   CH34_SW             		(FLAG5.BIT_FLAG.SW34)//ͨ��34���ر�־λ 
//#define   CH35_SW 					(FLAG5.BIT_FLAG.SW35)//ͨ��35���ر�־λ 
//#define   CH36_SW 					(FLAG5.BIT_FLAG.SW36)//ͨ��36���ر�־λ 
//#define   CH37_SW 					(FLAG5.BIT_FLAG.SW37)//ͨ��37���ر�־λ 
//#define   CH38_SW 					(FLAG5.BIT_FLAG.SW38)//ͨ��38���ر�־λ 
//#define   CH39_SW       			(FLAG5.BIT_FLAG.SW39)//ͨ��39���ر�־λ 
//#define   CH40_SW       		    (FLAG5.BIT_FLAG.SW40)//ͨ��40���ر�־λ

//#define   TCTYPE       		    	(FLAG6.BIT_FLAG.TC)  //�ȵ�ż���ͱ�־λ
//#define   BEEP       		    	(FLAG6.BIT_FLAG.BP)  //Ѷ�쿪�ر�־λ
//#define   UNIT       		    	(FLAG6.BIT_FLAG.UT)  //��λ��־λ
//#define   FILTER       		    	(FLAG6.BIT_FLAG.FT)  //�Ƚϱ�־λ

//#define   FONT       		    	(FLAG7.BIT_FLAG.FN)  //�����־λ
//#define   SPEED       		    	(FLAG7.BIT_FLAG.SP)  //�ٶȱ�־λ
//#define   BAUD       		    	(FLAG7.BIT_FLAG.BD)  //���ر�־λ
//#define   LANG       		    	(FLAG7.BIT_FLAG.LG)  //���Ա�־λ
//#define   BRTS						(FLAG8.BIT_FLAG.BR)  //���ȱ�־λ
//#define   DIM						(FLAG8.BIT_FLAG.DM)  //�������ȱ�־λ

//��Ҫ���������
#define   CH1_SW             		savedata[0]//ͨ��1���ر�־λ 
#define   CH2_SW             		savedata[1]//ͨ��2���ر�־λ 
#define   CH3_SW 					savedata[2]//ͨ��3���ر�־λ 
#define   CH4_SW 					savedata[3]//ͨ��4���ر�־λ 
#define   CH5_SW 					savedata[4]//ͨ��5���ر�־λ 
#define   CH6_SW 					savedata[5]//ͨ��6���ر�־λ 
#define   CH7_SW       				savedata[6]//ͨ��7���ر�־λ 
#define   CH8_SW       		    	savedata[7]//ͨ��8���ر�־λ 

#define   CH9_SW             		savedata[8]//ͨ��9���ر�־λ 
#define   CH10_SW             		savedata[9]//ͨ��10���ر�־λ 
#define   CH11_SW 					savedata[10]//ͨ��11���ر�־λ 
#define   CH12_SW 					savedata[11]//ͨ��12���ر�־λ 
#define   CH13_SW 					savedata[12]//ͨ��13���ر�־λ 
#define   CH14_SW 					savedata[13]//ͨ��14���ر�־λ 
#define   CH15_SW       			savedata[14]//ͨ��15���ر�־λ 
#define   CH16_SW       		    savedata[15]//ͨ��16���ر�־λ 

#define   CH17_SW             		savedata[16]//ͨ��17���ر�־λ 
#define   CH18_SW             		savedata[17]//ͨ��18���ر�־λ 
#define   CH19_SW 					savedata[18]//ͨ��19���ر�־λ 
#define   CH20_SW 					savedata[19]//ͨ��20���ر�־λ 
#define   CH21_SW 					savedata[20]//ͨ��21���ر�־λ 
#define   CH22_SW 					savedata[21]//ͨ��22���ر�־λ 
#define   CH23_SW       			savedata[22]//ͨ��23���ر�־λ 
#define   CH24_SW       		    savedata[23]//ͨ��24���ر�־λ 

#define   CH25_SW             		savedata[24]//ͨ��25���ر�־λ 
#define   CH26_SW             		savedata[25]//ͨ��26���ر�־λ 
#define   CH27_SW 					savedata[26]//ͨ��27���ر�־λ 
#define   CH28_SW 					savedata[27]//ͨ��28���ر�־λ 
#define   CH29_SW 					savedata[28]//ͨ��29���ر�־λ 
#define   CH30_SW 					savedata[29]//ͨ��30���ر�־λ 
#define   CH31_SW       			savedata[30]//ͨ��31���ر�־λ 
#define   CH32_SW       		    savedata[31]//ͨ��32���ر�־λ

#define   CH33_SW             		savedata[32]//ͨ��33���ر�־λ 
#define   CH34_SW             		savedata[33]//ͨ��34���ر�־λ 
#define   CH35_SW 					savedata[34]//ͨ��35���ر�־λ 
#define   CH36_SW 					savedata[35]//ͨ��36���ر�־λ 
#define   CH37_SW 					savedata[36]//ͨ��37���ر�־λ 
#define   CH38_SW 					savedata[37]//ͨ��38���ر�־λ 
#define   CH39_SW       			savedata[38]//ͨ��39���ر�־λ 
#define   CH40_SW       		    savedata[39]//ͨ��40���ر�־λ

#define   TCTYPE       		    	savedata[41]  //�ȵ�ż���ͱ�־λ
#define   BEEP       		    	savedata[42]  //Ѷ�쿪�ر�־λ
#define   UNIT       		    	savedata[43]  //��λ��־λ
#define   FILTER       		    	savedata[44]  //�Ƚϱ�־λ

#define   FONT       		    	savedata[45]  //�����־λ
#define   SPEED       		    	savedata[46]  //�ٶȱ�־λ
#define   BAUD       		    	savedata[47]  //���ر�־λ
#define   LANG       		    	savedata[48]  //���Ա�־λ
#define   BRTS						savedata[49]  //���ȱ�־λ
#define   DIM						savedata[50]  //�������ȱ�־λ
#define   CHNUM						savedata[51]//�豸ͨ����
#define   TOUCH						savedata[52]//��������
#define   SAVETIME					savedata[53]//�豸ͨ����
#define   TIMETIRG					savedata[54]//��ʱ�ɼ�
#define   REPEAT					savedata[55]//�ظ��ɼ�
#define   STARTH					savedata[56]//��ʼʱ��H
#define   STARTM					savedata[57]//��ʼʱ��M
#define   ENDH						savedata[58]//��ʼʱ��H
#define   ENDM						savedata[59]//��ʼʱ��M
#define   CH1TYPE					savedata[60]//�ȵ�ż����1
#define   CH2TYPE					savedata[61]//�ȵ�ż����2
#define   CH3TYPE					savedata[62]//�ȵ�ż����3
#define   CH4TYPE					savedata[63]//�ȵ�ż����4
#define   CH5TYPE					savedata[64]//�ȵ�ż����5
#define   CH6TYPE					savedata[65]//�ȵ�ż����6
#define   CH7TYPE					savedata[66]//�ȵ�ż����7
#define   CH8TYPE					savedata[67]//�ȵ�ż����8
#define   CH9TYPE					savedata[68]//�ȵ�ż����9
#define   CH10TYPE					savedata[69]//�ȵ�ż����10
#define   CH11TYPE					savedata[70]//�ȵ�ż����11
#define   CH12TYPE					savedata[71]//�ȵ�ż����12
#define   CH13TYPE					savedata[72]//�ȵ�ż����13
#define   CH14TYPE					savedata[73]//�ȵ�ż����14
#define   CH15TYPE					savedata[74]//�ȵ�ż����15
#define   CH16TYPE					savedata[75]//�ȵ�ż����16

/* У׼���� */
#define   XCOR						corpara[0]  
#define   XOffset					corpara[1]
#define   YCOR						corpara[2]
#define   YOffset					corpara[3]
#define   Polar1					corpara[4]
#define   Polar2					corpara[5]

/*USB״̬*/
#define UNKNOWN		     2
#define UNCONNECTED		 0
#define CONNECTED		 1
/*���ȱ�־λ*/
#define L0               0
#define L1        	     1
#define L2    	         2
#define L3    	         3
#define L4    	         4

/*�������ȱ�־λ*/
#define D5               1
#define D10        	     2
#define D15    	         3
#define D30    	         6
#define DOFF    	     0

/* ҳ���־λ */
#define display          0
#define sysset           1
#define sysinfo          2
#define graph            3
#define settings         4
#define separation       5
#define graphset         6
#define history			 7
#define calibrate		 8
#define poweron 		 9
#define touchcal		 10
#define poweroff 		 11
#define factory 		 12
#define hisfile			 13
#define hisgraph		 14
#define hisgset		 	 15


/* ѡ���־λ */
#define home_type        0
#define home_ch1         1
#define home_ch2         2
#define home_ch3         3
#define home_ch4         4
#define home_ch5         5
#define home_ch6         6
#define home_ch7         7
#define home_ch8         8
#define home_ch9         9
#define home_ch10        10
#define home_ch11        11
#define home_ch12        12
#define home_ch13        13
#define home_ch14        14
#define home_ch15        15
#define home_ch16        16
#define home_ch17        17
#define home_ch18        18
#define home_ch19        19
#define home_ch20        20
#define home_ch21        21
#define home_ch22        22
#define home_ch23        23
#define home_ch24        24
#define home_ch25        25
#define home_ch26        26
#define home_ch27        27
#define home_ch28        28
#define home_ch29        29
#define home_ch30        30
#define home_ch31        31
#define home_ch32        32
#define home_ch33        33
#define home_ch34        34
#define home_ch35        35
#define home_ch36        36
#define home_ch37        37
#define home_ch38        38
#define home_ch39        39
#define home_ch40        40
#define type_1           41
#define type_2           42
#define type_3           43
#define set_filter       44
#define set_beep         45
#define set_unit         46
#define set_chnum        47
#define set_file         48
#define set_spd          49
#define set_baud         50
#define set_timer        51
#define set_font         52
#define set_lang         53
#define set_date         54
#define set_time         55
#define set_min1		 56
#define set_min2		 57
#define set_min3		 58
#define set_min4		 59
#define set_min5		 60
#define set_min6		 61
#define set_min7		 62
#define set_min8		 63
#define set_max1		 64
#define set_max2		 65
#define set_max3		 66
#define set_max4		 67
#define set_max5		 68
#define set_max6		 69
#define set_max7		 70
#define set_max8		 71
#define set_yhigh		 72
#define set_ylow		 73
#define set_multi		 74
#define set_g1		 	 75
#define set_g2		 	 76
#define set_g3		 	 77
#define set_g4		 	 78
#define set_g5		 	 79
#define set_g6		 	 80
#define set_g7		 	 81
#define set_g8		 	 82
#define set_c1		 	 83
#define set_c2		 	 84
#define set_c3		 	 85
#define set_c4		 	 86
#define set_c5		 	 87
#define set_c6		 	 88
#define set_c7		 	 89
#define set_c8		 	 90
#define set_brt			 91//����
#define set_dim		     92//��������
#define set_touch		 93//����������
#define his_search		 94//��ʷ����
#define timetrig		 95//��ʱ�ɼ�
#define repeat		 	 96//�ظ��ɼ�
#define starttime		 97//��ʼʱ��
#define endtime	 		 98//����ʱ��


/* �����־λ */
#define big	             0
#define middle           1
#define small            2

/* ���Ա�־λ */
#define chs	             0
#define eng              1


/* ��������־λ */
#define TCT          	1//T���ȵ�ż
#define TCK          	2//K���ȵ�ż
#define TCJ          	3//J���ȵ�ż
#define TCN          	4//N���ȵ�ż
#define TCE          	5//E���ȵ�ż
#define TCS          	6//S���ȵ�ż
#define TCR          	7//R���ȵ�ż
#define TCB          	8//B���ȵ�ż
#define PT100        	9//PT100���ȵ�ż
#define TCV       	 	10 //��ѹ
#define V1       	 	11//����
#define T18B20			16//18B20
#define AM2305			17//�¶�ʪ�ȴ�����

/* ҳ����־λ */
#define page1          0
#define page2          1
#define page3          2
#define page4          3
#define page5          4
/* ͨ�����ض��� */
#define ch_on           0
#define ch_off          1
/* �ȽϿ��ر�־λ */
#define ft_off          0
#define ft_on           1 
/* Ѷ�쿪�ر�־λ */
#define beep_off        0
#define beep_on         1
/* ��λ��־λ */
#define C               5
#define F               4
#define K               1
/* ���ر�־λ */
#define b9600           0
#define b19200          1
#define b38400          2
#define b57600          3
#define b115200         4
/* �ٶȱ�־λ */
#define fast            0
#define moderate        1
#define slow            2
/* ����ѡ��ѡ�б�־λ */
#define op_on           1
#define op_off          0
/* ��ʱ������־λ */
#define trig_on           1
#define trig_off          0
/* �ظ��ɼ���־λ */
#define rtp_on           1
#define rtp_off          0

/* ������Ч��Χ */
/*������*/
//���к�����
#define BIGCHLX1		5			
#define BIGCHLX2		70
//���к�����
#define BIGCHRX1		325			
#define BIGCHRX2		390
//������
#define BIGCH1Y1		90			
#define BIGCH1Y2		140

#define BIGCH2Y1		160			
#define BIGCH2Y2		210

#define BIGCH3Y1		240			
#define BIGCH3Y2		290

#define BIGCH4Y1		330			
#define BIGCH4Y2		380

/*������*/
//���к�����
#define MIDCHLX1		5			
#define MIDCHLX2		55
//���к�����
#define MIDCHMX1		325			
#define MIDCHMX2		375
//���к�����
#define MIDCHRX1		405			
#define MIDCHRX2		455
//������
#define MIDCH1Y1		100			
#define MIDCH1Y2		125

#define MIDCH2Y1		140			
#define MIDCH2Y2		165

#define MIDCH3Y1		180			
#define MIDCH3Y2		205

#define MIDCH4Y1		220			
#define MIDCH4Y2		245

#define MIDCH5Y1		260			
#define MIDCH5Y2		285

#define MIDCH6Y1		300			
#define MIDCH6Y2		325

#define MIDCH7Y1		340			
#define MIDCH7Y2		365

#define MIDCH8Y1		380			
#define MIDCH8Y2		405

/*С����*/
//���к�����
#define SMLCHLX1		5			
#define SMLCHLX2		35
//����1������
#define SMLCHM1X1		130			
#define SMLCHM1X2		160
//����2������
#define SMLCHM2X1		255			
#define SMLCHM2X2		285
//����3������
#define SMLCHM3X1		380			
#define SMLCHM3X2		405
//���к�����
#define SMLCHRX1		503			
#define SMLCHRX2		528

//������
#define SMLCH1Y1		100			
#define SMLCH1Y2		120

#define SMLCH2Y1		140			
#define SMLCH2Y2		160

#define SMLCH3Y1		180			
#define SMLCH3Y2		200

#define SMLCH4Y1		220			
#define SMLCH4Y2		240

#define SMLCH5Y1		260			
#define SMLCH5Y2		280

#define SMLCH6Y1		300			
#define SMLCH6Y2		320

#define SMLCH7Y1		340			
#define SMLCH7Y2		360

#define SMLCH8Y1		380			
#define SMLCH8Y2		400

/*�ȵ�ż��������*/
#define TCTYPEX1		90			
#define TCTYPEX2		160
#define TCTYPEY1		45			
#define TCTYPEY2		80

/*��������*/
#define FONTX1		270			
#define FONTX2		295
#define FONTY1		45			
#define FONTY2		80

/*�ײ��˵�*/
#define MENU1X1			5			
#define MENU1X2			120
#define MENU2X1			135			
#define MENU2X2			250
#define MENU3X1			260			
#define MENU3X2			375
#define MENU4X1			388			
#define MENU4X2			505
#define MENU5X1			515			
#define MENU5X2			630

#define MENUY1			445	
#define MENUY2			480

/*ϵͳ����*/
#define LANGX1			170			
#define LANGX2			282
#define DATEX1			170			
#define DATEX2			330
#define TIMEX1			400			
#define TIMEX2			528
#define BACKX1			170			
#define BACKX2			250
#define DIMX1			170			
#define DIMX2			218
#define TOUCHX1			170			
#define TOUCHX2			218
#define SYSBLANKX1		5
#define SYSBLANKX2		635

#define LANGY1			50			
#define LANGY2			81
#define DATEY1			90			
#define DATEY2			121
#define TIMEY1			90			
#define TIMEY2			121
#define BACKY1			130			
#define BACKY2			161
#define DIMY1			182			
#define DIMY2			213
#define TOUCHY1			212			
#define TOUCHY2			243
#define SYSBLANKY1		250
#define SYSBLANKY2		400

/*��������*/
#define COMPX1			150			
#define COMPX2			200
#define SPEEDX1			500			
#define SPEEDX2			550
#define BEEPX1			150			
#define BEEPX2			200
#define BAUDX1			500			
#define BAUDX2			600
#define UNITX1			150			
#define UNITX2			180
#define SETBLANKX1		5
#define SETBLANKX2		635

#define COMPY1			50			
#define COMPY2			75
#define SPEEDY1			50			
#define SPEEDY2			75
#define BEEPY1			90			
#define BEEPY2			115
#define BAUDY1			90			
#define BAUDY2			122
#define UNITY1			127			
#define UNITY2			158
#define SETBLANKY1		155
#define SETBLANKY2		278

/*У���ͷ�ѡ*/


#endif
