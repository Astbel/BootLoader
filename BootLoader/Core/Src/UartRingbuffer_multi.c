/*
 * UartRingbuffer.c
 *
 *  Created on: 10-Jul-2019
 *      Author: Controllerstech
 */
#include "main.h"
#include "UartRingbuffer_multi.h"
#include <string.h>

/*  Define the device uart and pc uart below according to your setup  */

/* put the following in the ISR

extern void Uart_isr (UART_HandleTypeDef *huart);

*/

/**************** =====================================>>>>>>>>>>>> NO chnages after this **********************/
// 執行標記
volatile uint8_t Process_Excecuted_Flag;

ring_buffer rx_buffer1 = {{0}, 0, 0};
ring_buffer tx_buffer1 = {{0}, 0, 0};
ring_buffer rx_buffer2 = {{0}, 0, 0};
ring_buffer tx_buffer2 = {{0}, 0, 0};

ring_buffer *_rx_buffer1;
ring_buffer *_tx_buffer1;
ring_buffer *_rx_buffer2;
ring_buffer *_tx_buffer2;

void store_char(unsigned char c, ring_buffer *buffer);

void Ringbuf_init(void)
{

	Process_Excecuted_Flag = False;
	/*
	buffer1 -> Uart1

	buffer2	-> Uart2
	*/
	_rx_buffer1 = &rx_buffer1;
	_tx_buffer1 = &tx_buffer1;
	_rx_buffer2 = &rx_buffer2;
	_tx_buffer2 = &tx_buffer2;

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	__HAL_UART_ENABLE_IT(device_uart, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(pc_uart, UART_IT_ERR);

	/* Enable the UART Data Register not empty Interrupt */
	__HAL_UART_ENABLE_IT(device_uart, UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(pc_uart, UART_IT_RXNE);
}

void store_char(unsigned char c, ring_buffer *buffer)
{
	int i = (unsigned int)(buffer->head + 1) % UART_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != buffer->tail)
	{
		buffer->buffer[buffer->head] = c;
		buffer->head = i;
	}
}

int Look_for(char *str, char *buffertolookinto)
{
	int stringlength = strlen(str);
	int bufferlength = strlen(buffertolookinto);
	int so_far = 0;
	int indx = 0;
repeat:
	while (str[so_far] != buffertolookinto[indx])
		indx++;
	if (str[so_far] == buffertolookinto[indx])
	{
		while (str[so_far] == buffertolookinto[indx])
		{
			so_far++;
			indx++;
		}
	}

	else
	{
		so_far = 0;
		if (indx >= bufferlength)
			return -1;
		goto repeat;
	}

	if (so_far == stringlength)
		return 1;
	else
		return -1;
}

void GetDataFromBuffer(char *startString, char *endString, char *buffertocopyfrom, char *buffertocopyinto)
{
	int startStringLength = strlen(startString);
	int endStringLength = strlen(endString);
	int so_far = 0;
	int indx = 0;
	int startposition = 0;
	int endposition = 0;

repeat1:
	while (startString[so_far] != buffertocopyfrom[indx])
		indx++;
	if (startString[so_far] == buffertocopyfrom[indx])
	{
		while (startString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == startStringLength)
		startposition = indx;
	else
	{
		so_far = 0;
		goto repeat1;
	}

	so_far = 0;

repeat2:
	while (endString[so_far] != buffertocopyfrom[indx])
		indx++;
	if (endString[so_far] == buffertocopyfrom[indx])
	{
		while (endString[so_far] == buffertocopyfrom[indx])
		{
			so_far++;
			indx++;
		}
	}

	if (so_far == endStringLength)
		endposition = indx - endStringLength;
	else
	{
		so_far = 0;
		goto repeat2;
	}

	so_far = 0;
	indx = 0;

	for (int i = startposition; i < endposition; i++)
	{
		buffertocopyinto[indx] = buffertocopyfrom[i];
		indx++;
	}
}

void Uart_flush(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		memset(_rx_buffer1->buffer, '\0', UART_BUFFER_SIZE);
		_rx_buffer1->head = 0;
	}
	if (uart == pc_uart)
	{
		memset(_rx_buffer2->buffer, '\0', UART_BUFFER_SIZE);
		_rx_buffer2->head = 0;
	}
}

int Uart_peek(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		if (_rx_buffer1->head == _rx_buffer1->tail)
		{
			return -1;
		}
		else
		{
			return _rx_buffer1->buffer[_rx_buffer1->tail];
		}
	}

	else if (uart == pc_uart)
	{
		if (_rx_buffer2->head == _rx_buffer2->tail)
		{
			return -1;
		}
		else
		{
			return _rx_buffer2->buffer[_rx_buffer2->tail];
		}
	}

	return -1;
}

