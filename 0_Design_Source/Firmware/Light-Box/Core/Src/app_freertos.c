/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "data_format.h"
#include "beep.h"
#include "encoder.h"
#include "pwm_app.h"
#include "mixedlight_switch.h"
#include "ntc_temperature.h"
// task file
#include "Temperature_Task.h"
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
/* USER CODE BEGIN Variables */
char msg_task_init0[128];
char msg_task_init1[128];
char msg_task_init2[128];
char msg_task_init3[128];
extern uint16_t adc_buffer[3];
/* USER CODE END Variables */
osThreadId mainTaskHandle;
osThreadId AdjustLightHandle;
osThreadId AdjustTargetHandle;
osThreadId OutputModeHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Run_AdjustLightOutput(void const * argument);
void Run_AdjustTargetChange(void const * argument);
void Run_OutputModeChange(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of mainTask */
  osThreadDef(mainTask, StartDefaultTask, osPriorityNormal, 0, 256);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* definition and creation of AdjustLight */
  osThreadDef(AdjustLight, Run_AdjustLightOutput, osPriorityAboveNormal, 0, 128);
  AdjustLightHandle = osThreadCreate(osThread(AdjustLight), NULL);

  /* definition and creation of AdjustTarget */
  osThreadDef(AdjustTarget, Run_AdjustTargetChange, osPriorityBelowNormal, 0, 128);
  AdjustTargetHandle = osThreadCreate(osThread(AdjustTarget), NULL);

  /* definition and creation of OutputMode */
  osThreadDef(OutputMode, Run_OutputModeChange, osPriorityAboveNormal, 0, 256);
  OutputModeHandle = osThreadCreate(osThread(OutputMode), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  sprintf(msg_task_init0, "main task...\r\n");
  Encoder_Init(&htim2); // Initialize encoder interface
  osDelay(10);
  PWM_App_Init();       // Initialize PWM application
  osDelay(200);
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init0, strlen(msg_task_init0), 100);
  osDelay(50);
  /* Infinite loop */
  //Temperature_Task(NULL);
  for(;;)
  {
    // osDelay(250);
    // Beep_Blocking(30);
    Temperature_Task(NULL);
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Run_AdjustLightOutput */
/**
* @brief Function implementing the AdjustLight thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Run_AdjustLightOutput */
void Run_AdjustLightOutput(void const * argument)
{
  /* USER CODE BEGIN Run_AdjustLightOutput */
  osDelay(10);
  sprintf(msg_task_init1, "AdjustLightOutput...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init1, strlen(msg_task_init1), 200);
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END Run_AdjustLightOutput */
}

/* USER CODE BEGIN Header_Run_AdjustTargetChange */
/**
* @brief Function implementing the AdjustTarget thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Run_AdjustTargetChange */
void Run_AdjustTargetChange(void const * argument)
{
  /* USER CODE BEGIN Run_AdjustTargetChange */
  osDelay(40);
  sprintf(msg_task_init2, "AdjustTargetChange...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init2, strlen(msg_task_init2), 200);
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END Run_AdjustTargetChange */
}

/* USER CODE BEGIN Header_Run_OutputModeChange */
/**
* @brief Function implementing the OutputMode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Run_OutputModeChange */
void Run_OutputModeChange(void const * argument)
{
  /* USER CODE BEGIN Run_OutputModeChange */
  osDelay(70);
  sprintf(msg_task_init3, "OutputModeChange...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 200);
  char VBUS_voltage_buf[8];
  int VBUS;
  /* Infinite loop */
  for(;;)
  {
    int VBUS = adc_buffer[0];
    osDelay(20);
    float VBUS_voltage = 3.3/4096*VBUS*6.1;
    float2ascii(VBUS_voltage_buf, VBUS_voltage, 3);
    osDelay(20);
    sprintf(msg_task_init3, "VBUS: %s V (%d)\r\n",VBUS_voltage_buf, VBUS);
    HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 200);
    osDelay(1000);
  }
  /* USER CODE END Run_OutputModeChange */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

