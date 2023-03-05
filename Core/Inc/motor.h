#include "main.h"
#include "tim.h"
#define pai 3.14159
#define wheel_r_mm 150
#define encoder_num 1000
#define wheel_center_x 250
#define wheel_center_y 250

typedef struct _Motor_Parameter_
{
  float target;
  int preloader;
  TIM_HandleTypeDef htim;
  uint8_t direction_Target;
  float encoder;
  uint8_t direction_Now;
} Motor_Parameter;

extern Motor_Parameter MOTORD;
extern Motor_Parameter MOTORA;
extern Motor_Parameter MOTORB;
extern Motor_Parameter MOTORC;

void Drive_Motor();

void Get_Encoder();
