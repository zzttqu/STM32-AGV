/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "motor.h"
/* USER CODE BEGIN 0 */
uint8_t speed_receiver[24];
uint8_t speed_reporter[24];
extern Motor_Parameter MOTOR_Parameters[];
uint8_t UART1_RX_BUF[64];
uint8_t UART1_Speed_Flag = 0;
uint8_t UART1_Setting_Flag = 0;
uint8_t UART1_Report_Flag = 0;
uint8_t Motor_Start_Flag = 0;
extern uint8_t direction_Flag;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void USAR_UART_IDLECallback(UART_HandleTypeDef *huart, uint8_t rxlen)
{
  if (huart == &huart1) // 判断是否为串口1产生中断
  {
    memcpy(speed_receiver, UART1_RX_BUF, rxlen); // 将UART1_RX_BUF的数据复制到UART1_RX_Data中，长度是rxlen
    UART_Communicate_Init();
    UART_Receive_Handler();
    rxlen = 0;                                                  // 清除数据长度计数
    HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 重新打开DMA接收
  }
}

void UART_Communicate_Init(void)
{
    // 判断是否是初始化命令
  if (UART1_RX_BUF[0] == 'A')
  {
    // 判断是否发送速度数据
    switch (UART1_RX_BUF[1])
    {
    case active_code:
      UART1_Report_Flag = 1;
      break;
    case deactive_code:
      UART1_Report_Flag = 0;
      break;
    default:
      break;
    }
    // 是否开启电机
    switch (UART1_RX_BUF[2])
    {
    case active_code:
      Motor_Start();
      break;
    case deactive_code:
      Motor_Stop();
      break;
    default:
      break;
    }
    // 接受到了设置的串口消息
    UART1_Setting_Flag = 1;
    memset(UART1_RX_BUF, 0x00, sizeof(UART1_RX_BUF));
  }
}

void UART_Receive_Handler(void)
{
  if (speed_receiver[0] == Header && speed_receiver[15] == Tail) // 0x53  0x45
  {
    uint8_t verify = 0x00;
    for (uint8_t i = 0; i < 14; i++)
    {
      // 第十四位是异或校验位
      verify = speed_receiver[i] ^ verify;
    }
    if (verify == speed_receiver[14])
    {
      // 将数据切分后传入相应变量
      for (uint8_t i = 2; i < 14; i++)
      {
        if (i < 4)
        {
          MOTOR_Parameters[0].preloader.byte[(i % 2) ? 0 : 1] = speed_receiver[i];
        }
        else if (3 < i && i < 6)
        {
          MOTOR_Parameters[1].preloader.byte[(i % 2) ? 0 : 1] = speed_receiver[i];
        }
        else if (5 < i && i < 8)
        {
          MOTOR_Parameters[2].preloader.byte[(i % 2) ? 0 : 1] = speed_receiver[i];
        }
        else if (7 < i && i < 10)
        {
          MOTOR_Parameters[3].preloader.byte[(i % 2) ? 0 : 1] = speed_receiver[i];
        }
        else if (9 < i && i < 14)
        {
          MOTOR_Parameters[(i - 2) % 4].direction_Target = speed_receiver[i];
        }
      }
      printf("收到的速度%d %d %d %d \r\n", MOTOR_Parameters[0].preloader.i_data, MOTOR_Parameters[1].preloader.i_data, MOTOR_Parameters[2].preloader.i_data, MOTOR_Parameters[3].preloader.i_data);
      memset(speed_receiver, 0x00, sizeof(speed_receiver));
      // 接收完数据标志位
      UART1_Speed_Flag = 1;
    }
  }
}

void UART_Report_Handler()
{

  memset(speed_reporter, 0x00, sizeof(speed_reporter));
  speed_reporter[0] = Header;
  for (size_t i = 2; i < 10; i++)
  {
    if (i < 4)
    {
      speed_reporter[i] = MOTOR_Parameters[0].encoder.byte[(i - 2) % 2];
    }
    else if (3 < i && i < 6)
    {
      speed_reporter[i] = MOTOR_Parameters[1].encoder.byte[(i - 2) % 2];
    }
    else if (5 < i && i < 8)
    {
      speed_reporter[i] = MOTOR_Parameters[2].encoder.byte[(i - 2) % 2];
    }
    else if (7 < i && i < 10)
    {
      speed_reporter[i] = MOTOR_Parameters[3].encoder.byte[(i - 2) % 2];
    }
  }
  for (size_t i = 0; i < 10; i++)
  {
    // 第十四位是异或校验位
    speed_reporter[10] = speed_reporter[i] ^ speed_reporter[10];
  }

  speed_reporter[11] = Tail;
  
  HAL_UART_Transmit_DMA(&huart1, speed_reporter, sizeof(speed_reporter));
}

/* USER CODE END 1 */
