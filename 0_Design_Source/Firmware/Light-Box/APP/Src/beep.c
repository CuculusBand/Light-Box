/**
  * @file    beep.c
  * @brief   Implementation of beep functions
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "beep.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

/* Private variables ---------------------------------------------------------*/
osTimerId beepTimerId;

// Directly set beep state
/**
    * @brief    Set the beep state (on/off)
    * @param    state 1 to turn on the beep, 0 to turn off
    */
void Beep_SetState(uint8_t state)
{
    state ? BEEP_ON() : BEEP_OFF();
}

// Blocking beep
/**
    * @brief    Make a blocking beep for a specified duration
    * @note     This function blocks the execution for the duration of the beep.
    */
void Beep_Blocking(uint32_t duration_ms)
{
    BEEP_ON();
    HAL_Delay(duration_ms);
    BEEP_OFF();
}

// Timer callback for beep
/**
    * @brief    Timer callback to turn off the beep
    * @note     This function is called when the beep duration timer expires.
    */
static void Beep_TimerCallback(void const *argument) {
    (void)argument;
    BEEP_OFF();
}

// Non-blocking beep
/**
    * @brief Make a non-blocking beep for a specified duration
    * @note	This function starts a timer to turn off the beep after the specified duration.
    */
void Beep_NonBlocking(uint32_t duration_ms)
{
    osTimerDef(beepTimer, Beep_TimerCallback);
    // Create the timer for the beep duration
    osTimerId beepTimerId = osTimerCreate(osTimer(beepTimer), osTimerOnce, NULL);
    if (beepTimerId != NULL) {
        BEEP_ON();                              // start beep
        osTimerStart(beepTimerId, duration_ms); // start the timer
    }
}