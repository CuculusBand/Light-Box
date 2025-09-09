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
void Shortcut_Init(ShortcutHandle_t *handle)
{
    if (handle == NULL) return;
    
    handle->last_press_time = 0;
    handle->click_count = 0;
    handle->waiting_for_click = 1;
    handle->saved_state.is_valid = 0;
    handle->saved_state.brightness = 0.0f;
    handle->saved_state.cct_level = 0.5f;
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
    // Set default state - Key released
    static GPIO_PinState last_pin_state = GPIO_PIN_SET; // High level if released
    // Get current time
    uint32_t current_time = HAL_GetTick();
    // Read current pin state
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(port, pin);
    // Key is pressed while released before, judge single or double click
    if (pin_state == GPIO_PIN_RESET && last_pin_state == GPIO_PIN_SET) {
        osDelay(30); // Debounce
        // Check if the key is still pressed
        while (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) {
            osDelay(5);
        }
        // Key released, press detected
        // If this is the first click, update state and wait for another click
        if (handle->waiting_for_click) {
            current_time = HAL_GetTick();           // Get current time
            handle->last_press_time = current_time; // Update last press time
            handle->waiting_for_click = 0;          // First ckick detected, then just wait for the second click
            handle->click_count = 1;                // First click
            last_pin_state = pin_state;             // Update latest pin state
            return SHORTCUT_NONE;                   // No action yet
        // The second click detected, check time interval now
        } else {
            // Update current time
            current_time = HAL_GetTick();
            // Calculate time between two clicks
            if (current_time - handle->last_press_time <= DOUBLE_CLICK_THRESHOLD_MS) {
                handle->waiting_for_click = 1;      // 2 clicks detected and start waiting for next clicks
                handle->click_count = 2;            // 2 clicks detected
                last_pin_state = pin_state;         // Update latest pin state
                return SHORTCUT_RESTORE_STATE;      // Double click action
            } else {
                // Timeout for double click, return one click
                handle->last_press_time = current_time;
                handle->waiting_for_click = 1;      // Wait for double click agian
                handle->click_count = 1;            // First click
                last_pin_state = pin_state;         // Update latest pin state
                return SHORTCUT_QUICK_OFF;          // Single click action
            }
        }
    }
    // No more press detected, check for waiting timeout
    if (handle->waiting_for_click && (current_time - handle->last_press_time > DOUBLE_CLICK_THRESHOLD_MS)) {
        handle->waiting_for_click = 1;      // Wait for double click agian
        handle->click_count = 1;            // First click
        last_pin_state = pin_state;         // Update latest pin state
        return SHORTCUT_QUICK_OFF;          // Single click action
    }
    // Update last pin state
    last_pin_state = pin_state;
    // No action
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