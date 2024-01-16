#ifndef __VARIABLE_H_
#define __VARIABLE_H_
/************************include**************/
#define LinkList_Receive_Size (1)
/******************結構體***********************/
extern struct PFC_VARIABLES PFC_Variables;
extern struct Allert_Portect Dyanmic_Portect;
extern struct Flash_Dynamic Flash_Memory;

/**********************C# command 指標函數************************/
typedef void (*CommandHandler)(void);
/***********************Flash**********************/
// 測試函數 變數宣告
// enum宣告
typedef enum
{
  UnKnow =-1,
  MENU,
  CHECK_VERSION,
  ERASE_APPLICATION,
  UPDATE_APPLICATION,
  RUN_APPLICATION,
  TEST_CMD
} BootloaderState;

extern BootloaderState currentState;

typedef enum
{
  CHECK_VERSION_CMD = 1,
  ERASE_APPLICATION_CMD,
  UPDATE_APPLICATION_CMD,
  RUN_APPLICATION_CMD,
  INVALID_CMD,
  View_C_Shrap
} UserCommand;

extern UserCommand userCommand;

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
extern uint8_t aFileName[64];
// Addr for memory
extern uint32_t Data_5V_Min_Addr;
extern uint32_t Data_5V_Max_Addr;
extern uint32_t Data_12V_Min_Addr;
extern uint32_t Data_12V_Max_Addr;
extern uint32_t Data_OTP_Addr;
extern uint32_t Data_OCP_Addr;
extern uint32_t Data_OVP_Addr;
/*********************BootLoader**********************/
typedef  void (*pFunction)(void);  //User Applcation
/*********************FW Version**********************/
extern float FW_Code_Number;
extern uint8_t Print_Menu_Message;
extern uint16_t Time_Out_cnt;
/******************Buffer View  Flash Memory******************/
extern uint32_t VIEW_RX_Buffer[UART_BUFFER_SIZE];
extern uint32_t Test_Buffer[LinkList_Receive_Size];
#endif
