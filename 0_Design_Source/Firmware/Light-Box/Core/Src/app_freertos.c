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
static ShortcutHandle_t shortcut_handle;    // shortcut handler state
static EncoderMode_t last_mode = MODE_Temperature;
/* USER CODE END Variables */
osThreadId MainTaskHandle;
osThreadId AdjustLightHandle;
osThreadId AdjustTargetHandle;
osThreadId ShortcutHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// Callback function of Encoder
static void Encoder_AppCallback(float new_brightness, float new_cct_level)
{
    PWM_App_Update(new_brightness, new_cct_level);
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
  // Create a timer for non-blocking beep
  Beep_NonBlocking_Init();
  // Config temperature channels
  Temperature_Channel_Config(NULL);
  // Initialization finished
  osDelay(100);
  sprintf(msg_task_init0, "main task...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init0, strlen(msg_task_init0), 100);
  osDelay(20);
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
    Temperature_Task(NULL);
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
  // Register Encoder callback
  Encoder_RegisterCallback(Encoder_AppCallback);
  // Create a queue to transfer the encoder's mode
  MixedlightSwitch_init();
  // Initialize encoder
  Encoder_Init(&htim2);
  // Initialize PWM application
  PWM_App_Init();
  // Initialization finished
  sprintf(msg_task_init1, "AdjustLightOutput...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init1, strlen(msg_task_init1), 500);
  osDelay(5);
  /* Infinite loop */
  for(;;)
  {
    // Process encoder action
    Encoder_Update(&htim2);
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
  osDelay(10);
  // Get the latest mode
  last_mode = MixedlightSwitch_GetCurrentMode();
  // Initialization finished
  sprintf(msg_task_init2, "AdjustTargetChange...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init2, strlen(msg_task_init2), 500);
  osDelay(5);
  /* Infinite loop */
  for(;;)
  {
    MixedlightSwitch_UpdateCurrentMode();
    EncoderMode_t current = MixedlightSwitch_GetCurrentMode();
    // Beep if the encoder's mode changed
    if (current != last_mode) {
      // Update the latest mode
      last_mode = current;
      Beep_NonBlocking(10);
    }
    osDelay(10);
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
  Shortcut_Init(&shortcut_handle);
  // Initialization finished
  sprintf(msg_task_init3, "Shortcut...\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 500);
  osDelay(5);
  // Read the initial state of the mode change switch
  GPIO_PinState last_pin = HAL_GPIO_ReadPin(Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin);
  /* Infinite loop */
  for(;;)
  {
    // Detect key's status
    GPIO_PinState current_pin = HAL_GPIO_ReadPin(Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin);
    // The key is pressed
    if (current_pin == GPIO_PIN_RESET && last_pin != GPIO_PIN_RESET) {
      osDelay(30); // Debounce
      // Check if the key is still pressed
      if (HAL_GPIO_ReadPin(Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin) == GPIO_PIN_RESET) {
        // Wait for release
        while (HAL_GPIO_ReadPin(Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin) == GPIO_PIN_RESET) {
          osDelay(5); // Debounce
        }
      // After release, process the action
      ShortcutAction_t action = Shortcut_ProcessPress(&shortcut_handle);
      if (action == SHORTCUT_QUICK_OFF) {
        // Get and save the current state
        float cur_b = Encoder_GetBrightness();
        float cur_c = Encoder_GetCCTLevel();
        Shortcut_SaveCurrentState(&shortcut_handle, cur_b, cur_c);
        // Update PWM and turn off to avoid encoder jitter
        PWM_App_Update(0.0f, cur_c); // keep cct value but brightness 0
        PWM_App_Stop();
        Beep_Blocking(20);
        if(factory == 1) {
          osDelay(5);
          sprintf(msg_task_init3, "QUICK_OFF\r\n");
          HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 500);
        }
      } else if (action == SHORTCUT_RESTORE_STATE) {
        // Get the saved state
        LightState_t state_to_restore = Shortcut_GetSavedState(&shortcut_handle);
        // Restore the saved state and check if valid at first
        if (state_to_restore.is_valid) {
          // Update PWM
          PWM_App_Update(shortcut_handle.saved_state.brightness, shortcut_handle.saved_state.cct_level);
          // Start PWM output
          PWM_App_Init();
          Beep_Blocking(20);
          if(factory == 1) {
            osDelay(5);
            sprintf(msg_task_init3, "RESTORE_STATE\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 500);
          }
        } else {  // Error beep if no valid state
          Beep_Blocking(50);
          osDelay(25);
          Beep_Blocking(12);

          if(factory == 1) {
            osDelay(5);
            sprintf(msg_task_init3, "RESTORE_STATE\r\n");
            HAL_UART_Transmit(&huart1, (uint8_t*)msg_task_init3, strlen(msg_task_init3), 500);
          }
        }
      } else {
        // No action for SHORTCUT_NONE
      }
    }
      current_pin = HAL_GPIO_ReadPin(Mode_Change_SW_GPIO_Port, Mode_Change_SW_Pin);
    }
    last_pin = current_pin;
    osDelay(20);
  }
  /* USER CODE END Run_Shortcut */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

