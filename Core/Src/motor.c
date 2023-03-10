/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu zzttqu@gmail.com
 * @LastEditTime: 2023-03-10 14:03:34
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#include "motor.h"

extern Speed_Reporter speed_reporter;
extern Speed_Receiver speed_receiver;
/* Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD; */
Motor_Parameter MOTOR_Parameters[4];
void Motor_Init()
{
  MOTOR_Parameters[0].htim_speed = htim6;
  MOTOR_Parameters[0].htim_encoder = htim2;
  MOTOR_Parameters[1].htim_speed = htim7;
  MOTOR_Parameters[1].htim_encoder = htim3;
  MOTOR_Parameters[2].htim_speed = htim8;
  MOTOR_Parameters[2].htim_encoder = htim4;
  MOTOR_Parameters[3].htim_speed = htim1;
  MOTOR_Parameters[3].htim_encoder = htim5;
}
void Motor_Start()
{
  // 使用it表示有中断回调，但是编码器不需要回调，只要定时读取就行，就不用带it后缀的了。
  for (uint8_t i = 0; i < 4; i++)
  {
    // 开启脉冲定时器
    HAL_TIM_Base_Start_IT(&MOTOR_Parameters[i].htim_speed);
    // 开启编码器计时器
    HAL_TIM_Encoder_Start(&MOTOR_Parameters[i].htim_encoder, TIM_CHANNEL_ALL);
  }
}
void Motor_Stop()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    // 关闭脉冲定时器
    HAL_TIM_Base_Stop_IT(&MOTOR_Parameters[i].htim_speed);
    // 关闭编码器计时器
    HAL_TIM_Encoder_Stop(&MOTOR_Parameters[i].htim_encoder, TIM_CHANNEL_ALL);
    // 清空定时器中断
    __HAL_TIM_CLEAR_IT(&MOTOR_Parameters[i].htim_speed, TIM_IT_UPDATE);
  }
}

void Change_Speed()
{
  float tmp[4];
  // 车轮运动学逆解算
  // 转化为个轮子的线速度值，单位mm/s
  // 左上 左下 右下 右上 四个轮子都是朝着y轴正方向
  MOTOR_Parameters[0].target = speed_receiver.X_speed.f_data + speed_receiver.Y_speed.f_data - speed_receiver.Z_speed.f_data * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[1].target = -speed_receiver.X_speed.f_data + speed_receiver.Y_speed.f_data - speed_receiver.Z_speed.f_data * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[2].target = speed_receiver.X_speed.f_data + speed_receiver.Y_speed.f_data + speed_receiver.Z_speed.f_data * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[3].target = -speed_receiver.X_speed.f_data + speed_receiver.Y_speed.f_data + speed_receiver.Z_speed.f_data * (wheel_center_x + wheel_center_y);
  // 先计算完需要计算的再赋值防止时间差
  for (uint8_t i = 0; i < 4; i++)
  {
    // 要先变为角速度值，再转化为preloader数值
    tmp[i] = (pai * wheel_r_mm * 1000 / MOTOR_Parameters[i].target - 1);
  }

  for (uint8_t i = 0; i < 4; i++)
  {
    // 取绝对值
    MOTOR_Parameters[i].preloader = (tmp[i] > 0) ? tmp[i] : -tmp[i];
    // 方向置位
    MOTOR_Parameters[i].direction_Target = (tmp[i] > 0) ? 1 : -1;
    // 修改定时器
    __HAL_TIM_SET_AUTORELOAD(&MOTOR_Parameters[i].htim_speed, MOTOR_Parameters[i].preloader);
  }
  // 修改轮子方向
  (MOTOR_Parameters[0].direction_Target > 0) ? MOTORA_FORWARD : MOTORA_BACKWARD;
  (MOTOR_Parameters[1].direction_Target > 0) ? MOTORB_FORWARD : MOTORB_BACKWARD;
  (MOTOR_Parameters[2].direction_Target > 0) ? MOTORC_FORWARD : MOTORC_BACKWARD;
  (MOTOR_Parameters[3].direction_Target > 0) ? MOTORD_FORWARD : MOTORD_BACKWARD;

  /*  已优化
    MOTORA.target = (int)(pai * wheel_r_mm * 1000 / MOTORA.target - 1);
    MOTORB.target = (int)(pai * wheel_r_mm * 1000 / MOTORB.target - 1);
    MOTORC.target = (int)(pai * wheel_r_mm * 1000 / MOTORC.target - 1);
    MOTORD.target = (int)(pai * wheel_r_mm * 1000 / MOTORD.target - 1); */

  // 修改计时器

  /*   __HAL_TIM_SET_AUTORELOAD(&htim6, MOTORA.target);
    __HAL_TIM_SET_AUTORELOAD(&htim7, MOTORB.target);
    __HAL_TIM_SET_AUTORELOAD(&htim8, MOTORC.target);
    __HAL_TIM_SET_AUTORELOAD(&htim1, MOTORD.target); */
}

void Get_Encoder()
{
  for (uint8_t i = 0; i < 4; i++)
  {
    // 取是正转还是反转（已经废弃）?
    MOTOR_Parameters[i].direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&MOTOR_Parameters[i].htim_encoder);
    // 取定时器的数值
    MOTOR_Parameters[i].encoder = __HAL_TIM_GET_COUNTER(&MOTOR_Parameters[i].htim_encoder) / 4;
  }

  /*   MOTORA.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
    MOTORB.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
    MOTORC.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
    MOTORD.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim5);

    MOTORA.encoder = __HAL_TIM_GET_COUNTER(&htim2) / 4;
    MOTORB.encoder = __HAL_TIM_GET_COUNTER(&htim3) / 4;
    MOTORC.encoder = __HAL_TIM_GET_COUNTER(&htim4) / 4;
    MOTORD.encoder = __HAL_TIM_GET_COUNTER(&htim5) / 4;

    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    __HAL_TIM_SET_COUNTER(&htim5, 0); */

  speed_reporter.Y_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                   (MOTOR_Parameters[0].encoder + MOTOR_Parameters[1].encoder + MOTOR_Parameters[2].encoder + MOTOR_Parameters[3].encoder) / 4);
  speed_reporter.X_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                   (MOTOR_Parameters[0].encoder - MOTOR_Parameters[1].encoder + MOTOR_Parameters[2].encoder - MOTOR_Parameters[3].encoder) / 4);
  speed_reporter.Z_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                   (-MOTOR_Parameters[0].encoder - MOTOR_Parameters[1].encoder + MOTOR_Parameters[2].encoder + MOTOR_Parameters[3].encoder) / 4 / (wheel_center_x + wheel_center_y));
  for (uint8_t i = 0; i < 4; i++)
  {
    // 清零计数
    __HAL_TIM_SET_COUNTER(&MOTOR_Parameters[i].htim_encoder, 0);
  }
  // printf("X_Speed is %d \r\n Y_Speed is %d \r\n Z_Speed is %d \r\n",
  //        speed_reporter.Y_speed, speed_reporter.X_speed, speed_reporter.Z_speed);
}
