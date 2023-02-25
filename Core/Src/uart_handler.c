/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:51:28
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-02-25 21:56:03
 * @FilePath: \uart\Core\Src\uart_handler.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#include "usart.h"


#define UART1_RX_SIZE 128
#define Header 'S'
#define Tail 'E'

uint8_t UART1_RX_BUF[] = {0}; // 接收缓冲
uint8_t Uart1_Rx_Cnt = 0;     // 接收缓冲计数
uint8_t Uart1_RxFlag = 0;     // 接收完成标志
uint8_t cAlmStr[] = "数据溢出(大于256)\r\n";
extern Speed_Receiver speed_receiver;
extern Speed_Reporter speed_reporter;
/**
 * @description:
 * @param {uint8_t} High
 * @param {uint8_t} Low
 * @return {short}
 */
short XYZ_Target_Speed_transition(uint8_t High, uint8_t Low)
{
    // 数据转换的中间变量
    short transition;
    // 将高8位和低8位整合成一个16位的short型数据
    transition = ((High << 8) + Low);
    return transition; // 接收单位是mm/s
}
/**
 * @description: 接收ros速度的串口函数
 * @param {float} *speed
 * @return {Speed_Receiver}
 */
int UART_Receive_Handler(void)
{
    if (Uart1_Rx_Cnt >= 255) // 溢出判断
    {
        Uart1_Rx_Cnt = 0;
        memset(speed_receiver.buffer, 0x00, sizeof(speed_receiver.buffer));   // 临时缓存区清空
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *)&cAlmStr, sizeof(cAlmStr)); // 抛出异常
    }
    else
    {
        if (speed_receiver.buffer[0] == Header) // 0x53
        {
            if (speed_receiver.buffer[7] == Tail) // 0x45
            {
                Uart1_RxFlag = 1; // 接收完全部数据就将标志位改为1
                speed_receiver.Speed.X_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[1], speed_receiver.buffer[2]);
                speed_receiver.Speed.Y_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[3], speed_receiver.buffer[4]);
                speed_receiver.Speed.Z_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[5], speed_receiver.buffer[6]);
                printf("speeddata receiver complete");
                return 1;
            }
        }
    }
    HAL_UART_Receive_DMA(&huart1,UART1_RX_BUF,UART1_RX_SIZE); // 再开启接收中断
    return 0;
}


/**
 * @description: 四轮速度发布
 * @return {*}
 */
void UART_Report_Handler()
{
    speed_reporter.Speed.Data_Header = 'S';
    speed_reporter.Speed.Data_Tail = 'E';

    // 四轮计算速度计算三轴速度
    // 赋值到buffer中进行传输
    speed_reporter.buffer[0] = speed_reporter.Speed.Data_Header;
    speed_reporter.buffer[2] = speed_reporter.Speed.X_speed >> 8; // 只会截取低八位，所以高八位要右移放入
    speed_reporter.buffer[3] = speed_reporter.Speed.X_speed;
    speed_reporter.buffer[4] = speed_reporter.Speed.Y_speed >> 8;
    speed_reporter.buffer[5] = speed_reporter.Speed.Y_speed;
    speed_reporter.buffer[6] = speed_reporter.Speed.Z_speed >> 8;
    speed_reporter.buffer[7] = speed_reporter.Speed.Z_speed;
    speed_reporter.buffer[9] = speed_reporter.Speed.Data_Tail;
    HAL_UART_Transmit_DMA(&huart1, speed_reporter.buffer, sizeof(speed_reporter.buffer));
}