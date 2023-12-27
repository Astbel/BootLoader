#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/

/******************結構體***********************/

extern struct PFC_VARIABLES PFC_Variables;
extern struct Allert_Portect Dyanmic_Portect;
extern struct Flash_Dynamic Flash_Memory;
/*********************ADC*****************************/

/**********************C# command 指標函數************************/
typedef void (*CommandHandler)(void);
/***********************Flash**********************/
// 測試函數 變數宣告

// 正式使用函數 變數宣告

/********************STRUCT***************************/
struct PFC_VARIABLES
{
    uint32_t adc_raw[5];
};
struct Allert_Portect
{
    uint8_t OVP;
    uint8_t OCP;
    uint8_t OTP;

};
// Flash 結構體
struct Flash_Dynamic
{
    /*兩點校正公式參數*/
    float block_c;
    float block_b;
    float block_a;
    /*兩點Max min上下限存取*/
    uint32_t adc_value_min;
    uint32_t adc_value_max;
    /*2點校正輸出*/
    float slope_value;
};

// 保護點設置
extern uint32_t Protect_12V, Protect_5V;
/*command 結構體*/
// 定义命令-处理函数映射表
typedef struct
{
    const char *commandName;
    CommandHandler handler;
} CommandEntry;

/*********************Flash***************************/
extern int check_data;
// Addr for memory
extern uint32_t Data_5V_Min_Addr;
extern uint32_t Data_5V_Max_Addr;
extern uint32_t Data_12V_Min_Addr;
extern uint32_t Data_12V_Max_Addr;
extern uint32_t Data_OTP_Addr;
extern uint32_t Data_OCP_Addr;
extern uint32_t Data_OVP_Addr;
/*********************Slope method**********************/


#endif
