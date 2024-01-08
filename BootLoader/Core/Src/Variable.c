#include "main.h"
struct PFC_VARIABLES PFC_Variables;
struct Allert_Portect Dyanmic_Portect;
/**********************************申明變數********************************/
/*Flash 變數申明區*/
uint8_t aFileName[64];
/*Flash 地址申明區*/
uint32_t Data_5V_Min_Addr;
uint32_t Data_5V_Max_Addr;
uint32_t Data_12V_Min_Addr;
uint32_t Data_12V_Max_Addr;
uint32_t Data_OTP_Addr;
uint32_t Data_OCP_Addr;
uint32_t Data_OVP_Addr;
uint8_t Print_Menu_Message;
/*版本號碼*/
float FW_Code_Number=0.1;
BootloaderState currentState;
UserCommand userCommand;
/*
 * 初始化變數變量
 * 請區分 所有結構體 為一組 以利於分辨
 *
 */
void Initail_Variable(void)
{
    BootloaderState currentState = MENU;
    UserCommand userCommand =0;
    Print_Menu_Message = False;
    // inital adc value of array adc
    for (int i = 0; i < 5; i++)
        PFC_Variables.adc_raw[i] = 0;
    // Inital  variable for bootloader
    
   
}
