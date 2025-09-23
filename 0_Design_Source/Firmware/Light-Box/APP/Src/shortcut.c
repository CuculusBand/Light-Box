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
#define DOUBLE_CLICK_THRESHOLD_MS 400
#define LONG_PRESS_THRESHOLD_MS   1200
// Default GPIO state
static GPIO_PinState last_state = GPIO_PIN_SET;

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
    // Action of the key
    ShortcutAction_t action = SHORTCUT_NONE;
    // Get state from GPIO
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(port, pin);
    // Get current time
    uint32_t current_time = HAL_GetTick();
    // When the button is pressed
    if (pin_state == GPIO_PIN_RESET && last_state == GPIO_PIN_SET)
    {
        handle->last_press_time = current_time;
        handle->waiting_for_click = 1;
        handle->click_count++;
    }
    // Long press
    if (pin_state == GPIO_PIN_RESET &&
        handle->waiting_for_click)
    {
        if (current_time - handle->last_press_time > LONG_PRESS_THRESHOLD_MS) {
            handle->waiting_for_click = 0;
            handle->click_count = 0;
            action = SHORTCUT_LONG_PRESS;
        }
    }
    // When the button is released
    if (pin_state == GPIO_PIN_SET &&
        last_state == GPIO_PIN_RESET)
    {
        if (handle->click_count == 1)
        {
            // Hold for potential double click
            handle->last_press_time = current_time;
        } else if (handle->click_count == 2) {
            // Double click detected
            handle->waiting_for_click = 0;
            handle->click_count = 0;
            action = SHORTCUT_DOUBLE_CLICK;
        }
    }
    // Timeout for single click
    if (pin_state == GPIO_PIN_SET &&
        handle->waiting_for_click &&
        (current_time - handle->last_press_time > DOUBLE_CLICK_THRESHOLD_MS) &&
        handle->click_count == 1)
    {
        handle->waiting_for_click = 0;
        handle->click_count = 0;
        action = SHORTCUT_SINGLE_CLICK;
    }
    // Timeout for long press
    if (pin_state == GPIO_PIN_RESET &&
        handle->waiting_for_click)
    {
        if (current_time - handle->last_press_time > LONG_PRESS_THRESHOLD_MS) {
            handle->waiting_for_click = 0;
            handle->click_count = 0;
            action = SHORTCUT_LONG_PRESS;
        }
    }
    // Update last state
    last_state = pin_state;
    // return detected action
    return action;
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