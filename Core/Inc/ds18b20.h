/*
 * @Author: zzttqu zzttqu@gamil.com
 * @Date: 2023-04-03 00:28:36
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-03 14:47:31
 * @FilePath: \Graduation_Project\Core\Inc\ds18b20.h
 * @Description: 
 * 一个大学生的毕业设计
 */
#ifndef DS18B20_H__
#define DS18B20_H__

#include "main.h"

#define DS18B20_DQ_OUT_HIGH HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET)
#define DS18B20_DQ_OUT_LOW HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET)
#define DS18B20_DQ_IN HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)
#define DS18B20_GPIO_Port GPIOB
#define DS18B20_Pin GPIO_PIN_1

uint8_t DS18B20_Init(void);           // 初始化DS18B20
short DS18B20_Get_Temperature(void);      // 获取温度
void DS18B20_Start(void);             // 开始温度转换
void DS18B20_Write_Byte(uint8_t dat); // 写入一个字节
uint8_t DS18B20_Read_Bit(void);      // 读出一个字节
uint8_t DS18B20_Read_Byte(void);      // 读出一个位
uint8_t DS18B20_Check(void);          // 检测是否存在DS18B20
void DS18B20_Rst(void);               // 复位DS18B20

#endif