/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu zzttqu@gmail.com
 * @LastEditTime: 2023-03-19 21:03:48
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#include "motor.h"

/* Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD; */
Motor_Parameter MOTOR_Parameters[4];
uint8_t direction_Flag = 0;
extern uint8_t Motor_Start_Flag;
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
  MOTORA_STOP;
  MOTORB_STOP;
  MOTORC_STOP;
  MOTORD_STOP;
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

void Change_Direction()
{
  // 修改轮子方向
  (MOTOR_Parameters[0].direction_Target > 0) ? MOTORA_FORWARD : MOTORA_BACKWARD;
  (MOTOR_Parameters[1].direction_Target > 0) ? MOTORB_FORWARD : MOTORB_BACKWARD;
  (MOTOR_Parameters[2].direction_Target > 0) ? MOTORC_FORWARD : MOTORC_BACKWARD;
  (MOTOR_Parameters[3].direction_Target > 0) ? MOTORD_FORWARD : MOTORD_BACKWARD;
}

void Change_Speed()
{
  MOTORA_STOP;
  MOTORB_STOP;
  MOTORC_STOP;
  MOTORD_STOP;
  for (uint8_t i = 0; i < 4; i++)
  {
    // 脉冲太长就直接停定时器了
    if (MOTOR_Parameters[i].preloader.i_data > 10000)
    {
      HAL_TIM_Base_Stop_IT(&MOTOR_Parameters[i].htim_speed);
    }
    else
    { // 不能太短了
      if (MOTOR_Parameters[i].preloader.i_data < 100)
      {
        MOTOR_Parameters[i].preloader.i_data = 100;
      }
      // 修改定时器
      __HAL_TIM_SET_AUTORELOAD(&MOTOR_Parameters[i].htim_speed, MOTOR_Parameters[i].preloader.i_data);
      Motor_Start_Flag = 1;
      HAL_TIM_Base_Start_IT(&MOTOR_Parameters[i].htim_speed);
    }
  }
  /*  已优化
    MOTORA.target = (int)(pai * wheel_r_mm * 1000 / MOTORA.target - 1);
    MOTORB.target = (int)(pai * wheel_r_mm * 1000 / MOTORB.target - 1);
    MOTORC.target = (int)(pai * wheel_r_mm * 1000 / MOTORC.target - 1);
    MOTORD.target = (int)(pai * wheel_r_mm * 1000 / MOTORD.target - 1); */

  /*   __HAL_TIM_SET_AUTORELOAD(&htim6, MOTORA.target);
    __HAL_TIM_SET_AUTORELOAD(&htim7, MOTORB.target);
    __HAL_TIM_SET_AUTORELOAD(&htim8, MOTORC.target);
    __HAL_TIM_SET_AUTORELOAD(&htim1, MOTORD.target); */
}

void Get_Encoder()
{
  // 左右不一样，编码器也要取负哦！
  for (uint8_t i = 0; i < 2; i++)
  {
    // 取是正转还是反转（已经废弃）?
    MOTOR_Parameters[i].direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&MOTOR_Parameters[i].htim_encoder);
    // 取定时器的数值，想了想还是强制转换吧，毕竟改成了50ms就采集一次不会太大
    MOTOR_Parameters[i].encoder.i_data = (short)__HAL_TIM_GET_COUNTER(&MOTOR_Parameters[i].htim_encoder);
    MOTOR_Parameters[i].encoder.i_data = (short)(MOTOR_Parameters[i].encoder.i_data / 4);
  }
  for (uint8_t i = 2; i < 4; i++)
  {
    // 取是正转还是反转（已经废弃）?
    MOTOR_Parameters[i].direction_Now = -(__HAL_TIM_IS_TIM_COUNTING_DOWN(&MOTOR_Parameters[i].htim_encoder));
    // 取定时器的数值，想了想还是强制转换吧，毕竟改成了50ms就采集一次不会太大
    MOTOR_Parameters[i].encoder.i_data = (short)__HAL_TIM_GET_COUNTER(&MOTOR_Parameters[i].htim_encoder);
    MOTOR_Parameters[i].encoder.i_data = -(short)(MOTOR_Parameters[i].encoder.i_data / 4);
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
  for (uint8_t i = 0; i < 4; i++)
  {
    // 清零计数
    __HAL_TIM_SET_COUNTER(&MOTOR_Parameters[i].htim_encoder, 0);
  }
  // printf("X_Speed is %d \r\n Y_Speed is %d \r\n Z_Speed is %d \r\n",
  //        speed_reporter.Y_speed, speed_reporter.X_speed, speed_reporter.Z_speed);
}