int Uart_read(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
	{
		// if the head isn't ahead of the tail, we don't have any characters
		if (_rx_buffer1->head == _rx_buffer1->tail)
		{
			return -1;
		}
		else
		{
			unsigned char c = _rx_buffer1->buffer[_rx_buffer1->tail];
			_rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
			return c;
		}
	}

	else if (uart == pc_uart)
	{
		// if the head isn't ahead of the tail, we don't have any characters
		if (_rx_buffer2->head == _rx_buffer2->tail)
		{
			return -1;
		}
		else
		{
			unsigned char c = _rx_buffer2->buffer[_rx_buffer2->tail];
			_rx_buffer2->tail = (unsigned int)(_rx_buffer2->tail + 1) % UART_BUFFER_SIZE;
			return c;
		}
	}

	else
		return -1;
}

void Uart_write(int c, UART_HandleTypeDef *uart)
{
	if (c >= 0)
	{
		if (uart == device_uart)
		{
			int i = (_tx_buffer1->head + 1) % UART_BUFFER_SIZE;

			// If the output buffer is full, there's nothing for it other than to
			// wait for the interrupt handler to empty it a bit
			// ???: return 0 here instead?
			while (i == _tx_buffer1->tail)
				;

			_tx_buffer1->buffer[_tx_buffer1->head] = (uint8_t)c;
			_tx_buffer1->head = i;

			__HAL_UART_ENABLE_IT(device_uart, UART_IT_TXE); // Enable UART transmission interrupt
		}

		else if (uart == pc_uart)
		{
			int i = (_tx_buffer2->head + 1) % UART_BUFFER_SIZE;

			// If the output buffer is full, there's nothing for it other than to
			// wait for the interrupt handler to empty it a bit
			// ???: return 0 here instead?
			while (i == _tx_buffer2->tail)
				;

			_tx_buffer2->buffer[_tx_buffer2->head] = (uint8_t)c;
			_tx_buffer2->head = i;

			__HAL_UART_ENABLE_IT(pc_uart, UART_IT_TXE); // Enable UART transmission interrupt
		}
	}
}

int IsDataAvailable(UART_HandleTypeDef *uart)
{
	if (uart == device_uart)
		return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer1->head - _rx_buffer1->tail) % UART_BUFFER_SIZE;
	else if (uart == pc_uart)
		return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer2->head - _rx_buffer2->tail) % UART_BUFFER_SIZE;
	return -1;
}

int Get_after(char *string, uint8_t numberofchars, char *buffertosave, UART_HandleTypeDef *uart)
{

	while (Wait_for(string, uart) != 1)
		;
	for (int indx = 0; indx < numberofchars; indx++)
	{
		while (!(IsDataAvailable(uart)))
			;
		buffertosave[indx] = Uart_read(uart);
	}
	return 1;
}

void Uart_sendstring(const char *s, UART_HandleTypeDef *uart)
{
	while (*s != '\0')
		Uart_write(*s++, uart);
}

