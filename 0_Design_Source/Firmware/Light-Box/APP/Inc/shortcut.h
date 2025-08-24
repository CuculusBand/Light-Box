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
    SHORTCUT_NONE = 0,        // no action
    SHORTCUT_QUICK_OFF,       // immediate off
    SHORTCUT_RESTORE_STATE    // restore saved state
} ShortcutAction_t;

/* Light state structure storing brightness and color temperature for shortcut functionality */
typedef struct {
    float brightness;  // saved brightness (0.0-1.0)
    float cct_level;   // saved cct level (0.0-1.0)
    uint8_t is_valid;        // whether a state is saved (1) or not (0)
} LightState_t;

/* Shortcut button handling structure */
typedef struct {
    uint32_t last_press_time; // last press timestamp
    uint8_t click_count;      // number of clicks detected
    LightState_t saved_state; // saved light state
} ShortcutHandle_t;

/* Initialize the shortcut button handling */
void Shortcut_Init(ShortcutHandle_t *handle);
/* Process button press and return the detected action */
ShortcutAction_t Shortcut_ProcessPress(ShortcutHandle_t *handle);
/* Save the current light state */
void Shortcut_SaveCurrentState(ShortcutHandle_t *handle, float brightness, float cct_level);
/* Retrieve the saved light state */
LightState_t Shortcut_GetSavedState(ShortcutHandle_t *handle);

#ifdef __cplusplus
}
#endif

#endif /* __SHORTCUT_H */