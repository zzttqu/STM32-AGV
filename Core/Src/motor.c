/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-03-05 15:48:56
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#include "motor.h"

#define MOTORA_FORWARD HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define MOTORA_BACKWARD HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

#define MOTORB_FORWARD HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)
#define MOTORB_BACKWARD HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)

#define MOTORC_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define MOTORC_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)

#define MOTORD_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define MOTORD_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)

extern Speed_Reporter speed_reporter;
extern Speed_Receiver speed_receiver;
Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD;
Motor_Parameter MOTOR_Parameters[4];

void Drive_Motor()
{
  float tmp;
  // 车轮运动学逆解算
  // 转化为个轮子的线速度值，单位mm/s
  MOTOR_Parameters[0].htim=htim6;
  MOTOR_Parameters[1].htim=htim7;
  MOTOR_Parameters[2].htim=htim8;
  MOTOR_Parameters[3].htim=htim1;
  MOTOR_Parameters[0].target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[1].target = -speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[2].target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  MOTOR_Parameters[3].target = -speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (wheel_center_x + wheel_center_y);
  for (size_t i = 0; i < 4; i++)
  {
    // 要先变为角速度值，再转化为preloader数值
    tmp = (pai * wheel_r_mm * 1000 / MOTOR_Parameters[i].target - 1);
    MOTOR_Parameters[i].preloader = (tmp > 0) ? tmp : -tmp;
    MOTOR_Parameters[i].direction_Target = (tmp > 0) ? 1 : -1;

    //修改定时器
    __HAL_TIM_SET_AUTORELOAD(&MOTOR_Parameters->htim, MOTOR_Parameters[i].preloader);
  }
  /*   优化
    MOTORA.target = (int)(pai * wheel_r_mm * 1000 / MOTORA.target - 1);
    MOTORB.target = (int)(pai * wheel_r_mm * 1000 / MOTORB.target - 1);
    MOTORC.target = (int)(pai * wheel_r_mm * 1000 / MOTORC.target - 1);
    MOTORD.target = (int)(pai * wheel_r_mm * 1000 / MOTORD.target - 1); */

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
  MOTORA.encoder = __HAL_TIM_GET_COUNTER(&htim2) / 4;
  MOTORB.encoder = __HAL_TIM_GET_COUNTER(&htim3) / 4;
  MOTORC.encoder = __HAL_TIM_GET_COUNTER(&htim4) / 4;
  MOTORD.encoder = __HAL_TIM_GET_COUNTER(&htim5) / 4;

  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim5, 0);

  speed_reporter.Speed.Y_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                         (MOTORA.encoder + MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4);
  speed_reporter.Speed.X_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                         (MOTORA.encoder - MOTORB.encoder + MOTORC.encoder - MOTORD.encoder) / 4);
  speed_reporter.Speed.Z_speed.f_data = (pai * wheel_r_mm / encoder_num *
                                         (-MOTORA.encoder - MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4 / (wheel_center_x + wheel_center_y));
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim5, 0);
  // printf("X_Speed is %d \r\n Y_Speed is %d \r\n Z_Speed is %d \r\n",
  //        speed_reporter.Speed.Y_speed, speed_reporter.Speed.X_speed, speed_reporter.Speed.Z_speed);
}
