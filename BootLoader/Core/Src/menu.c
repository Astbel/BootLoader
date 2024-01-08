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
  uint8_t User_input;

  /*傳送字串長度*/
  Uart_sendstring("Welcome to Bootloader Mode,Plz Enter a cmd to continue\r\n", pc_uart);
  Uart_sendstring("1.Check the Version of the firmware\r\n", pc_uart);
  Uart_sendstring("2.Earse the User Applcation\r\n", pc_uart);
  Uart_sendstring("3.Updata the User Applcation\r\n", pc_uart);
  /*等待User 輸入 timeout 和 非法輸入時 不執行並跳出非正規輸入*/
  User_input = Receive_User_Select();
  /**
   * 1. 呼叫check 版本
   * 2. 擦除User 軟體
   * 3. 寫入User 軟體
   */
  switch (User_input)
  {
  case 1:
    Check_FW_Version();
    break;

  case 2:
    Erase_User_Applcation();
    break;

  case 3:
    Flash_User_Applcation();
    break;
  case 4:
    RunApp();
    break;

  default:
    // Uart_sendstring("UnValid Enter Plz check\r\n",pc_uart);
    break;
  }
}

/**
 * @brief  Run to user app
 * @param  None
 * @retval None
 * ARM架構下User跳轉地址reset規範中ADDR+4才是USER APP位置(僅限ARM MCU上)
 * 
 * 跳轉流程如下:
 * User ptr 指標建立 (如果再#define中本身就有配置+4 reset復位指標配置上就不用再度添加否則會導致跳轉找不到User程序)
 * Jump_To_Application = (pFunction)JumpAddress 建構函數指標函數作為跳轉呼叫地址
 * __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS) User App stack層正確性
 * Jump_To_Application()跳轉地址
 */
void RunApp(void)
{
  // 从应用程序的第二字（地址 APPLICATION_ADDRESS+4）读取一个32位的值
  JumpAddress = *(__IO uint32_t *)(APPLICATION_ADDRESS);

  // 将 Jump_To_Application 设置为一个函数指针，指向 JumpAddress 地址处的函数
  Jump_To_Application = (pFunction)JumpAddress;

  // 设置主堆栈指针（MSP）为应用程序的起始地址处的值,確保User app stack層正確性
  __set_MSP(*(__IO uint32_t *)APPLICATION_ADDRESS);

  // 调用 Jump_To_Application，实际上执行了一个间接函数调用，跳转到用户应用程序
  Jump_To_Application();

  /*close isr*/
  // __disable_irq();
  /*可以正確跳轉致User 應用層*/
  // void (*user_app_reset_handler)(void) = (void *)(*((uint32_t *)(APPLICATION_ADDRESS)));
  // user_app_reset_handler();
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

void Test_Cnt_Jump_User_App(void)
{
  /*buffer for cnt*/
  char buffer[Buffer_size];
  /*計時跳轉*/
  static int16_t value = 0;
  value++;
  /*Print on terinaml*/
  sprintf(buffer, "TImer is %d", value);
  Uart_sendstring(buffer, pc_uart);
  if(value>=Jump_CNT)
    RunApp();
}
