/* USER CODE BEGIN Header */
/*
 * @Author: zzttqu
 * @Date: 2023-01-14 17:14:44
 * @LastEditors: zzttqu 1161085395@qq.com
 * @LastEditTime: 2023-01-23 23:43:23
 * @FilePath: \uart\Core\Src\main.c
 * @Description: 一个大学生的毕业设计
 * Copyright  2023 by zzttqu email: 1161085395@qq.com, All Rights Reserved.
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MicroLib 1
#define INPUTERROR 1
#define UARTTOOLONG 2
#define XIFEN 1600
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Tx_str1[] = "Hello PC!\r\n";
uint8_t Rx_str1[] = "";
uint8_t aRxBuffer;            // 接收中断缓冲
char Uart1_RxBuff[256] = {0}; // 接收缓冲
uint8_t Uart1_Rx_Cnt = 0;     // 接收缓冲计数
uint8_t Uart1_RxFlag = 0;     // 接收完成标志
int PulesCount = 0;           // 记录输出脉冲数量
uint8_t cAlmStr[] = "数据溢出(大于256)\r\n";
uint8_t pulseActivate = 0;
uint8_t errorFlag = 0; // 错误变量
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// 重定向printf到串口输出，只用标准库实现，不用microlib
/* #if !MicroLib
//关闭半主机模
__asm(".global __use_no_semihosting\n\t");
//标准库需要的支持函数 给删
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
  x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
#endif
 */

// 要点选microlib
/**
 * @description: 重定向printf到串口输出
 * @param {int} ch
 * @param {FILE} *f
 * @return {*}
 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/**
 * @description: 定时器回调函数
 * @param {TIM_HandleTypeDef} *htim 定时器指令
 * @return {*}
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    if (pulseActivate)
    {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
      PulesCount++;
    }
  }
}
/**
 * @description: 处理串口输入
 * @param {char} *f 字符
 * @return {*}
 */
int UARThandler(char *f)
{
  if (Uart1_RxFlag == 0)
  {
    return INPUTERROR;
  }
  char speed = f[0];
  char direction = f[1];
  __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
  HAL_TIM_Base_Start(&htim2);
  pulseActivate = 1;
  // 通过比较字符对应的信息调节脉冲的频率（通过调节计时器的reload值）
  if (speed != NULL)
  {
    switch (speed)
    {
    case '1':
      __HAL_TIM_SET_AUTORELOAD(&htim2, 49);
      break;
    case '2':
      __HAL_TIM_SET_AUTORELOAD(&htim2, 249);
      break;
    case '3':
      __HAL_TIM_SET_AUTORELOAD(&htim2, 499);
      break;
    case '0':
      HAL_TIM_Base_Stop(&htim2);
      pulseActivate = 0;
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
      break;
    default:
      errorFlag = INPUTERROR; // 抛出异常
      break;
    }
  }
  if (direction != NULL)
  {
    switch (direction)
    {
    case 'f':
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
      break;
    case 'b':
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_SET);
      break;
    case 'l':
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_2, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_3, GPIO_PIN_SET);
      break;
    case 'r':
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_2, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1 | GPIO_PIN_3, GPIO_PIN_RESET);
      break;
    default:
      errorFlag = INPUTERROR; // 抛出异常
      break;
    }
  }
  //printf("Uart data %s \r\n", Uart1_RxBuff);
  if (errorFlag == 0)
  {
    PulesCount = PulesCount / 2;
    printf("already output %d pulse\r\n", PulesCount); // 输出已发送的脉冲个数
    int jiaodu=PulesCount / XIFEN * 18;
    printf("alread rotate %f jiaodu\r\n", jiaodu);
    printf("now pulse rate is %d Hz\r\n", 1000000 / ((__HAL_TIM_GetAutoreload(&htim2) + 1) * 2)); // 当前脉冲频率
    PulesCount = 0;
    return 0;
  }
  else
  {
    pulseActivate = 0;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_RESET);
    return errorFlag;
  }
}

/**
 * @description: 串口异步接收回调函数
 * @param {UART_HandleTypeDef} *huart 串口地址
 * @return {*}
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    if (Uart1_Rx_Cnt >= 255) // 溢出判断
    {
      Uart1_Rx_Cnt = 0;
      memset(Uart1_RxBuff, 0x00, sizeof(Uart1_RxBuff)); // 临时缓存区清空
      errorFlag = UARTTOOLONG;                          // 抛出异常
      HAL_UART_Transmit(&huart1, (uint8_t *)&cAlmStr, sizeof(cAlmStr), 0xFFFF);
    }
    else
    {
      Uart1_RxBuff[Uart1_Rx_Cnt++] = aRxBuffer; // 接收数据转存
      // 判断结束位也就是换行符！！！
      if ((Uart1_RxBuff[Uart1_Rx_Cnt - 1] == 0x0A) && (Uart1_RxBuff[Uart1_Rx_Cnt - 2] == 0x0D))
      {
        Uart1_RxFlag = 1; // 接收完全部数据就将标志位改为1
      }
    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&aRxBuffer, 1); // 再开启接收中断
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
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  // 用哪个串口，发什么东西，东西长度多少，超时多少ms
  HAL_UART_Transmit(&huart1, Tx_str1, sizeof(Tx_str1), 10000);
  // 用哪个串口，收什么东西，长度多少
  HAL_UART_Receive_IT(&huart1, &aRxBuffer, 1);
  // 事先清除定时器中断
  __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
  // 一定要先开启定时器，可以在设置标志位计数
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (Uart1_RxFlag == 1)
    {
      if (UARThandler(Uart1_RxBuff) == 0)
      {
        Uart1_RxFlag = 0; // 标志位清0
        Uart1_Rx_Cnt = 0; // 计数值清0
      };
      if (errorFlag != 0)
      {
        printf("error %d \r\n", errorFlag);
        printf("Uart data %s \r\n", Uart1_RxBuff);
        errorFlag = 0;
      }
      memset(Uart1_RxBuff, 0x00, 256);
    }
    HAL_Delay(10);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
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
