/**
  * @file    shortcut.c
  * @brief   Implementation of shortcut button functions
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>

#include "shortcut.h"

/* Private variables ---------------------------------------------------------*/
// Double click detection threshold in milliseconds
#define DOUBLE_CLICK_THRESHOLD_MS 300

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
    handle->saved_state.is_valid = 0;
    handle->saved_state.brightness = 0.0f;
    handle->saved_state.cct_level = 0.5f;
}

// Process button press and return the detected action
/** 
    * @brief    Process a button press event and determine the action
    * @param    handle: Pointer to the ShortcutHandle_t structure
    * @return   Detected ShortcutAction_t based on the button press pattern
    * @note     This function should be called whenever a button press is detected.
    */
ShortcutAction_t Shortcut_ProcessPress(ShortcutHandle_t *handle)
{
    // null check
    if (handle == NULL) return SHORTCUT_NONE;
    
    uint32_t current_time = HAL_GetTick(); // when presseed, get current time
    uint32_t time_since_last_press = current_time - handle->last_press_time;
    
    // update last press time
    handle->last_press_time = current_time;
    
    // judge single or double click
    if (time_since_last_press < DOUBLE_CLICK_THRESHOLD_MS) {
        // double click - restore state
        handle->click_count = 2;
        return SHORTCUT_RESTORE_STATE;
    } else {
        // single click - cut off immediately
        handle->click_count = 1;
        return SHORTCUT_QUICK_OFF;
    }
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
    // null check
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
    // create an invalid state to return if handle is null
    LightState_t invalid_state = {0, 0, 0};
    invalid_state.is_valid = 0;

    // null check
    if (handle == NULL) return invalid_state;
    
    // return saved state
    return handle->saved_state;
}