/**
  * @file    shortcut.h
  * @brief   function prototypes for the shortcut.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SHORTCUT_H
#define __SHORTCUT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>

/* Shortcut action types */
typedef enum {
    SHORTCUT_NONE = 0,      // No action
    SHORTCUT_QUICK_OFF,     // Immediate off
    SHORTCUT_RESTORE_STATE, // Restore saved state
    SHORTCUT_LONG_PRESS,    // Long press action
} ShortcutAction_t;

/* Light state structure storing brightness and color temperature for shortcut functionality */
typedef struct {
    float brightness;   // Saved brightness (0.0-1.0)
    float cct_level;    // Saved cct level (0.0-1.0)
    uint8_t is_valid;   // Whether a state is saved (1) or not (0)
} LightState_t;

/* Shortcut button handling structure */
typedef struct {
    uint32_t last_press_time;       // Last press timestamp
    uint8_t click_count;            // Number of clicks detected
    uint8_t waiting_for_click;      // Whether waiting for the next click (1) or not (0)
    GPIO_PinState last_pin_state;   // Last stable pin state
    LightState_t saved_state;       // Saved light state
} ShortcutHandle_t;

/* Initialize the shortcut button handling */
void Shortcut_Init(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin);
/* Process button press and return the detected action */
ShortcutAction_t Shortcut_ProcessPress(ShortcutHandle_t *handle, GPIO_TypeDef *port, uint16_t pin);
/* Save the current light state */
void Shortcut_SaveCurrentState(ShortcutHandle_t *handle, float brightness, float cct_level);
/* Retrieve the saved light state */
LightState_t Shortcut_GetSavedState(ShortcutHandle_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* __SHORTCUT_H */