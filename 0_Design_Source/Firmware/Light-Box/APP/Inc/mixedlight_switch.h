/**
  * @file    mixedlight_switch.h
  * @brief   function prototypes for the mixedlight_switch.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MIXEDLIGHT_SWITCH_H
#define __MIXEDLIGHT_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Adjustment target of the encoder */
typedef enum {
    MODE_Temperature = 0,   // Control parameter A for color temperature
    MODE_Brightness         // Control parameter B for brightness
} EncoderMode_t;

/* Create the encoder queue for mode switching */
void MixedlightSwitch_init(void)
/* Get current adjustment target mode */
EncoderMode_t MixedlightSwitch_UpdateCurrentMode(void);
/* Switch adjustment target mode */
extern QueueHandle_t xEncoderQueue;  // Queue for encoder mode switching

#ifdef __cplusplus
}
#endif

#endif /* __MIXEDLIGHT_SWITCH_H */