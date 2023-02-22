/*
 * @Author: zzttqu
 * @Date: 2023-02-22 23:38:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-02-22 23:51:12
 * @FilePath: \uart\Core\Src\motor.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by ${git_name} email: ${git_email}, All Rights Reserved.
 */

typedef struct _Motor_Parameter_
{
    float target;
    float encoder;
} Motor_Parameter;

Motor_Parameter MOTORA;
Motor_Parameter MOTORB;
Motor_Parameter MOTORC;
Motor_Parameter MOTORD;

void Drive_Motor(float Vx,float Vy,float Vz){
  //车轮运动学逆解算
  MOTORA.target=Vx+Vy;
  MOTORB.target=Vx-Vy;
  MOTORC.target=Vx+Vy;
  MOTORD.target=Vx-Vy;
}