void Uart_printbase(long n, uint8_t base, UART_HandleTypeDef *uart)
{
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *s = &buf[sizeof(buf) - 1];

	*s = '\0';

	// prevent crash if called with base == 1
	if (base < 2)
		base = 10;

	do
	{
		unsigned long m = n;
		n /= base;
		char c = m - base * n;
		*--s = c < 10 ? c + '0' : c + 'A' - 10;
	} while (n);

	while (*s)
		Uart_write(*s++, uart);
}

int Copy_upto(char *string, char *buffertocopyinto, UART_HandleTypeDef *uart)
{
	int so_far = 0;
	int len = strlen(string);
	int indx = 0;

again:
	while (!IsDataAvailable(uart))
		;
	while (Uart_peek(uart) != string[so_far])
	{
		buffertocopyinto[indx] = _rx_buffer1->buffer[_rx_buffer1->tail];
		_rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
		indx++;
		while (!IsDataAvailable(uart))
			;
	}
	while (Uart_peek(uart) == string[so_far])
	{
		so_far++;
		buffertocopyinto[indx++] = Uart_read(uart);
		if (so_far == len)
			return 1;
		while (!IsDataAvailable(uart))
			;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len)
		return 1;
	else
		return -1;
}

int Wait_for(char *string, UART_HandleTypeDef *uart)
{
	int so_far = 0;
	int len = strlen(string);

again_device:
	while (!IsDataAvailable(uart))
		;
	if (Uart_peek(uart) != string[so_far])
	{
		_rx_buffer1->tail = (unsigned int)(_rx_buffer1->tail + 1) % UART_BUFFER_SIZE;
		goto again_device;
	}
	while (Uart_peek(uart) == string[so_far])
	{
		so_far++;
		Uart_read(uart);
		if (so_far == len)
			return 1;
		while (!IsDataAvailable(uart))
			;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again_device;
	}

	if (so_far == len)
		return 1;
	else
		return -1;
}

void Uart_isr(UART_HandleTypeDef *huart)
{
	uint32_t isrflags = READ_REG(huart->Instance->SR);
	uint32_t cr1its = READ_REG(huart->Instance->CR1);

	/* if DR is not empty and the Rx Int is enabled */
	if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
	{
		/******************
					 *  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
					 *          error) and IDLE (Idle line detected) flags are cleared by software
					 *          sequence: a read operation to USART_SR register followed by a read
					 *          operation to USART_DR register.
					 * @note   RXNE flag can be also cleared by a read to the USART_DR register.
					 * @note   TC flag can be also cleared by software sequence: a read operation to
					 *          USART_SR register followed by a write operation to USART_DR register.
					 * @note   TXE flag is cleared only by a write to the USART_DR register.

		*********************/
		huart->Instance->SR;				   /* Read status register */
		unsigned char c = huart->Instance->DR; /* Read data register */
		if (huart == device_uart)
		{
			store_char(c, _rx_buffer1); // store data in buffer
		}

		else if (huart == pc_uart)
		{
			store_char(c, _rx_buffer2); // store data in buffer
		}

		return;
	}

	/*If interrupt is caused due to Transmit Data Register Empty */
	if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
	{
		if (huart == device_uart)
		{
			if (tx_buffer1.head == tx_buffer1.tail)
			{
				// Buffer empty, so disable interrupts
				__HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
			}

			else
			{
				// There is more data in the output buffer. Send the next byte
				unsigned char c = tx_buffer1.buffer[tx_buffer1.tail];
				tx_buffer1.tail = (tx_buffer1.tail + 1) % UART_BUFFER_SIZE;

				/******************
				*  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
				*          error) and IDLE (Idle line detected) flags are cleared by software
				*          sequence: a read operation to USART_SR register followed by a read
				*          operation to USART_DR register.
				* @note   RXNE flag can be also cleared by a read to the USART_DR register.
				* @note   TC flag can be also cleared by software sequence: a read operation to
				*          USART_SR register followed by a write operation to USART_DR register.
				* @note   TXE flag is cleared only by a write to the USART_DR register.

				*********************/

				huart->Instance->SR;
				huart->Instance->DR = c;
			}
		}

		else if (huart == pc_uart)
		{
			if (tx_buffer2.head == tx_buffer2.tail)
			{
				// Buffer empty, so disable interrupts
				__HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
			}

			else
			{
				// There is more data in the output buffer. Send the next byte
				unsigned char c = tx_buffer2.buffer[tx_buffer2.tail];
				tx_buffer2.tail = (tx_buffer2.tail + 1) % UART_BUFFER_SIZE;

				/******************
				*  @note   PE (Parity error), FE (Framing error), NE (Noise error), ORE (Overrun
				*          error) and IDLE (Idle line detected) flags are cleared by software
				*          sequence: a read operation to USART_SR register followed by a read
				*          operation to USART_DR register.
				* @note   RXNE flag can be also cleared by a read to the USART_DR register.
				* @note   TC flag can be also cleared by software sequence: a read operation to
				*          USART_SR register followed by a write operation to USART_DR register.
				* @note   TXE flag is cleared only by a write to the USART_DR register.

				*********************/

				huart->Instance->SR;
				huart->Instance->DR = c;
			}
		}
		return;
	}
}

