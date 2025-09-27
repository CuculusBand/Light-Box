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
#include "shortcut.h"
#include "temperature_monitor.h"
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
char msg_task_init[25];
char msg_task_task0[64];
char msg_task_task1[64];
char msg_task_task2[64];
char msg_task_task3[64];
extern uint16_t adc_buffer[3];
static ShortcutHandle_t shortcut_handle;            // shortcut handler state
static EncoderMode_t last_mode = MODE_Temperature;  // last encoder mode
/* USER CODE END Variables */
osThreadId MainTaskHandle;
osThreadId AdjustLightHandle;
osThreadId AdjustTargetHandle;
osThreadId ShortcutHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// Callback for updating PWM output based on encoder changes
static void Encoder_AppCallback(float new_brightness, float new_cct_level)
{
    PWM_App_Update(new_brightness, new_cct_level);
}
// System initialization function
void SYS_Init(void *argument)
{
  // Register Encoder callback
  Encoder_RegisterCallback(Encoder_AppCallback);
  // Initialize encoder
  Encoder_Init(&htim2);
  // Initialize PWM application
  PWM_App_Init();
  // Initialization finished
  sprintf(msg_task_init, "AdjustLightOutput\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init, strlen(msg_task_init), 500);
  // Delay 10ms
  HAL_Delay(5);
  // Initialization finished
  sprintf(msg_task_init, "AdjustTargetChange\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init, strlen(msg_task_init), 500);
  // Delay 10ms
  HAL_Delay(5);
  // Initialize shortcut button handling
  Shortcut_Init(&shortcut_handle, Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin);
  // Initialization finished
  sprintf(msg_task_init, "Shortcut\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init, strlen(msg_task_init), 500);
  // Delay 10ms
  HAL_Delay(5);
  // Create a timer for non-blocking beep
  Beep_NonBlocking_Init();
  // Config temperature channels
  Temperature_Channel_Config(NULL);
  // Initialization finished
  sprintf(msg_task_init, "main task...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init, strlen(msg_task_init), 100);
}
/* USER CODE END FunctionPrototypes */

void StartMainTask(void const * argument);
void Run_AdjustLightOutput(void const * argument);
void Run_AdjustTargetChange(void const * argument);
void Run_Shortcut(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  SYS_Init(NULL);
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
  /* definition and creation of MainTask */
  osThreadDef(MainTask, StartMainTask, osPriorityHigh, 0, 256);
  MainTaskHandle = osThreadCreate(osThread(MainTask), NULL);

  /* definition and creation of AdjustLight */
  osThreadDef(AdjustLight, Run_AdjustLightOutput, osPriorityAboveNormal, 0, 128);
  AdjustLightHandle = osThreadCreate(osThread(AdjustLight), NULL);

  /* definition and creation of AdjustTarget */
  osThreadDef(AdjustTarget, Run_AdjustTargetChange, osPriorityBelowNormal, 0, 128);
  AdjustTargetHandle = osThreadCreate(osThread(AdjustTarget), NULL);

  /* definition and creation of Shortcut */
  osThreadDef(Shortcut, Run_Shortcut, osPriorityAboveNormal, 0, 128);
  ShortcutHandle = osThreadCreate(osThread(Shortcut), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartMainTask */
/**
  * @brief  Function implementing the MainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMainTask */
void StartMainTask(void const * argument)
{
  /* USER CODE BEGIN StartMainTask */
  osDelay(30);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1500);
    Temperature_Monitor(NULL);
    osDelay(1500);
  }
  /* USER CODE END StartMainTask */
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
  osDelay(5);
  // Initialize PWM application
  PWM_App_Init();
  // Create a queue to transfer the encoder's mode
  MixedlightSwitch_init();
  // Start PWM output
  osDelay(20);
  PWM_App_Start();
  /* Infinite loop */
  for(;;)
  {
    // Process encoder action
    Encoder_Update(&htim2);
    osDelay(50);
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
  osDelay(5);
  // Get the latest mode
  last_mode = MixedlightSwitch_GetCurrentMode();
  /* Infinite loop */
  for(;;)
  {
    MixedlightSwitch_UpdateCurrentMode();
    EncoderMode_t current_mode = MixedlightSwitch_GetCurrentMode();
    // Beep if the encoder's mode changed
    if (current_mode != last_mode) {
      // Update the latest mode
      last_mode = current_mode;
      Beep_NonBlocking(20);
      if(factory == 1) {
        sprintf(msg_task_task2, "last_mode: %d [MODE_Temperature = 0, MODE_Brightness = 1]\r\n", last_mode);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_task2, strlen(msg_task_task2), 500);
      }
    }
    osDelay(20);
  }
  /* USER CODE END Run_AdjustTargetChange */
}

/* USER CODE BEGIN Header_Run_Shortcut */
/**
* @brief Function implementing the Shortcut thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Run_Shortcut */
void Run_Shortcut(void const * argument)
{
  /* USER CODE BEGIN Run_Shortcut */
  osDelay(5);
  /* Infinite loop */
  for(;;)
  {
    ShortcutAction_t action = Shortcut_ProcessPress(&shortcut_handle, Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin);
    // sprintf(msg_task_task3, "Shortcut_ProcessPress %d\r\n", action);
    // HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_task3, strlen(msg_task_task3), 500);
    switch (action) {
      case SHORTCUT_SINGLE_CLICK:
      {
        // Get and save the current state
        float cur_b = Encoder_GetBrightness();
        float cur_c = Encoder_GetCCTLevel();
        Shortcut_SaveCurrentState(&shortcut_handle, cur_b, cur_c);
        // Update PWM and turn off to avoid encoder jitter
        Encoder_Lock();   // lock the encoder
        PWM_App_Stop();   // stop PWM output
        Beep_NonBlocking(40);
        if(factory == 1) {
          sprintf(msg_task_task3, "QUICK_OFF (b_%0.3f c_%0.3f)\r\n", cur_b, cur_c);
          HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_task3, strlen(msg_task_task3), 500);
        }
        break;
      }
      case SHORTCUT_DOUBLE_CLICK:
      {
        // Get the saved state
        LightState_t state_to_restore = Shortcut_GetSavedState(&shortcut_handle);
        // Restore the saved state if valid
        if (state_to_restore.is_valid) {
          // Unlock the encoder
          Encoder_Unlock(&htim2);
          // Update PWM
          PWM_App_Update(shortcut_handle.saved_state.brightness, shortcut_handle.saved_state.cct_level);
          // Start PWM output
          PWM_App_Start();
          Beep_NonBlocking(85);
          if(factory == 1) {
          sprintf(msg_task_task3, "RESTORE_STATE (b_%0.3f c_%0.3f)\r\n", shortcut_handle.saved_state.brightness, shortcut_handle.saved_state.cct_level);
            HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_task3, strlen(msg_task_task3), 500);
          }
        }
        break;
      }
      case SHORTCUT_LONG_PRESS:
      {
      if(factory == 1) {
              sprintf(msg_task_task3, "LONG_PRESS\r\n");
              HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_task3, strlen(msg_task_task3), 500);
            }
      }
      default:
        break; 
      }
    osDelay(10);
  }
  /* USER CODE END Run_Shortcut */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

