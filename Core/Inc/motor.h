/*
 * @Author: zzttqu zzttqu@gmail.com
 * @Date: 2023-02-25 23:05:12
 * @LastEditors: zzttqu zzttqu@gamil.com
 * @LastEditTime: 2023-04-03 20:10:24
 * @FilePath: \Graduation_Project\Core\Inc\motor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "main.h"
#include "tim.h"
#include "INA226.h"
#define pai 3.14159
#define wheel_r_mm 150
#define encoder_num 1000
#define wheel_center_x 250
#define wheel_center_y 250
#define MOTOR1_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define MOTOR1_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

#define MOTOR2_FORWARD HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET)
#define MOTOR2_BACKWARD HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_RESET)

#define MOTOR3_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)
#define MOTOR3_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)

#define MOTOR4_FORWARD HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET)
#define MOTOR4_BACKWARD HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET)

#define MOTOR1_SPEED HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5)
#define MOTOR1_STOP HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)

#define MOTOR2_SPEED HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_6)
#define MOTOR2_STOP HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_RESET)

#define MOTOR3_SPEED HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12)
#define MOTOR3_STOP HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)

#define MOTOR4_SPEED HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_5)
#define MOTOR4_STOP HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET)

/* extern Motor_Parameter MOTORD;
extern Motor_Parameter MOTORA;
extern Motor_Parameter MOTORB;
extern Motor_Parameter MOTORC; */
extern Motor_Parameter MOTOR_Parameters[];
void Motor_Init();

void Motor_Start();

void Motor_Stop();

void Change_Speed();

void Change_Direction();

void Get_Encoder();
void Get_INA226();
#endif