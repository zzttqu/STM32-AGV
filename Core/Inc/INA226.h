/*
 * @Author: zzttqu zzttqu@gamil.com
 * @Date: 2023-04-03 15:49:06
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-15 22:15:43
 * @FilePath: \Graduation_Project\Core\Inc\INA226.h
 * @Description:
 * 一个大学生的毕业设计
 */
#ifndef __INA226_H
#define __INA226_H
#include "main.h"
#include "i2c.h"

#define CFG_REG 0x00         //
#define SV_REG 0x01          // 分流电压， 此处分流电阻为 0.1欧
#define BV_REG 0x02          // 总线电压
#define PWR_REG 0x03         // 电源功率
#define CUR_REG 0x04         // 电流
#define CAL_REG 0x05         // 校准，设定满量程范围以及电流和功率测数的
#define ONFF_REG 0x06        // 屏蔽 使能 警报配置和转换准备就绪
#define AL_REG 0x07          // 包含与所选警报功能相比较的限定值
#define INA226_GET_ADDR 0XFF // 包含唯一的芯片标识号


void INA226_Init(uint8_t INA226_ADDR);
int INA226_Get_AND_REPORT(Motor_Parameter* data);

#endif
