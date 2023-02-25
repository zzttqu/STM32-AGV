/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-02-25 16:44:01
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */
#define pai 3.14159
typedef struct _Motor_Parameter_
{
  float target;
  uint8_t direction_Target;
  float encoder;
  uint8_t direction_Now;
} Motor_Parameter;

Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD;

Speed_Receiver Drive_Motor(Speed_Receiver speed_receiver)
{
  // 车轮运动学逆解算
  MOTORA.target = speed_receiver.Speed.X_speed - speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (250 + 250);
  // 转化为mm每个轮子的线速度值
  MOTORB.target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed - speed_receiver.Speed.Z_speed * (250 + 250);
  MOTORC.target = speed_receiver.Speed.X_speed - speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (250 + 250);
  MOTORD.target = speed_receiver.Speed.X_speed + speed_receiver.Speed.Y_speed + speed_receiver.Speed.Z_speed * (250 + 250);
  //要先变为角速度值
  MOTORA.target = (int)(pai * 150 * 1000 / MOTORA.target - 1);
  MOTORB.target = (int)(pai * 150 * 1000 / MOTORB.target - 1);
  MOTORC.target = (int)(pai * 150 * 1000 / MOTORC.target - 1);
  MOTORD.target = (int)(pai * 150 * 1000 / MOTORD.target - 1);
  //修改计时器
  __HAL_TIM_SET_AUTORELOAD(&htim6, MOTORA.target);
  __HAL_TIM_SET_AUTORELOAD(&htim7, MOTORB.target);
  __HAL_TIM_SET_AUTORELOAD(&htim8, MOTORC.target);
  __HAL_TIM_SET_AUTORELOAD(&htim1, MOTORD.target);
  return speed_receiver;
}

Speed_Reporter Get_Encoder(Speed_Reporter speed_reporter)
{
  MOTORA.encoder = (short)__HAL_TIM_GET_COUNTER(&htim2) / 4;
  MOTORA.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2);
  __HAL_TIM_SET_COUNTER(&htim2, 0);

  MOTORB.encoder = (short)__HAL_TIM_GET_COUNTER(&htim3) / 4;
  MOTORB.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);
  __HAL_TIM_SET_COUNTER(&htim3, 0);

  MOTORC.encoder = (short)__HAL_TIM_GET_COUNTER(&htim4) / 4;
  MOTORC.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4);
  __HAL_TIM_SET_COUNTER(&htim4, 0);

  MOTORD.encoder = (short)__HAL_TIM_GET_COUNTER(&htim5) / 4;
  MOTORD.direction_Now = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim5);
  __HAL_TIM_SET_COUNTER(&htim5, 0);

  speed_reporter.Speed.Y_speed = (short)(0.47 * (MOTORA.encoder + MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4);
  speed_reporter.Speed.X_speed = (short)(0.47 * (MOTORA.encoder - MOTORB.encoder + MOTORC.encoder - MOTORD.encoder) / 4);
  speed_reporter.Speed.Z_speed = (short)(0.47 * (-MOTORA.encoder - MOTORB.encoder + MOTORC.encoder + MOTORD.encoder) / 4 / (250 + 250));
  __HAL_TIM_SET_COUNTER(&htim2, 0);
  __HAL_TIM_SET_COUNTER(&htim3, 0);
  __HAL_TIM_SET_COUNTER(&htim4, 0);
  __HAL_TIM_SET_COUNTER(&htim5, 0);
  return speed_reporter;
}