/*** Depreciated For now. This is not needed, try using other functions to meet the requirement ***/
/* get the position of the given string within the incoming data.
 * It returns the position, where the string ends
 */
/*
int16_t Get_position (char *string, UART_HandleTypeDef *uart)
{
  static uint8_t so_far;
  uint16_t counter;
  int len = strlen (string);
  if (uart == device_uart)
  {
	 if (_rx_buffer1->tail>_rx_buffer1->head)
	 {
	  if (Uart_read(device_uart) == string[so_far])
			{
			  counter=UART_BUFFER_SIZE-1;
			  so_far++;
			}
	  else so_far=0;
	 }
	 unsigned int start = _rx_buffer1->tail;
	 unsigned int end = _rx_buffer1->head;
	 for (unsigned int i=start; i<end; i++)
	 {
	  if (Uart_read(device_uart) == string[so_far])
		{
		  counter=i;
		  so_far++;
		}
	  else so_far =0;
	 }
  }
  else if (uart == pc_uart)
  {
	 if (_rx_buffer2->tail>_rx_buffer2->head)
	 {
	  if (Uart_read(pc_uart) == string[so_far])
			{
			  counter=UART_BUFFER_SIZE-1;
			  so_far++;
			}
	  else so_far=0;
	 }
	 unsigned int start = _rx_buffer2->tail;
	 unsigned int end = _rx_buffer2->head;
	 for (unsigned int i=start; i<end; i++)
	 {
	  if (Uart_read(pc_uart) == string[so_far])
		{
		  counter=i;
		  so_far++;
		}
	  else so_far =0;
	 }
  }

	 if (so_far == len)
	 {
	  so_far =0;
	  return counter;
	 }
	 else return -1;
}
*/

/*Here do C# Uart serials  method */

// find C# string method
//
// Parameter:ring_buffer *buffer 接收端主字串
//			char Target[]   目標字串
//
void Search_String(char s[], char out[], uint16_t p, uint16_t l)
{
	int8_t cnt = 0;				// 取值得計數
	int8_t out_last_length = l; // 要寫入的位置
	int8_t j = 0;				// 補 0的計數
	/*strcspn*/
	char *sTmp;
	// 指標致receive端
	sTmp = s;
	// 填入Buffer
	while (cnt < l)
	{
		out[out_last_length] = *(sTmp + p + cnt);
		cnt++;
		out_last_length++;
	}
	// 剩下位數補0
	while (j < l)
	{
		out[j] = '0';
		j++;
	}
}

// 命令处理函数
void Get5VMinCommand(void)
{
	Uart_sendstring("Saving 5V min ADC value to Flash memory\n", pc_uart);
	// 確認地址內資料是否存在
	Data_5V_Min_Addr = Flash_Addr_5V_Min;
	Data_5V_Min_Addr = Flash_Read_Addr_Data_Exit(Data_5V_Min_Addr);
	// 处理 Get_5V_Min 命令储存当前ADC值
	Flash_Write_NUM(Data_5V_Min_Addr, Flash_5V_Min_Hex);

	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}

