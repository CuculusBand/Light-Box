/**
  * @file    encoder.c
  * @brief   Implementation of encoder functions
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "tim.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"
#include "math.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "mixedlight_switch.h"
#include "encoder.h"
#include "pwm_app.h"

// Queue for encoder mode switching
extern QueueHandle_t xEncoderQueue;

/* Private variables ---------------------------------------------------------*/
static float brightness = 0.00f; // 0.0 ~ 1.0 
static float cct_level  = 0.50f; // 0.0 ~ 1.0 
static EncoderMode_t current_mode = MODE_Temperature;
static int16_t last_cnt = 0;    // Last encoder count value
static uint32_t last_tick = 0;  // Last update tick
// Flag to indicate if encoder is locked
static bool encoder_isLocked = false;
// UART for debug
static char msg_encoder[64];

/* * Callback function pointer for encoder updates
 * This will be called with the new brightness and color temperature values
 */
static void (*Encoder_UpdateCallback)(float, float) = NULL;

// Encoder initialization
/**
    * @brief    Initialize the encoder and start counting
    */
void Encoder_Init(TIM_HandleTypeDef *htim_encoder)
{
    if (htim_encoder == NULL) return;   // return if not initialized
    last_cnt = __HAL_TIM_GET_COUNTER(htim_encoder);
    HAL_TIM_Encoder_Start(htim_encoder, TIM_CHANNEL_ALL);
    brightness = 0.00f;
    cct_level = 0.50f;
    current_mode = MODE_Temperature;
}

// Callback registration for encoder updates
/**
    * @brief    Register a callback function to be called when brightness or color temperature changes
    * @note     The callback function should have the signature: void callback(float brightness, float cct_level)
    */
void Encoder_RegisterCallback(EncoderCallback cb) {
    Encoder_UpdateCallback = cb;
}

// Update brightness and color temperature based on encoder input
/**
    * @brief    Update brightness and color temperature based on encoder input and current mode
    * @note     This function should be called periodically to process encoder input.
    */
void Encoder_Update(TIM_HandleTypeDef *htim_encoder)
 {
    // Return if not initialized or locked
    if (htim_encoder == NULL || encoder_isLocked) return;
    // Define variables for adjustment mode 
    EncoderMode_t new_mode;
    // Check if there is a new mode in the queue
    if (xQueueReceive(xEncoderQueue, &new_mode, 0) == pdTRUE) {
        current_mode = new_mode;
    }
    // Create a local variable to hold the current encoder count
    int32_t cnt = __HAL_TIM_GET_COUNTER(htim_encoder);
    int32_t delta = cnt - last_cnt;
    last_cnt = cnt;
    // Process the change in encoder count
    if (delta != 0) {
        uint32_t now = HAL_GetTick();
        uint32_t dt = now - last_tick;
        last_tick = now;
        // set speed factor based on time interval
        float speed_factor = 1.0f;
        if (dt > 0) {
            float FAST_THRESHOLD = 150.0f;  // threshold for fast rotation
            float SCALE = 70.0f;            // scale for speed factor calculation
            speed_factor = expf((float)(FAST_THRESHOLD - dt) / SCALE);
            if (speed_factor < 1.0f) speed_factor = 1.0f;
            if (speed_factor > 10.0f) speed_factor = 10.0f;
        }
        // Calculate and L=limit step size
        float step = ENCODER_BASE_STEP * speed_factor;
        if (step > ENCODER_MAX_STEP) step = ENCODER_MAX_STEP;
        // Calculate change amount
        float delta_step = delta * step;
        // Update brightness or color temperature based on current mode
        if (current_mode == MODE_Brightness) {
            brightness += delta_step;
            if (brightness > BRIGHTNESS_MAX) brightness = BRIGHTNESS_MAX;
            if (brightness < BRIGHTNESS_MIN) brightness = BRIGHTNESS_MIN;
        } else if (current_mode == MODE_Temperature) {
            cct_level += delta_step;
            if (cct_level > CCT_LEVEL_MAX) cct_level = CCT_LEVEL_MAX;
            if (cct_level < CCT_LEVEL_MIN) cct_level = CCT_LEVEL_MIN;
        }
        // Notify application layer to update PWM output
        if (Encoder_UpdateCallback) {
            // Debug info
            if(factory == 1) {
                sprintf(msg_encoder, "Require: BRI-%0.3f CCT-%0.3f / step: %0.4f\r\n", brightness, cct_level, delta_step);
                HAL_UART_Transmit(&huart1, (uint8_t*)msg_encoder, strlen(msg_encoder), 500);
            }
            // Execute callback
            Encoder_UpdateCallback(brightness, cct_level);
        }
    }
}

// Functions to get current mode and values
float Encoder_GetBrightness(void) {
    return brightness;
}
float Encoder_GetCCTLevel(void) {
    return cct_level;
}
EncoderMode_t Encoder_GetMode(void) {
    return current_mode;
}

// Function to lock or unlock the encoder
void Encoder_Lock(void) {
    encoder_isLocked = true;
}
void Encoder_Unlock(TIM_HandleTypeDef *htim_encoder) {
    encoder_isLocked = false;
    // Reset last count to avoid jump
    last_cnt = __HAL_TIM_GET_COUNTER(htim_encoder);
}
// Function to check if the encoder is active
bool Encoder_isLocked(void) {
    return encoder_isLocked;
}