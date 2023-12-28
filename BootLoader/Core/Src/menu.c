#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t FileName[FILE_NAME_LENGTH];//array to store filename of download *.bin
pFunction Jump_To_Application;
uint32_t JumpAddress;


/* Private function prototypes -----------------------------------------------*/

/**
 * @brief 
 * 顯示PC端進入Bootloader 選單
 */
void BootLoader_Menu(void)
{
    /*傳送字串長度*/
    char buffer[Uart_Buffer];

    sprintf(buffer, "Welcome to Bootloader Mode,Plz Enter a cmd to continue....");

    Uart_sendstring(buffer, pc_uart);

    /*呼叫bootloader執行續*/

    /**
     * 1. 呼叫check 版本
     * 2. 擦除User 軟體
     * 3. 寫入User 軟體 
     * 4.  
     */

   
}

/**
  * @brief  Run to user app
  * @param  None
  * @retval None
  */
void RunApp(void)
{
    Uart_sendstring("Start program execution......\r\n\n", pc_uart);

    // 从应用程序的第二字（地址 APPLICATION_ADDRESS+4）读取一个32位的值
    JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);

    // 将 Jump_To_Application 设置为一个函数指针，指向 JumpAddress 地址处的函数
    Jump_To_Application = (pFunction)JumpAddress;

    // 设置主堆栈指针（MSP）为应用程序的起始地址处的值
    __set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);

    // 调用 Jump_To_Application，实际上执行了一个间接函数调用，跳转到用户应用程序
    Jump_To_Application();
}

/**
 * @brief 移除使用者內程序
 * ADDR  起始sector bank
 * ADDR  結束sector bank
 * 這邊要定義User Applcation 的記憶體層
 */
void Erase_User_Applcation(void)
{
   // printf("Erasing Flash memory\n");
	Uart_sendstring("Erasing User Applcation\n", pc_uart);
	// 处理 Erase Flash memory 命令
	Flash_Erase_Sectors(ADDR_FLASH_SECTOR_1, ADDR_FLASH_SECTOR_7);
}

/**
 * @brief 燒錄bin檔
 * ADDR  起始sector bank
 * ADDR  結束sector bank
 * 透過Ymodem寫入 bin檔
 */
void Flash_User_Applcation(void)
{
   
}

/**
 * @brief 
 * 定義 軟體版本在記憶體層
 * ADDR 寫入bootloader 地址
 *  
 */
void Check_FW_Version(void)
{
   /*傳送字串長度*/
    char buffer[Uart_Buffer];

    sprintf(buffer, "The Version is");

    Uart_sendstring(buffer, pc_uart);

} 



