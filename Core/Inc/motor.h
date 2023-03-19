/*
 * @Author: zzttqu zzttqu@gmail.com
 * @Date: 2023-02-25 23:05:12
 * @LastEditors: zzttqu zzttqu@gmail.com
 * @LastEditTime: 2023-03-17 23:10:39
 * @FilePath: \uart\Core\Inc\motor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "main.h"
#include "tim.h"
#define pai 3.14159
#define wheel_r_mm 150
#define encoder_num 1000
#define wheel_center_x 250
#define wheel_center_y 250
#define MOTORA_FORWARD HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define MOTORA_BACKWARD HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)

#define MOTORB_FORWARD HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)
#define MOTORB_BACKWARD HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)

#define MOTORC_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define MOTORC_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

#define MOTORD_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
#define MOTORD_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)

#define MOTORA_SPEED HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4)
#define MOTORB_SPEED HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4)
#define MOTORC_SPEED HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0)
#define MOTORD_SPEED HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10)


/* extern Motor_Parameter MOTORD;
extern Motor_Parameter MOTORA;
extern Motor_Parameter MOTORB;
extern Motor_Parameter MOTORC; */
extern Motor_Parameter MOTOR_Parameters[];
void Motor_Init();

void Motor_Start();

void Motor_Stop();

void Change_Speed();

void Get_Encoder();
