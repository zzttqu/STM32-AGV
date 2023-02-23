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
Speed_Receiver speed_receiver;
Speed_Reporter speed_reporter;


/**
 * @description: 合并串口高低8位数据
 * @param {uint8_t} High
 * @param {uint8_t} Low
 * @return {*}
 */
float XYZ_Target_Speed_transition(uint8_t High,uint8_t Low)
{
	//数据转换的中间变量
	short transition; 
	
	//将高8位和低8位整合成一个16位的short型数据
	transition=((High<<8)+Low); 
	return 
		transition/1000+(transition%1000)*0.001; //单位转换, mm/s->m/s						
}
/**
 * @description: 接收ros速度的串口函数
 * @param {float} *speed
 * @return {*}
 */
int UART_Receive_Handler(void)
{
    if (Uart1_Rx_Cnt >= 255) // 溢出判断
    {
        Uart1_Rx_Cnt = 0;
        memset(speed_receiver.buffer, 0x00, sizeof(speed_receiver.buffer));                         // 临时缓存区清空
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
        speed_receiver.buffer[Uart1_Rx_Cnt++] = aRxBuffer; // 接收数据转存
        if (Uart1_Rx_Cnt == 7)
        {
            Uart1_Rx_Cnt = 0;
            // 判断结束位是E
            if (speed_receiver.buffer[Uart1_Rx_Cnt - 1] == Tail)
            {
                Uart1_RxFlag = 1; // 接收完全部数据就将标志位改为1
                speed_receiver.Speed.X_speed=XYZ_Target_Speed_transition(speed_receiver.buffer[1],speed_receiver.buffer[2]);
                speed_receiver.Speed.Y_speed=XYZ_Target_Speed_transition(speed_receiver.buffer[3],speed_receiver.buffer[4]);
                speed_receiver.Speed.Z_speed=XYZ_Target_Speed_transition(speed_receiver.buffer[5],speed_receiver.buffer[6]);
                HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); // 再开启接收中断
                return 1;
            }
            
        }
    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); // 再开启接收中断
    return 0;
}


/**
 * @description: 四轮速度正向求解
 * @return {*}
 */
void UART_Report_Handler()
{
  speed_reporter.Speed.Data_Header = 'S';
  speed_reporter.Speed.Data_Tail = 'E';

  // 四轮计算速度计算三轴速度
  // 赋值到buffer中进行传输
  speed_reporter.buffer[0] = speed_reporter.Speed.Data_Header;
  speed_reporter.buffer[2] = speed_reporter.Speed.X_speed >> 8;
  speed_reporter.buffer[3] = speed_reporter.Speed.X_speed;
  speed_reporter.buffer[4] = speed_reporter.Speed.Y_speed >> 8;
  speed_reporter.buffer[5] = speed_reporter.Speed.Y_speed;
  speed_reporter.buffer[6] = speed_reporter.Speed.Z_speed >> 8;
  speed_reporter.buffer[7] = speed_reporter.Speed.Z_speed;
  speed_reporter.buffer[8] = speed_reporter.Speed.Data_Tail;
  HAL_UART_Transmit(&huart1,speed_reporter.buffer,sizeof(speed_reporter.buffer),1000);
}