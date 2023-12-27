#include "main.h"

/*Variable*/
void Uart_send_ADC_Result(struct PFC_VARIABLES *pfc_vars);

/*ADC Select Channel*/
/**ADC1 GPIO Configuration
PA0-    ------> ADC1_IN0   12V
PA1     ------> ADC1_IN1   5 V
*/
inline void ADC_Select_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/*Task for sample multi data*/
void Multi_ADC_Sample(void)
{
    const uint32_t channels[] = {ADC_CHANNEL_0, ADC_CHANNEL_1};
    //取樣對應的Channel
    for (int i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) {
        ADC_Select_Channel(channels[i]);
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1000);
        PFC_Variables.adc_raw[i] = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
    }
   
    //傳送訊息
//    Uart_send_ADC_Result(&PFC_Variables);
}

/*傳送ADC採樣值給C#*/
void Uart_send_ADC_Result(struct PFC_VARIABLES *pfc_vars)
{
	char buffer[50];
    static uint16_t convert_data1,convert_data2;
    //換算對應的數值
    convert_data1=pfc_vars->adc_raw[0];
    convert_data2=pfc_vars->adc_raw[1];
    // 使用结构体指针访问结构体成员
	sprintf(buffer, "volt is %d, curr is %d", convert_data1, convert_data2);
	Uart_sendstring(buffer,pc_uart);
}

//內差法取Flash值計算保護點
/**
 * @brief 
 * 
 */
void Dynamic_Interpolation_ADC(void)
{
    //Value from Flash Max multi the dynamic Gain



}




