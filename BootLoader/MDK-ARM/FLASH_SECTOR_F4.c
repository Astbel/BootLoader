
/**
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************
  File:	      FLASH_SECTOR_F4.c
  Modifier:   ControllersTech.com
  Updated:    27th MAY 2021
  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com
  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.
  ***************************************************************************************************************
*/

#include "main.h"

/* DEFINE the SECTORS according to your reference manual
 * STM32F446RE have:-
 *  Sector 0 to Sector 3 each 16KB
 *  Sector 4 as 64KB
 *  Sector 5 to Sector 7 each 128KB
 */
int check_data;

void calculateSlope(struct Flash_Dynamic *flashMemory, uint32_t test_adc, uint32_t flashAddrMin, uint32_t flashAddrMax, float max, float min);

uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;

  if ((Address < 0x08003FFF) && (Address >= 0x08000000))
  {
    sector = FLASH_SECTOR_0;
  }
  else if ((Address < 0x08007FFF) && (Address >= 0x08004000))
  {
    sector = FLASH_SECTOR_1;
  }
  else if ((Address < 0x0800BFFF) && (Address >= 0x08008000))
  {
    sector = FLASH_SECTOR_2;
  }
  else if ((Address < 0x0800FFFF) && (Address >= 0x0800C000))
  {
    sector = FLASH_SECTOR_3;
  }
  else if ((Address < 0x0801FFFF) && (Address >= 0x08010000))
  {
    sector = FLASH_SECTOR_4;
  }
  else if ((Address < 0x0803FFFF) && (Address >= 0x08020000))
  {
    sector = FLASH_SECTOR_5;
  }
  else if ((Address < 0x0805FFFF) && (Address >= 0x08040000))
  {
    sector = FLASH_SECTOR_6;
  }
  else if ((Address < 0x0807FFFF) && (Address >= 0x08060000))
  {
    sector = FLASH_SECTOR_7;
  }
  /*  else if((Address < 0x0809FFFF) && (Address >= 0x08080000))
    {
      sector = FLASH_SECTOR_8;
    }
    else if((Address < 0x080BFFFF) && (Address >= 0x080A0000))
    {
      sector = FLASH_SECTOR_9;
    }
    else if((Address < 0x080DFFFF) && (Address >= 0x080C0000))
    {
      sector = FLASH_SECTOR_10;
    }
    else if((Address < 0x080FFFFF) && (Address >= 0x080E0000))
    {
      sector = FLASH_SECTOR_11;
    }
    else if((Address < 0x08103FFF) && (Address >= 0x08100000))
    {
      sector = FLASH_SECTOR_12;
    }
    else if((Address < 0x08107FFF) && (Address >= 0x08104000))
    {
      sector = FLASH_SECTOR_13;
    }
    else if((Address < 0x0810BFFF) && (Address >= 0x08108000))
    {
      sector = FLASH_SECTOR_14;
    }
    else if((Address < 0x0810FFFF) && (Address >= 0x0810C000))
    {
      sector = FLASH_SECTOR_15;
    }
    else if((Address < 0x0811FFFF) && (Address >= 0x08110000))
    {
      sector = FLASH_SECTOR_16;
    }
    else if((Address < 0x0813FFFF) && (Address >= 0x08120000))
    {
      sector = FLASH_SECTOR_17;
    }
    else if((Address < 0x0815FFFF) && (Address >= 0x08140000))
    {
      sector = FLASH_SECTOR_18;
    }
    else if((Address < 0x0817FFFF) && (Address >= 0x08160000))
    {
      sector = FLASH_SECTOR_19;
    }
    else if((Address < 0x0819FFFF) && (Address >= 0x08180000))
    {
      sector = FLASH_SECTOR_20;
    }
    else if((Address < 0x081BFFFF) && (Address >= 0x081A0000))
    {
      sector = FLASH_SECTOR_21;
    }
    else if((Address < 0x081DFFFF) && (Address >= 0x081C0000))
    {
      sector = FLASH_SECTOR_22;
    }
    else if (Address < 0x081FFFFF) && (Address >= 0x081E0000)
    {
      sector = FLASH_SECTOR_23;
    }*/
  return sector;
}

uint8_t bytes_temp[4];

void float2Bytes(uint8_t *ftoa_bytes_temp, float float_variable)
{
  union
  {
    float a;
    uint8_t bytes[4];
  } thing;

  thing.a = float_variable;

  for (uint8_t i = 0; i < 4; i++)
  {
    ftoa_bytes_temp[i] = thing.bytes[i];
  }
}

float Bytes2float(uint8_t *ftoa_bytes_temp)
{
  union
  {
    float a;
    uint8_t bytes[4];
  } thing;

  for (uint8_t i = 0; i < 4; i++)
  {
    thing.bytes[i] = ftoa_bytes_temp[i];
  }

  float float_variable = thing.a;
  return float_variable;
}

uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
{

  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t SECTORError;
  int sofar = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Erase the user Flash area */

  /* Get the number of sector to erase from 1st sector */

  uint32_t StartSector = GetSector(StartSectorAddress);
  uint32_t EndSectorAddress = StartSectorAddress + numberofwords * 4;
  uint32_t EndSector = GetSector(EndSectorAddress);

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = StartSector;
  EraseInitStruct.NbSectors = (EndSector - StartSector) + 1;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */

  /*這裡不執行擦除flash層會導致先前寫入的資料遺失*/

  // if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
  // {
  //   return HAL_FLASH_GetError();
  // }

  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  while (sofar < numberofwords)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
    {
      StartSectorAddress += 4; // use StartPageAddress += 2 for half word and 8 for double word
      sofar++;
    }
    else
    {
      /* Error occurred while writing data in Flash memory*/
      return HAL_FLASH_GetError();
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return 0;
}

void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
  while (1)
  {

    *RxBuf = *(__IO uint32_t *)StartSectorAddress;
    StartSectorAddress += 4;
    RxBuf++;
    if (!(numberofwords--))
      break;
  }
}

void Convert_To_Str(uint32_t *Data, char *Buf)
{
  int numberofbytes = ((strlen((char *)Data) / 4) + ((strlen((char *)Data) % 4) != 0)) * 4;

  for (int i = 0; i < numberofbytes; i++)
  {
    Buf[i] = Data[i / 4] >> (8 * (i % 4));
  }
}

void Flash_Write_NUM(uint32_t StartSectorAddress, float Num)
{

  float2Bytes(bytes_temp, Num);

  Flash_Write_Data(StartSectorAddress, (uint32_t *)bytes_temp, 1);
}

float Flash_Read_NUM(uint32_t StartSectorAddress)
{
  uint8_t buffer[4];
  float value;

  Flash_Read_Data(StartSectorAddress, (uint32_t *)buffer, 1);
  value = Bytes2float(buffer);
  return value;
}

/**
 * @brief 確認當前地址是否有資料
 * @param StartSectorAddress 檢測地址
 * @return uint32_t  回傳地址
 */
uint32_t Flash_Read_Addr_Data_Exit(uint32_t StartSectorAddress)
{
  int i;
  uint32_t *Addr = (uint32_t *)StartSectorAddress;
  /*確認data是否存在如果存在則向後移動4個地址,如果資料為null則不變跟地址*/

  for (i = 0; i < 3; ++i)
  {
    if (*Addr == Flash_Memory_Empty) // 如果資料不存在的話
      return StartSectorAddress;

    StartSectorAddress += Flash_Addr_size;
    Addr = (uint32_t *)StartSectorAddress;
  }
  return StartSectorAddress;
}

/**
 * @brief
 * 測試兩點校正結構體版本
 * 修要優化結構體
 *實際值=Min+(原始值−Min_adc)xMax_adc−Min_adc/Max−Min
 */
void Serial_Slopping_Method(void)
{
  /*測試參數*/
  uint32_t test_adc_5V = 2201;
  uint32_t test_adc_12V = 0;
  /*uart buffer 變數*/
  char buffer[Uart_Buffer];

  /*申明類別*/
  struct Flash_Dynamic flashMemory_5V;
  struct Flash_Dynamic flashMemory_12V;
  // 5V
  calculateSlope(&flashMemory_5V, test_adc_5V, Flash_Addr_5V_Min, Flash_Addr_5V_Max, MAX_5V, MIN_5V);
  // 12V
  calculateSlope(&flashMemory_12V, test_adc_12V, Flash_Addr_12V_Min, Flash_Addr_12V_Max, MAX_12V, MIN_12V);

  /*Serial log打印*/
  sprintf(buffer, "volt is %f, curr is %f\n", flashMemory_5V.slope_value, flashMemory_12V.slope_value);
  Uart_sendstring(buffer, pc_uart);
}
/**
 * @brief
 *
 * @param flashMemory 結構體申明
 * @param test_adc    ADC讀取值
 * @param flashAddrMin Min flash adc
 * @param flashAddrMax Max flash adc
 * @param max 實際值上限
 * @param min 實際值下限
 */
void calculateSlope(struct Flash_Dynamic *flashMemory, uint32_t test_adc, uint32_t flashAddrMin, uint32_t flashAddrMax, float max, float min)
{
  flashMemory->adc_value_min = Flash_Read_NUM(flashAddrMin);
  flashMemory->adc_value_max = Flash_Read_NUM(flashAddrMax);
  flashMemory->block_c = (((max - min) * Flash_Gain) / (flashMemory->adc_value_max - flashMemory->adc_value_min));
  flashMemory->block_b = (test_adc - flashMemory->adc_value_min);
  flashMemory->block_a = flashMemory->block_b * flashMemory->block_c;
  flashMemory->slope_value = (min + flashMemory->block_a) / Flash_Gain;
}


