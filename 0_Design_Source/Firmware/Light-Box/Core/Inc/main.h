/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OverTempLimit TempState
#define VDDA_APPLI 3300
#define Adjustment_Target_SW_Pin GPIO_PIN_9
#define Adjustment_Target_SW_GPIO_Port GPIOB
#define Temp_1_Pin GPIO_PIN_0
#define Temp_1_GPIO_Port GPIOA
#define Temp_2_Pin GPIO_PIN_1
#define Temp_2_GPIO_Port GPIOA
#define GPIO3_Pin GPIO_PIN_3
#define GPIO3_GPIO_Port GPIOA
#define PWM1_Pin GPIO_PIN_4
#define PWM1_GPIO_Port GPIOA
#define Mode_Change_SW_Pin GPIO_PIN_5
#define Mode_Change_SW_GPIO_Port GPIOA
#define PWM2_Pin GPIO_PIN_6
#define PWM2_GPIO_Port GPIOA
#define PWM3_Pin GPIO_PIN_7
#define PWM3_GPIO_Port GPIOA
#define Beep_Ctrl_Pin GPIO_PIN_0
#define Beep_Ctrl_GPIO_Port GPIOB
#define VSENSE_Pin GPIO_PIN_1
#define VSENSE_GPIO_Port GPIOB
#define GPIO6_Pin GPIO_PIN_2
#define GPIO6_GPIO_Port GPIOB
#define ENCODER2_B_Pin GPIO_PIN_6
#define ENCODER2_B_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_9
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_10
#define USART_RX_GPIO_Port GPIOA
#define ENCODER1_A_Pin GPIO_PIN_15
#define ENCODER1_A_GPIO_Port GPIOA
#define ENCODER1_B_Pin GPIO_PIN_3
#define ENCODER1_B_GPIO_Port GPIOB
#define Fan_Ctrl_Pin GPIO_PIN_4
#define Fan_Ctrl_GPIO_Port GPIOB
#define ENCODER2_A_Pin GPIO_PIN_5
#define ENCODER2_A_GPIO_Port GPIOB
#define Panel_Type_Detec_Pin GPIO_PIN_8
#define Panel_Type_Detec_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define current_chip_series "STM32G0xx" // Current chip series
#define factory 1  // 1: factory test mode; 0: normal mode
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