void Get5VMaxCommand(void)
{
	Uart_sendstring("Saving 5V max ADC value to Flash memory\n", pc_uart);
	// 处理 Get_5V_Max 命令储存当前ADC值
	Data_5V_Max_Addr = Flash_Addr_5V_Max;
	Data_5V_Max_Addr = Flash_Read_Addr_Data_Exit(Data_5V_Max_Addr);
	// 处理 Get_5V_Min 命令储存当前ADC值
	Flash_Write_NUM(Data_5V_Max_Addr, Flash_5V_Max_Hex);

	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}

void Get12VMinCommand(void)
{
	Uart_sendstring("Saving 12V min ADC value to Flash memory\n", pc_uart);
	// 处理 Get_5V_Max 命令储存当前ADC值
	Data_12V_Min_Addr = Flash_Addr_12V_Min;
	Data_12V_Min_Addr = Flash_Read_Addr_Data_Exit(Data_12V_Min_Addr);
	// 处理 Get_5V_Min 命令储存当前ADC值
	Flash_Write_NUM(Data_12V_Min_Addr, Flash_12V_Min_Hex);

	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}

void Get12VMaxCommand(void)
{
	// printf("Saving 12V max ADC value to Flash memory\n");
	Uart_sendstring("Saving 12V max ADC value to Flash memory\n", pc_uart);
	Data_12V_Max_Addr = Flash_Addr_12V_Max;
	Data_12V_Max_Addr = Flash_Read_Addr_Data_Exit(Data_12V_Max_Addr);
	// 处理 Get_5V_Min 命令储存当前ADC值
	Flash_Write_NUM(Data_12V_Max_Addr, Flash_12V_Max_Hex);

	// 清空buffer
	Process_Excecuted_Flag = True;
}

void EraseFlashMemoryCommand(void)
{
	// printf("Erasing Flash memory\n");
	Uart_sendstring("Erasing Flash memory\n", pc_uart);
	// 处理 Erase Flash memory 命令
	Flash_Erase_Sectors(ADDR_FLASH_SECTOR_1, ADDR_FLASH_SECTOR_7);

	// 清空buffer
	Process_Excecuted_Flag = True;
}
/*測試記憶體寫入值*/
void Check_Flash_Memory_Data(void)
{
	Uart_sendstring("Test the write Flash memory\n", pc_uart);
	// Read Flash data
	char buffer[50];
	int value;
	value = Flash_Read_NUM(0x0800C110);
	sprintf(buffer, "data is %d", value);
	Uart_sendstring(buffer, pc_uart);
	// 清空buffer旗標
	Process_Excecuted_Flag = True;
}
/*保護測試事件*/
/*OTP 事件存入*/
void OTP_Protect_Event(void)
{
	Uart_sendstring("Test the OTP TASK\n", pc_uart);
	/*測試用目前暫定當接收到則旗標cnt++*/
	Dyanmic_Portect.OTP = True;
	// 設定地址
	Data_OTP_Addr = Flash_Addr_OTP;
	// 確認地址內是否有值有的話則往後
	Data_OTP_Addr = Flash_Read_Addr_Data_Exit(Data_OTP_Addr);
	// 寫入flash 地址
	Flash_Write_NUM(Data_OTP_Addr, Dyanmic_Portect.OTP);
	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}
/*OCP 事件存入*/
void OCP_Protect_Event(void)
{
	Uart_sendstring("Test the OCP TASK\n", pc_uart);
	/*測試用目前暫定當接收到則旗標cnt++*/
	Dyanmic_Portect.OCP = True;
	// 設定地址
	Data_OCP_Addr = Flash_Addr_OCP;
	// 確認地址內是否有值有的話則往後
	Data_OCP_Addr = Flash_Read_Addr_Data_Exit(Data_OCP_Addr);
	// 寫入flash 地址
	Flash_Write_NUM(Data_OCP_Addr, Dyanmic_Portect.OCP);
	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}
