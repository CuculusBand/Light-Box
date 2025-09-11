/**
  * @file    shortcut.c
  * @brief   Implementation of shortcut button functions
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "shortcut.h"

/* Private variables ---------------------------------------------------------*/
// Double click detection threshold in milliseconds
#define DOUBLE_CLICK_THRESHOLD_MS 1200

// Initialize the shortcut button handling
/** 
    * @brief    Initialize the shortcut button handling structure
    * @param    handle: Pointer to the ShortcutHandle_t structure to initialize
    * @note     This function should be called during system initialization to set up the shortcut handling.
    */
void Shortcut_Init(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin)
{
    if (handle == NULL) return;
    
    handle->last_press_time = 0;
    handle->click_count = 0;
    handle->waiting_for_click = 0;
    handle->saved_state.is_valid = 0;
    handle->saved_state.brightness = 0.0f;
    handle->saved_state.cct_level = 0.0f;
    handle->last_pin_state = HAL_GPIO_ReadPin(port, pin);
}

// Process button press and return the detected action
/** 
    * @brief    Process a button press event and determine the action
    * @param    handle: Pointer to the ShortcutHandle_t structure
    * @param    port: GPIO port of the shortcut button
    * @param    pin: GPIO pin of the shortcut button
    * @return   Detected ShortcutAction_t based on the button press pattern
    * @note     This function should be called whenever a button press is detected.
    */
ShortcutAction_t Shortcut_ProcessPress(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin)
{
    // Null check
    if (!handle) return SHORTCUT_NONE;
    // Get state from GPIO
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(port, pin);
    // Get current time
    uint32_t current_time = HAL_GetTick();
    // Debounce handling: if state changed, wait for it to stabilize
    if (pin_state != handle->last_pin_state) {
        static uint32_t debounce_start = 0;
        // State changed, start debounce timer
        if (debounce_start == 0) {
            debounce_start = current_time;
        }
        // If state changed for more than 30ms, consider it stable
        if ((current_time - debounce_start) < 30) {
            return SHORTCUT_NONE;
        }
    // After debounce, update last pin state and clear debounce timer
    handle->last_pin_state = pin_state;
    debounce_start = 0;
    }
    // Handle key press
    if (pin_state == GPIO_PIN_RESET) {
        // Check if the system is waiting for the second click
        if (!handle->waiting_for_click) {
            handle->last_press_time = current_time;
            handle->waiting_for_click = 1;
            handle->click_count = 1;    // The first click
            return SHORTCUT_NONE;       // hold on for the second click
        }
        // If this is the second click system is waiting for, ignore further presses
    // If the key is not pressed, no action needed
    }
    // If the key is released while waiting for the second click
    if (pin_state == GPIO_PIN_SET && handle->waiting_for_click) {
        // When click count is 1, the system have to judge whether this click is the second click
        if (handle->click_count == 1) {
            // Check if within double click threshold
            if (current_time - handle->last_press_time <= DOUBLE_CLICK_THRESHOLD_MS) {
                handle->click_count = 2;    // 2 clicks detected
                handle->waiting_for_click = 0;  // Stop waiting for clicks
                return SHORTCUT_RESTORE_STATE;  // Double click action
            } else {
                // Timeout for double click, return one click
                handle->waiting_for_click = 0;
                return SHORTCUT_QUICK_OFF;
            }
        }
    }
    // If timeout waiting for the second click
    if (handle->waiting_for_click && (current_time - handle->last_press_time > DOUBLE_CLICK_THRESHOLD_MS)) {
        handle->waiting_for_click = 0;
        return SHORTCUT_QUICK_OFF;
    }
    return SHORTCUT_NONE;
}

// Save the current light state
/** 
    * @brief    Save the current light state (brightness and color temperature)
    * @param    handle: Pointer to the ShortcutHandle_t structure
    * @param    brightness: Current brightness level (0.0 to 1.0)
    * @param    cct_level: Current color temperature level (0.0 to 1.0)
    * @note     This function should be called when the light state needs to be saved.
    */
void Shortcut_SaveCurrentState(ShortcutHandle_t *handle, float brightness, float cct_level)
{
    // Null check
    if (handle == NULL) return;
    
    handle->saved_state.brightness = brightness;
    handle->saved_state.cct_level = cct_level;
    handle->saved_state.is_valid = 1;
}

// Retrieve the saved light state
/** 
    * @brief    Retrieve the saved light state
    * @param    handle: Pointer to the ShortcutHandle_t structure
    * @return   LightState_t structure containing the saved state
    * @note     This function can be called to get the last saved light state.
    */
LightState_t Shortcut_GetSavedState(ShortcutHandle_t *handle)
{
    // Create an invalid state to return if handle is null
    LightState_t invalid_state = {0, 0, 0};
    invalid_state.is_valid = 0;

    // Null check
    if (handle == NULL) return invalid_state;
    
    // Return saved state
    return handle->saved_state;
}