#include "usart.h"

uint8_t Tx_str1[] = "Hello PC!\r\n";
uint8_t Rx_str1[] = "";
uint8_t Header = 'S';
uint8_t Tail = 'E';
uint8_t aRxBuffer;            // 接收中断缓冲
char Uart1_RxBuff[256] = {0}; // 接收缓冲
uint8_t Uart1_Rx_Cnt = 0;     // 接收缓冲计数
uint8_t Uart1_RxFlag = 0;     // 接收完成标志
uint8_t cAlmStr[] = "数据溢出(大于256)\r\n";
int UARThandler(float *speed)
{
    if (Uart1_Rx_Cnt >= 255) // 溢出判断
    {
        Uart1_Rx_Cnt = 0;
        memset(Uart1_RxBuff, 0x00, sizeof(Uart1_RxBuff));                         // 临时缓存区清空
        HAL_UART_Transmit(&huart1, (uint8_t *)&cAlmStr, sizeof(cAlmStr), 0xFFFF); // 抛出异常
    }
    else
    {
        if (aRxBuffer == 'S')
        {
            Uart1_Rx_Cnt++;
        }
        else
        {
            Uart1_Rx_Cnt = 0;
        }
        Uart1_RxBuff[Uart1_Rx_Cnt++] = aRxBuffer; // 接收数据转存
        if (Uart1_Rx_Cnt == 7)
        {
            Uart1_Rx_Cnt = 0;
            // 判断结束位是E
            if (Uart1_RxBuff[Uart1_Rx_Cnt - 1] == Tail)
            {
                Uart1_RxFlag = 1; // 接收完全部数据就将标志位改为1
                speed[0]=XYZ_Target_Speed_transition(Uart1_RxBuff[1],Uart1_RxBuff[2]);
                speed[1]=XYZ_Target_Speed_transition(Uart1_RxBuff[3],Uart1_RxBuff[4]);
                speed[2]=XYZ_Target_Speed_transition(Uart1_RxBuff[5],Uart1_RxBuff[6]);
                HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); // 再开启接收中断
                return 0;
            }
        }
    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); // 再开启接收中断
}
float XYZ_Target_Speed_transition(uint8_t High,uint8_t Low)
{
	//数据转换的中间变量
	short transition; 
	
	//将高8位和低8位整合成一个16位的short型数据
	transition=((High<<8)+Low); 
	return 
		transition/1000+(transition%1000)*0.001; //单位转换, mm/s->m/s						
}