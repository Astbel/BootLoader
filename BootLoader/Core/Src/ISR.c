#include "main.h"
void UART_TEST_SEND(void);
/*ISR call Back Handler*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim10)
    {
        // ADC Scan
        //  Multi_ADC_Sample();
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        // C# LOG獲得命令
        Get_Command_From_C_shrap();

        /*Uart 傳輸端顯示 */
        // UART_TEST_SEND();
    }
}

// ADC讀取值進入 並傳輸uart輸出給gui顯示
void UART_TEST_SEND(void)
{
    /*傳送字串長度*/
    char buffer[Uart_Buffer];
    /*條件編譯 測試用*/
#ifdef DEBUG_MODE_UART_ADC_Message
    int volt_test = rand() % 100;
    int curr_test = rand() % 100;
    sprintf(buffer, "volt is %d, curr is %d", volt_test, curr_test);
#endif
    /*從adc來取值*/

    sprintf(buffer, "volt is %d, curr is %d", PFC_Variables.adc_raw[0], PFC_Variables.adc_raw[1]);

    Uart_sendstring(buffer, pc_uart);
}
