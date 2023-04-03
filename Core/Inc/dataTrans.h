/*
 * @Author: zzttqu zzttqu@gamil.com
 * @Date: 2023-03-30 20:57:48
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-03 18:58:11
 * @FilePath: \Graduation_Project\Core\Inc\dataTrans.h
 * @Description: 
 * 一个大学生的毕业设计
 */
typedef union _uart_Short_
{
    short i_data;
    uint8_t byte[2];
} uart_Short;
typedef struct _Motor_Parameter_
{
  uart_Short preloader;
  TIM_HandleTypeDef htim_speed;
  int8_t direction_Target;
  uart_Short encoder;
  TIM_HandleTypeDef htim_encoder;
  int8_t direction_Now;
  uint16_t INA226_ADDR;
  uart_Short voltage;
  uart_Short current;
  uart_Short temperature;
} Motor_Parameter;
typedef struct _System_Status_
{
  uart_Short battery_temperature;
} System_Status;
