#include "main.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t FileName[FILE_NAME_LENGTH]; // array to store filename of download *.bin
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
  Uart_sendstring("Welcome to Bootloader Mode,Plz Enter a cmd to continue\r\n", pc_uart);
  Uart_sendstring("1.Check the Version of the firmware\r\n", pc_uart);
  Uart_sendstring("2.Earse the User Applcation\r\n", pc_uart);
  Uart_sendstring("3.Updata the User Applcation\r\n", pc_uart);
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
  JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDRESS + 4);

  // 将 Jump_To_Application 设置为一个函数指针，指向 JumpAddress 地址处的函数
  Jump_To_Application = (pFunction)JumpAddress;

  // 设置主堆栈指针（MSP）为应用程序的起始地址处的值
  __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

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
 * @brief 燒錄User Data
 * 透過Ymodem寫入 Data
 * enum 判斷本次寫入狀態
 * 接收user 資料名稱需要修正
 */
void Flash_User_Applcation(void)
{
  /*申明檔名大小*/
  uint8_t number[11] = {0};
  uint32_t size = 0;
  /*字組Buffer*/
  char buffer[Buffer_size];
  // Uart寫入Flash結果
  COM_StatusTypeDef result;
  Uart_sendstring("Waiting for the file to be sent ... (press 'a' to abort)\n\r", pc_uart);
  result = Ymodem_Receive(&size);
  /*傳輸完成打印*/
  if (result == COM_OK)
  {
    Uart_sendstring("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n", pc_uart);
    /*打印file name*/
    sprintf(buffer, "File Name: %c\n\r", aFileName);
    Uart_sendstring(buffer, pc_uart);
    /*清空buffer*/
    memset(buffer, '\0', Buffer_size);
    /*打印file size*/
    sprintf(buffer, "File Size: %d\n\r", size);
    Uart_sendstring(buffer, pc_uart);
    /*清空buffer*/
    memset(buffer, '\0', Buffer_size);
  }
  else if (result == COM_LIMIT)
    Uart_sendstring("\n\n\r The file is over size of the space memory!\n\r", pc_uart);
  else if (result == COM_ABORT)
    Uart_sendstring("\n\n\r User aborted download the file\n\r", pc_uart);
  else if (result == COM_DATA)
    Uart_sendstring("\n\n\r Verification failed!\n\r", pc_uart);
  else
    Uart_sendstring("\n\r Failed to receive the file!\n\r ", pc_uart);
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

  sprintf(buffer, "\n\n\r FW Version is %0.1f\n\r", FW_Code_Number);
  Uart_sendstring(buffer, pc_uart);
}
