/**
  * @file    mixedlight_switch.c
  * @brief   Implementation of mixed light switch functions, changing adjustment target of the encoder: Brightness/Color temperature
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"

#include "mixedlight_switch.h"

/* Private variables ---------------------------------------------------------*/
static volatile EncoderMode_t current_mode = MODE_Temperature;  // Current adjustment target mode
#define MODE_SWITCH_ACTIVE (GPIO_PIN_RESET)                     // GPIO pin state when the mode switch is active
#define DEBOUNCE_DELAY_MS  50                                   // Debounce delay in milliseconds

// Create the encoder queue for mode switching
/**
    * @brief    Initialize the mixed light switch by creating the encoder queue
    * @note     This function should be called during system initialization to set up the queue 
    *           and must be called before using the encoder.
    */
void MixedlightSwitch_init(void)
{
    xEncoderQueue = xQueueCreate(5, sizeof(EncoderMode_t));
}

// Update Current Mode
/**
    * @brief    Update the current adjustment target mode based on button press
    * @note     This function should be called periodically to check the button state.
    */
void MixedlightSwitch_UpdateCurrentMode(void)
{
    static uint32_t last_tick = 0;
    uint32_t current_tick = HAL_GetTick();

    // Detect button press with debounce
    if (HAL_GPIO_ReadPin(Adjustment_Target_SW_GPIO_Port, Adjustment_Target_SW_Pin) == MODE_SWITCH_ACTIVE) {
        if ((current_tick - last_tick) > DEBOUNCE_DELAY_MS) {
            last_tick = current_tick;
            
            // Change mode
            current_mode = (current_mode == MODE_Temperature) ? MODE_Brightness : MODE_Temperature;
            
            // Send the new mode to the queue
            if (xEncoderQueue != NULL) {
                xQueueSend(xEncoderQueue, &current_mode, 0);
            }
        }
    }
}

/* Switch adjustment target */
EncoderMode_t MixedlightSwitch_GetCurrentMode(void) {
    return current_mode;
}