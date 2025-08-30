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
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "timers.h"

/* Define beep start operaiton */
#define BEEP_ON()     HAL_GPIO_WritePin(Beep_Ctrl_GPIO_Port, Beep_Ctrl_Pin, GPIO_PIN_SET)
/* Define beep stop operaiton */
#define BEEP_OFF()    HAL_GPIO_WritePin(Beep_Ctrl_GPIO_Port, Beep_Ctrl_Pin, GPIO_PIN_RESET)

/* Private variables ---------------------------------------------------------*/
extern osTimerId beepTimerId;

/* Directly set beep state */
void Beep_SetState(uint8_t state);
/* Blocking beep operation */
void Beep_Blocking(uint32_t duration_ms);
/* Non-blocking beep operation */
//void Beep_NonBlocking(Beep_OSHandleTypeDef *hbeep, uint32_t duration_ms);
void Beep_NonBlocking(uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif /* __BEEP_H */
