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

/* USER CODE BEGIN 0 */
extern Speed_Receiver speed_receiver;
extern Speed_Reporter speed_reporter;
uint8_t UART1_RX_BUF[64];
int UART1_Flag = 0;
uart_Float u;
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
  huart1.Init.BaudRate = 9600;
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

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1)
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
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmarx, hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle, hdmatx, hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

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
    memcpy(speed_receiver.buffer, UART1_RX_BUF, rxlen); // 将UART1_RX_BUF的数据复制到UART1_RX_Data中，长度是rxlen
    UART_Receive_Handler();
    rxlen = 0;                                                  // 清除数据长度计数
    HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 重新打开DMA接收
  }
}

void UART_Receive_Handler(void)
{
  if (speed_receiver.buffer[0] == Header) // 0x53
  {
    if (speed_receiver.buffer[7] == Tail) // 0x45
    {
      speed_receiver.Speed.X_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[1], speed_receiver.buffer[2]);
      speed_receiver.Speed.Y_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[3], speed_receiver.buffer[4]);
      speed_receiver.Speed.Z_speed = XYZ_Target_Speed_transition(speed_receiver.buffer[5], speed_receiver.buffer[6]);
      printf("收到的速度为%d %d %d", speed_receiver.Speed.X_speed, speed_receiver.Speed.Y_speed, speed_receiver.Speed.Z_speed);
      memset(speed_receiver.buffer, 0x00, sizeof(speed_receiver.buffer));
      //接收完数据标志位
      UART1_Flag = 1;
      HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 再开启接收中断
      return;
    }
  }
  HAL_UART_Receive_DMA(&huart1, UART1_RX_BUF, UART1_RX_SIZE); // 再开启接收中断
}

void UART_Report_Handler()
{
  speed_reporter.Speed.Data_Header = 'S';
  speed_reporter.Speed.Data_Tail = 'E';

  // 四轮计算速度计算三轴速度
  // 赋值到buffer中进行传输
  speed_reporter.buffer[0] = speed_reporter.Speed.Data_Header;
  for (size_t i = 2; i < 14; i++)
  {
    if (i<6)
    {
     speed_reporter.buffer[i]=speed_reporter.Speed.X_speed.byte[(i-2)%4];
    }
    else if (5<i&&i<10)
    {
      speed_reporter.buffer[i]=speed_reporter.Speed.Y_speed.byte[(i-2)%4];
    }
    else if (9<i&&i<14)
    {
      speed_reporter.buffer[i]=speed_reporter.Speed.Z_speed.byte[(i-2)%4];
    }
  }
  speed_reporter.buffer[14] = speed_reporter.Speed.Data_Tail;
  printf("当前的速度为%f %f %f", speed_reporter.Speed.X_speed.f_data, speed_reporter.Speed.Y_speed.f_data, speed_reporter.Speed.Z_speed.f_data);
  HAL_UART_Transmit_DMA(&huart1, speed_reporter.buffer, sizeof(speed_reporter.buffer));
}

short XYZ_Target_Speed_transition(uint8_t High, uint8_t Low)
{
  // 数据转换的中间变量
  short transition;
  // 将高8位和低8位整合成一个16位的short型数据
  transition = ((High << 8) + Low);
  return transition; // 接收单位是mm/s
}

/* USER CODE END 1 */
