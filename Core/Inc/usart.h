/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
extern uint8_t UART1_RX_BUF[];  //串口接收缓存
extern uint8_t UART1_RX_LEN;  //接收到的数据量
extern uint8_t UART1_RX_Data[];  //数据缓存
extern uint8_t UART1_Recieve_Flag;
extern uint8_t UART1_Report_Flag;
#define UART1_RX_SIZE 128
#define Header 'S'
#define Tail 'E'
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USAR_UART_IDLECallback(UART_HandleTypeDef *huart, uint8_t rxlen);
short XYZ_Target_Speed_transition(uint8_t High, uint8_t Low);
void UART_Receive_Handler(void);
void UART_Report_Handler(void);
void UART_Report_Init(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

