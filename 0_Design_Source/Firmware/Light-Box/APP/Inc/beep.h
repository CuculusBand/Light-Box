/**
  * @file    beep.h
  * @brief   function prototypes for the beep.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BEEP_H
#define __BEEP_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "timers.h"

/* Define beep operaiton -----------------------------------------------------*/
#define BEEP_ON()     HAL_GPIO_WritePin(Beep_Ctrl_GPIO_Port, Beep_Ctrl_Pin, GPIO_PIN_SET)
#define BEEP_OFF()    HAL_GPIO_WritePin(Beep_Ctrl_GPIO_Port, Beep_Ctrl_Pin, GPIO_PIN_RESET)

/* Define beep structure */
typedef struct {
    TimerHandle_t timer_handle;  // get the start time of the beep from the timer
    uint32_t duration_ms; // time for beep duration
    uint8_t is_active;    // beep state
} Beep_OSHandleTypeDef;

/* Directly set beep state */
void Beep_SetState(uint8_t state);
/* Blocking beep operation */
void Beep_Blocking(uint32_t duration_ms);
/* Non-blocking beep operation */
void Beep_NonBlocking(Beep_OSHandleTypeDef *hbeep, uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H */
