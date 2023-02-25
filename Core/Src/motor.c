/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-02-25 16:44:01
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#include "motor.h"
extern Speed_Reporter speed_reporter;
extern Speed_Receiver speed_receiver;
Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD;

void Drive_Motor()
{
  // 车轮运动学逆解算
  // 转化为个轮子的线速度值，单位mm/s
  MOTORA.target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTORB.target = -speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTORC.target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTORD.target = -speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  // 要先变为角速度值，这个1000和细分有关系
  MOTORA.target = (int)(pai * wheel_r_mm * 1000 / MOTORA.target - 1);
  MOTORB.target = (int)(pai * wheel_r_mm * 1000 / MOTORB.target - 1);
  MOTORC.target = (int)(pai * wheel_r_mm * 1000 / MOTORC.target - 1);
  MOTORD.target = (int)(pai * wheel_r_mm * 1000 / MOTORD.target - 1);
  // 修改计时器
  __HAL_TIM_SET_AUTORELOAD(&htim6, MOTORA.target);
  __HAL_TIM_SET_AUTORELOAD(&htim7, MOTORB.target);
  __HAL_TIM_SET_AUTORELOAD(&htim8, MOTORC.target);
  __HAL_TIM_SET_AUTORELOAD(&htim1, MOTORD.target);
}

void Get_Encoder()
{
  // 取是正转还是反转（已经废弃）
  MOTORA.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
  MOTORB.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
  MOTORC.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
  MOTORD.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim5);
  // 取定时器的数值
  MOTORA.encoder = (short)__HAL_TIM_GET_COUNTER(&htim2) / 4;
  MOTORB.encoder = (short)__HAL_TIM_GET_COUNTER(&htim3) / 4;
  MOTORC.encoder = (short)__HAL_TIM_GET_COUNTER(&htim4) / 4;
  MOTORD.encoder = (short)__HAL_TIM_GET_COUNTER(&htim5) / 4;

  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim5, 0);

  speed_reporter.Speed.Y_speed = (short)(pai * wheel_r_mm / encoder_num *
                                         (MOTORA.encoder + MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4);
  speed_reporter.Speed.X_speed = (short)(pai * wheel_r_mm / encoder_num *
                                         (MOTORA.encoder - MOTORB.encoder + MOTORC.encoder - MOTORD.encoder) / 4);
  speed_reporter.Speed.Z_speed = (short)(pai * wheel_r_mm / encoder_num *
                                         (-MOTORA.encoder - MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4 / (wheel_center_x + wheel_center_y));
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim5, 0);
  //printf("X_Speed is %d \r\n Y_Speed is %d \r\n Z_Speed is %d \r\n",
  //       speed_reporter.Speed.Y_speed, speed_reporter.Speed.X_speed, speed_reporter.Speed.Z_speed);
}
