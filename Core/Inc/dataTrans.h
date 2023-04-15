/*
 * @Author: zzttqu zzttqu@gamil.com
 * @Date: 2023-03-30 20:57:48
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-04 14:13:59
 * @FilePath: \Graduation_Project\Core\Inc\dataTrans.h
 * @Description: 
 * 一个大学生的毕业设计
 */

typedef struct _Motor_Parameter_
{
  short preloader;
  TIM_HandleTypeDef htim_speed;
  int8_t direction_Target;
  short encoder;
  TIM_HandleTypeDef htim_encoder;
  int8_t direction_Now;
  uint8_t INA226_ADDR;
  short voltage;
  short current;
  short temperature;
} Motor_Parameter;
typedef struct _System_Status_
{
  short battery_temperature;
} System_Status;