/*OVP 事件存入*/
void OVP_Protect_Event(void)
{
	Uart_sendstring("Test the OVP TASK\n", pc_uart);
	/*測試用目前暫定當接收到則旗標cnt++*/
	Dyanmic_Portect.OVP = True;
	// 設定地址
	Data_OVP_Addr = Flash_Addr_OVP;
	// 確認地址內是否有值有的話則往後
	Data_OVP_Addr = Flash_Read_Addr_Data_Exit(Data_OVP_Addr);
	// 寫入flash 地址
	Flash_Write_NUM(Data_OVP_Addr, Dyanmic_Portect.OVP);
	//  清空buffer旗標
	Process_Excecuted_Flag = True;
}
/**
 * @brief
 * 黑盒子功能 紀錄上一次保護時並且傳遞給GUI上
 * 從所有保護中搜索遍歷後當GUI下達指令後回傳訊息跟新
 * 取止後再用黑盒子造成錯誤
 */
void Black_Box_Write_Message_Status(void)
{
	// Uart buffer
	char buffer[Uart_Buffer];
	// 檢索當前flash層中保護狀態值
	uint32_t otp_boolean = Dyanmic_Portect.OTP;
	uint32_t ocp_boolean = Dyanmic_Portect.OCP;
	uint32_t ovp_boolean = Dyanmic_Portect.OVP;
	// Uart buffer 傳送至serial message 端顯示當前狀態列ex 是什麼保護
	
	sprintf(buffer, "otp is %d, ocp is %d\n", otp_boolean, ocp_boolean);
	Uart_sendstring(buffer, pc_uart);

	// Process_Excecuted_Flag = True;
}

/**Command 窗口 擴充命令在這**/
CommandEntry commandTable[] = {
	{"Get_5V_Min", Get5VMinCommand},
	{"Get_5V_Max", Get5VMaxCommand},
	{"Get_12V_Min", Get12VMinCommand},
	{"Get_12V_Max", Get12VMaxCommand},
	{"Erase Flash memory", EraseFlashMemoryCommand},
	{"Check Flash Data", Check_Flash_Memory_Data},
	// below is  save protect event this only test for serial saving flash
	{"OTP EVENT", OTP_Protect_Event},
	{"OCP EVENT", OCP_Protect_Event},
	{"OVP EVENT", OVP_Protect_Event},
	//
	{"Black Box Status", Black_Box_Write_Message_Status},
	// below is testing the calibration
	{"Test Value is", Serial_Slopping_Method},
	// 添加其他命令...
};

// 執行來自C# Uart的指標函數命令
void ProcessCommand(const char *command)
{
	for (int i = 0; i < sizeof(commandTable) / sizeof(commandTable[0]); i++)
	{
		// 正常搜索字串執行cmd
		if (strcmp(command, commandTable[i].commandName) == 0)
		{
			commandTable[i].handler();
			return;
		}
	}
}

// 查找C# Uart Serial 的Command 指令
void Get_Command_From_C_shrap(void)
{
	if (Process_Excecuted_Flag != True)
	{
		char command_buffer[UART_BUFFER_SIZE];
		// _rx_buffer1->buffer 包含了接收到的命令
		strncpy(command_buffer, (const char *)_rx_buffer2->buffer, UART_BUFFER_SIZE);
		ProcessCommand(command_buffer);

		// 執行完後呼叫清空環型緩衝 指標 以及 buffer
		Reset_Rx_Buffer();
	}
}

/*重制圓形緩衝buffer和head和tail*/
void Reset_Rx_Buffer(void)
{
	// 當cmd執行完就呼叫執行
	memset(_rx_buffer2->buffer, '\0', UART_BUFFER_SIZE);
	_rx_buffer2->head = 0;
	_rx_buffer2->tail = 0;
	Process_Excecuted_Flag = False;
}
