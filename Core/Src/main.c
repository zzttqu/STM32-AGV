/* USER CODE BEGIN Header */
/*
 * @Author: zzttqu
 * @Date: 2023-01-14 17:14:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-03-07 18:31:50
 * @FilePath: \uart\Core\Src\main.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by zzttqu email: 1161085395@qq.com, All Rights Reserved.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int Sys_Count = 0;
Speed_Receiver speed_receiver = {0};
Speed_Reporter speed_reporter = {0};
extern uint8_t UART1_Speed_Flag;
extern uint8_t UART1_Report_Flag;
extern uint8_t UART1_Setting_Flag;
extern Motor_Parameter MOTOR_Parameters[];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 要点选microlib
/**
 * @description: 重定向printf到串口输出
 * @param {int} ch
 * @param {FILE} *f
 * @return {*}
 */
int fputc(int ch, FILE *f)
{
  // 这里如果用DMA的话不太行，只能输出一个字节
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
  return ch;
}
/**
 * @description: 定时器回调函数
 * @param {TIM_HandleTypeDef} *htim 定时器指令
 * @return {*}
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // 轮速脉冲输出
  if (htim->Instance == TIM6)
  {
    MOTORA_SPEED;
  }
  if (htim->Instance == TIM7)
  {
    MOTORB_SPEED;
  }
  if (htim->Instance == TIM8)
  {
    MOTORC_SPEED;
  }
  if (htim->Instance == TIM1)
  {
    MOTORD_SPEED;
  }
}

void HAL_SYSTICK_Callback(void)
{
  Sys_Count++;
  if (Sys_Count == 50) // 每0.05s传输一次速度数据
  {
    // printf("tim3 output is %d \r\n", (short)__HAL_TIM_GET_COUNTER(&htim3) / 4);
    Get_Encoder();
    if (UART1_Report_Flag)
    {
      UART_Report_Handler();
    }
    Sys_Count = 0;
  }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM1_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  // 用哪个串口，发什么东西，东西长度多少，超时多少ms
  uint8_t activate_text[] = "MCU Activated";
  HAL_UART_Transmit_DMA(&huart1, activate_text, sizeof(activate_text));
  // 用哪个串口，收什么东西，长度多少
  // HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, 1);
  HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE);

  // 使能idle中断
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  // 事先清除定时器中断
  //__HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);

  //  __HAL_TIM_SET_AUTORELOAD(&htim6, 499);
  // 初始化电机所需定时器参数
  Motor_Init();
  /*   HAL_TIM_Base_Start_IT(&htim6);
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim8);
    HAL_TIM_Base_Start_IT(&htim1);
    // 开启编码器计时器
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 循环读取标志位
    // 修改电机是否启动
    if (UART1_Motor_Start_Flag == 1)
    {
      Motor_Start();
    }
    else{
      Motor_Stop();
    }
    // 修改速度
    if (UART1_Speed_Flag == 1)
    {
      Change_Speed();
      UART1_Speed_Flag = 0;
    }
    // 喂看门狗
    if (UART1_Setting_Flag || UART1_Speed_Flag)
    {
      HAL_IWDG_Refresh(&hiwdg);
    }
    // TODO 这段看门狗需要删除，实际使用不能一直喂看门狗
    HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
