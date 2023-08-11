//--------------------------------------------------------------
// File     : usb_bsp.h
//--------------------------------------------------------------

#ifndef __USB_BSP__H__
#define __USB_BSP__H__

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------

#include "usb_core.h"
#include "usb_conf.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_BSP
  * @brief This file is the 
  * @{
  */ 


/** @defgroup USB_BSP_Exported_Defines
  * @{
  */ 
/**
  * @}
  */ 


#define USB_DEBUG_ON         1
#define USB_DEBUG_ARRAY_ON   0
#define USB_DEBUG_FUNC_ON    0


// Log define
#define USB_INFO(fmt,arg...)           printf("<<-USB-INFO->> "fmt"\n",##arg)
#define USB_ERROR(fmt,arg...)          printf("<<-USB-ERROR->> "fmt"\n",##arg)
#define USB_DEBUG(fmt,arg...)          do{\
                                         if(USB_DEBUG_ON)\
                                         printf("<<-USB-DEBUG->>[%s] [%d]"fmt"\n",__FILE__,__LINE__, ##arg);\
                                       }while(0)
#define USB_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(USB_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-USB-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)
#define USB_DEBUG_FUNC()               do{\
                                         if(USB_DEBUG_FUNC_ON)\
                                         printf("<<-USB-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)



/** @defgroup USB_BSP_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_BSP_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_BSP_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_BSP_Exported_FunctionsPrototype
  * @{
  */ 

void BSP_Init(void);

void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);
void USB_OTG_BSP_uDelay (const uint32_t usec);
void USB_OTG_BSP_mDelay (const uint32_t msec);
void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);



#endif
