typedef union _uart_Short_
{
    short i_data;
    uint8_t byte[2];
} uart_Short;
typedef struct _Motor_Parameter_
{
  uart_Short preloader;
  TIM_HandleTypeDef htim_speed;
  int8_t direction_Target;
  uart_Short encoder;
  TIM_HandleTypeDef htim_encoder;
  int8_t direction_Now;
  uart_Short voltage;
  uart_Short current;
  uart_Short temperature;
} Motor_Parameter;
typedef struct _System_Status_
{
  uart_Short battery_temperature;
} System_Status;
