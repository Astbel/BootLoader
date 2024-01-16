/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "UartRingbuffer_multi.h"
#include "function.h"
#include "variable.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "FLASH_SECTOR_F4.h"
#include "ymodem.h"
#include "LinkList.h"
/*Buffer 大小*/
#define Buffer_size (512)
#define Test_Uart_Buffer_Size (4096)

/*版本資訊地址定義*/
#define FW_Version_Addr (0x0800C134)

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define device_uart &huart2
#define pc_uart &huart3
  // #define UartHandle  huart3
  /* USER CODE END Includes */
  extern UART_HandleTypeDef huart2;
  extern UART_HandleTypeDef huart3;
  extern TIM_HandleTypeDef htim10;
  extern ADC_HandleTypeDef hadc1;
  extern FLASH_EraseInitTypeDef flashstruct;
/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define Timer_PRESCALER_VALUE (uint32_t)(((SystemCoreClock) / 45000000) - 1)
#define Timer_PERIOD_VALUE (uint32_t)(10500 - 1) /* Period Value  */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// 記憶體儲存地址區域
#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_2 /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR ADDR_FLASH_SECTOR_7   //+  GetSectorSize(ADDR_FLASH_SECTOR_7) -1 /* End @ of user Flash area : sector start address + sector size -1 */

#define data_size_adc (4)

/* USER CODE END EC */
/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  /* Base address of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  /* Base address of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  /* Base address of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  /* Base address of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  /* Base address of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  /* Base address of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  /* Base address of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  /* Base address of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */
/* Exported macro ------------------------------------------------------------*/
/* Flash define */
#define Flash_Addr_size (4)
#define Flash_Memory_Empty (0xFFFFFFFF)
/*定義資料儲存地址*/
#define Flash_Addr_5V_Min (0x0800C100)
#define Flash_Addr_5V_Max (0x0800C110)
#define Flash_Addr_12V_Min (0x0800C120)
#define Flash_Addr_12V_Max (0x0800C130)
#define View_Addr  (0x08020000)

/*保護地址用來記錄上一次保護原因*/
#define Flash_Addr_OTP (0x0800C140)
#define Flash_Addr_OCP (0x0800C150)
#define Flash_Addr_OVP (0x0800C160)
/*兩點校正MAX MIN*/
#define MAX_5V (20)
#define MIN_5V (0)
#define MAX_12V (100)
#define MIN_12V (0)
/*Flash Test Define value*/
#define Flash_5V_Max_Hex (2201)
#define Flash_5V_Min_Hex (23)
#define Flash_12V_Max_Hex (3986)
#define Flash_12V_Min_Hex (23)
/*Flash 縮放增益因為int型別在C上計算要放大的關西*/
#define Flash_Gain (1000)
/*等待Master TimeOut*/
#define Master_Flash_CMD_TimeOut (5000)
/*傳輸buffer 大小配置*/
#define Uart_Buffer (200)

/* ADC Gain transfer back to float number */
#define ADC_Driver_Gain (3.3 / 4095)

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
#define Dynmaic_Gain (2)

/*Boolean define*/
#define True (1)
#define False (0)
/* Strcmp check  */
#define String_True (0)
#define String_False (1)
/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define Volt_12V_Pin GPIO_PIN_0
#define Volt_12V_GPIO_Port GPIOA
#define Volt_5V_Pin GPIO_PIN_1
#define Volt_5V_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/*Jump Test*/
#define Jump_CNT (5000)
/*Bootloader 模式*/
#define FILE_NAME_LENGTH (64)
#define FILE_SIZE_LENGTH (16)
/* End of the Flash address */
#define USER_FLASH_END_ADDRESS 0x0807FFFF
/* Define the user application size */
#define USER_FLASH_SIZE (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)
/*Bootloader Master and Slaver Commucation*/
//flash 回應boootloader uart 的反應訊號


  /* Error code */
  enum
  {
    FLASHIF_OK = 0,
    FLASHIF_ERASEKO,
    FLASHIF_WRITINGCTRL_ERROR,
    FLASHIF_WRITING_ERROR
  };

  enum
  {
    FLASHIF_PROTECTION_NONE = 0,
    FLASHIF_PROTECTION_PCROPENABLED = 0x1,
    FLASHIF_PROTECTION_WRPENABLED = 0x2,
    FLASHIF_PROTECTION_RDPENABLED = 0x4,
  };

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the Bootloader code */
#define APPLICATION_ADDRESS (0x8020000 + 4)
#define APPLICATION_ADDRESS_END (uint32_t)0x08060000
/*Uart Time out*/
#define TX_TIMEOUT ((uint32_t)100)
#define RX_TIMEOUT ((uint32_t)0xFFFFFFFF)
/*Common*/
#define IS_AF(c) ((c >= 'A') && (c <= 'F'))
#define IS_af(c) ((c >= 'a') && (c <= 'f'))
#define IS_09(c) ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c) IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c) IS_09(c)
#define CONVERTDEC(c) (c - '0')

#define CONVERTHEX_alpha(c) (IS_AF(c) ? (c - 'A' + 10) : (c - 'a' + 10))
#define CONVERTHEX(c) (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

  /*條件編譯DEBUG區*/
  // #define DEBUG_MODE_FLASH 1
  //  #define DEBUG_MODE_UART  1
  // #define DEBUG_MODE_UART_ADC_Message 1
     #define View_Buffer 1
     #define Test_Receive_Buffer 1
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
