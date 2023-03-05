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

#define MOTORC_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)
#define MOTORC_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)

#define MOTORD_FORWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET)
#define MOTORD_BACKWARD HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET)

typedef struct _Motor_Parameter_
{
  float target;
  int preloader;
  TIM_HandleTypeDef htim_speed;
  uint8_t direction_Target;
  float encoder;
  TIM_HandleTypeDef htim_encoder;
  uint8_t direction_Now;
} Motor_Parameter;

/* extern Motor_Parameter MOTORD;
extern Motor_Parameter MOTORA;
extern Motor_Parameter MOTORB;
extern Motor_Parameter MOTORC; */
extern Motor_Parameter MOTOR_Parameters[];
void Motor_Init();

void Drive_Motor();

void Get_Encoder();
