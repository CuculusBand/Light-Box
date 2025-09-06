/**
  * @file    temperature_task.h
  * @brief   function prototypes for the temperature_task.c file
  */

// Define to prevent recursive inclusion -------------------------------------
#ifndef __TEMPERATURE_TASK_H
#define __TEMPERATURE_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ntc_temperature.h"
/* Function prototypes -------------------------------------------------------*/
// Check if temperature exceeds maximum safe limit
int Temp_Level_Check(float temp1, float temp2);
// Limit output based on temperature state
void Output_Temp_Limit(int state);
// Allow other files to get the temperature_limit_factor
float Temperature_Get_Limit(void);
// Config temperature monitoring channels
void Temperature_Channel_Config(void *argument);
// Task of monitoring temperature using NTC thermistors
void Temperature_Task(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __TEMPERATURE_TASK_H */