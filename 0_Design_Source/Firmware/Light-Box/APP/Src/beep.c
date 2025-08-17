/**
  ******************************************************************************
  * @file    beep.c
  * @brief   This file contains all the function prototypes for
  *          the beep.c file
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "beep.h"

/* Beep operations -----------------------------------------------------------*/
/* Directly set beep state */
void Beep_SetState(uint8_t state)
{
    state ? BEEP_ON() : BEEP_OFF();
}

/* Blocking beep operation */
void Beep_Blocking(uint32_t duration_ms)
{
    BEEP_ON();
    HAL_Delay(duration_ms);
    BEEP_OFF();
}

/* Timer callback for beep */
static void Beep_TimerCallback(TimerHandle_t xTimer) {
    BEEP_OFF();
}

/* Non-blocking beep operation */
void Beep_NonBlocking(Beep_OSHandleTypeDef *hbeep, uint32_t duration_ms)
{
    TimerHandle_t beep_timer = xTimerCreate(
        "BeepTimer",                    // name of the timer
        pdMS_TO_TICKS(duration_ms),     // beep duration in ticks
        pdFALSE,                        // single shot timer
        (void *)0,                      // parameter passed to the callback
        Beep_TimerCallback              // set the callback function
    );
    if (buzzer_timer != NULL) {
        BEEP_ON();                    // start beep
        xTimerStart(buzzer_timer, 0);   // start the timer
    }
}