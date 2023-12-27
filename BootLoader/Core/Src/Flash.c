
#include "main.h"

FLASH_EraseInitTypeDef flashstruct;


/**
 * @brief 
 * 
 * @param data 
 * @param startAddr 
 * @param numberofwords 
 * @return uint32_t 
 */

uint32_t Flash_Write_Flash_Memory(uint32_t *data,uint32_t startAddr, uint16_t numberofwords)
{
    HAL_FLASH_Unlock();
    /*檢索扇區*/
    uint32_t Flash_Start_Sector = GetSector(startAddr);
    uint32_t End_Sector_Addr = startAddr + numberofwords * 4;
    uint32_t Flash_End_Secotr = GetSector(End_Sector_Addr);

    flashstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    flashstruct.Sector = Flash_Start_Sector;
    flashstruct.NbSectors = (Flash_End_Secotr - Flash_Start_Sector) + 1;
    flashstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    uint32_t sectorError = 0;
    // if (HAL_FLASHEx_Erase(&flashstruct, &sectorError) != HAL_OK)
    // {
    // }

    for (int i = 0; i < numberofwords; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, startAddr, data[i]) != HAL_OK)
        {
        }
        startAddr += 4;
    }

    HAL_FLASH_Lock();
    return 0; // 返回写入的Flash地址
}

//
/**
 * @brief
 * Erase memory 只能擦除一個扇曲這裡是指一個區間到特定區間
 * @param startSector 開始區間
 * @param endSector   結束區間
 */
void Flash_Erase_Sectors(uint32_t startSector, uint32_t endSector)
{
    /*起始終點宣告*/
    static uint32_t Start_Address, End_Address;

    HAL_FLASH_Unlock();
    Start_Address = GetSector(startSector);
    End_Address = GetSector(endSector);
    //   FLASH_EraseInitTypeDef eraseInitStruct;
    flashstruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    flashstruct.Sector = Start_Address;
    flashstruct.NbSectors = End_Address - Start_Address + 1; // 计算要擦除的扇区数量
    flashstruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;        // 选择适当的电压范围

    uint32_t sectorError = 0;
    if (HAL_FLASHEx_Erase(&flashstruct, &sectorError) != HAL_OK)
    {
        // 擦除失败
        // 在此处处理错误
    }

    HAL_FLASH_Lock();
}

// 查找扇曲資料
/**
 * @brief 查找扇曲資料
 *
 * @param targetData  目標資料
 * @param size        資料大小
 * @param flashAddress 扇曲位置
 * @return uint32_t
 */
uint32_t Flash_Find_Data(uint32_t *targetData, uint32_t size, uint32_t flashAddress)
{
    for (uint32_t i = 0; i < size; i++)
    {
        if (*(__IO uint32_t *)flashAddress == targetData[i])
        {
            return flashAddress; // 找到匹配的数据，返回其地址
        }
        flashAddress += 4; // 因为我们在这里使用32位字（4个字节）
    }

    return 0; // 未找到匹配的数据
}
