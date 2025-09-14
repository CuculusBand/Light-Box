/**
  * @file    temperature_monitor.h
  * @brief   function prototypes for the temperature_monitor.c file
  */

// Define to prevent recursive inclusion -------------------------------------
#ifndef __TEMPERATURE_MONITOR_H
#define __TEMPERATURE_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ntc_temperature.h"
/* Function prototypes -------------------------------------------------------*/
// Evaluate NTC temperature level
int NTC_Temp_Level_Check(float tempN);
// Evaluate system temperature level
int SYS_Temp_Level_Check(float temps[], int num_sensors);
// Limit output based on temperature state
void Output_Temp_Limit(int state);
// Allow other files to get the temperature_limit_factor
float Temperature_Get_Limit(void);
// Config temperature monitoring channels
void Temperature_Channel_Config(void *argument);
// Task of monitoring temperature using NTC thermistors
void Temperature_Monitor(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __TEMPERATURE_MONITOR_H */