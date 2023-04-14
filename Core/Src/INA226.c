/*
 * @Author: zzttqu zzttqu@gamil.com
 * @Date: 2023-04-03 16:04:52
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-14 19:58:43
 * @FilePath: \Graduation_Project\Core\Src\INA226.c
 * @Description:
 * 一个大学生的毕业设计
 */
#include "INA226.h"
#include "main.h"

short bus_voltage, shunt_voltage, current, power;
uint16_t bus_voltage_raw, shunt_voltage_raw, current_raw, power_raw;
uint8_t bus_voltage_raw_data[2], shunt_voltage_raw_data[2], current_raw_data[2], power_raw_data[2];
void INA226_Init(uint16_t INA226_ADDR)
{
    // 设置转换时间588us,求平均值次数128，
    // 采样时间为588*128,75ms采集一次数据，设置模式为分流和总线连续模式
    uint8_t config[] = {0x48,0xdf};
    uint8_t cal[] = {0x04,0x00};
    HAL_I2C_Mem_Write(&hi2c1, INA226_ADDR, CFG_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&config, 2, 100);
    // CAL在current_lsb为0.5uA，电阻为0.01欧的时候为1024
    HAL_I2C_Mem_Write(&hi2c1, INA226_ADDR, CAL_REG, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&cal, 2, 100); // 设置分流电压转电流转换参数
}
int INA226_Get_AND_REPORT(Motor_Parameter data)
{
    uint16_t INA226_ADDR=data.INA226_ADDR;
    HAL_I2C_Mem_Read(&hi2c1, INA226_ADDR, BV_REG, I2C_MEMADD_SIZE_8BIT, bus_voltage_raw_data, 2, 100);
    HAL_I2C_Mem_Read(&hi2c1, INA226_ADDR, SV_REG, I2C_MEMADD_SIZE_8BIT, shunt_voltage_raw_data, 2, 100);
    HAL_I2C_Mem_Read(&hi2c1, INA226_ADDR, CUR_REG, I2C_MEMADD_SIZE_8BIT, current_raw_data, 2, 100);
    HAL_I2C_Mem_Read(&hi2c1, INA226_ADDR, PWR_REG, I2C_MEMADD_SIZE_8BIT, power_raw_data, 2, 100);
    bus_voltage_raw = ((uint16_t)bus_voltage_raw_data[0] << 8 | bus_voltage_raw_data[1]);
    shunt_voltage_raw = ((uint16_t)shunt_voltage_raw_data[0] << 8 | shunt_voltage_raw_data[1]);
    current_raw = ((uint16_t)current_raw_data[0] << 8 | current_raw_data[1]);
    power_raw = ((uint16_t)power_raw_data[0] << 8 | power_raw_data[1]);
    bus_voltage = bus_voltage_raw * 1.25; // 单位是mV
    current = current_raw * 0.5;          // 单位是mA
    power = current * bus_voltage;
    data.current = current;
    data.voltage = bus_voltage;
    return 1;
}
