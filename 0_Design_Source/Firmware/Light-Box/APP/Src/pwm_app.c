/**
  * @file    pwm_app.c
  * @brief   Implementation of pwm dimming
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "mixedlight_switch.h"
#include "gpio.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

#include "pwm_app.h"
#include "encoder.h"
#include "temperature_task.h"

/* Private variables ---------------------------------------------------------*/
// Timers for PWM control
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim16;
// UART for debug
static char msg_pwm[64];

// PWM_App Initialization
/**
 * @brief   Initialize PWM for light control
 * @note    This function starts the PWM channels for brightness and color temperature control.
 */
void PWM_App_Init(void)
{
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);
}

// PWM_App Stop
/**
 * @brief   Stop PWM for light control
 * @note    This function stops the PWM channels for brightness and color temperature control.
 */
void PWM_App_Stop(void)
{
    HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim16, TIM_CHANNEL_1);
}

// Update PWM duty cycle
/**
    * @brief    Update PWM duty cycles for brightness and color temperature
    * @note     Warm = (1 - CCT) * Brightness, Cool = CCT * Brightness
    * @param    brightness  [ 0.0 , 1.0 ]
    * @param    cct_level   [ 0.0 , 1.0 ]
    */
void PWM_App_Update(float brightness, float cct_level) {
    float temperature_limit_factor = Temperature_Get_Limit();
    // Check temperature limit
    if(brightness > temperature_limit_factor){
        brightness = temperature_limit_factor;
    }
    // Calculate the parameters of warm and cool channels
    float warm = (1.0f - cct_level) * brightness;
    float cool = cct_level * brightness;
    // Get arr in TIM14 & TIM16
    uint32_t arr14 = __HAL_TIM_GET_AUTORELOAD(&htim14);
    uint32_t arr16 = __HAL_TIM_GET_AUTORELOAD(&htim16);
    // Update Capture Compare register
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, (uint32_t)(warm * arr14));
    __HAL_TIM_SET_COMPARE(&htim16, TIM_CHANNEL_1, (uint32_t)(cool * arr16));
    // Debug info
    if(factory == 1) {
        float B = brightness;
        float C = cct_level;
        sprintf(msg_pwm, "Execute: BRI-%0.3f CCT-%0.3f / W-%0.3f C-%0.3f\r\n", B, C, warm, cool);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_pwm, strlen(msg_pwm), 500);
    }
}