/**
  * @file    temperature_task.c
  * @brief   Implementation of temperature monitoring task using NTC thermistors
  */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ntc_temperature.h"
#include "beep.h"
#include "usart.h"
//#include "system_queues.h"

/* Private variables ---------------------------------------------------------*/
float warn_temp_lev1 = 69.0f;   // Set warning temperature level 1
float warn_temp_lev2 = 75.0f;   // Set warning temperature level 2
float safe_temp = 80.0f;        // Set maximum safe temperature
int temp_state = 0;             // Temperature state variable, -1: unrealistic low temperature or sensor error, 0: normal, 1: warn1, 2: warn2, 3: overtemp
float temp_ntc1;                // Temperature from NTC1
float temp_ntc2;                // Temperature from NTC2
extern ADC_HandleTypeDef hadc1; // ADC handle defined in main.c
char msg_temp[100];                  // Buffer for UART messages

// Function prototype for temperature checking
// Check if temperature exceeds maximum safe limit
int Overtemp_Check(float temp1, float temp2)
{
    if (temp1 >= safe_temp || temp2 >= safe_temp)
    {
        //Beep_NonBlocking(75);   // Beep for 75ms
        osDelay(30);            // Short delay
        //Beep_NonBlocking(45);   // Beep for 45ms
        return 3; // Over safe temperature
    }
    else if (temp1 >= warn_temp_lev2 || temp2 >= warn_temp_lev2)
    {
        return 2; // Warning level 2
    }
    else if (temp1 >= warn_temp_lev1 || temp2 >= warn_temp_lev1)
    {
        return 1; // Warning level 1
    }
    else if (temp1 <= -50.0 || temp2 <= -50.0)
    {
        return -1; // unrealistic low temperature or sensor error
    }
    else
    {
        return 0; // Normal temperature
    }
}

void Output_Limit_Temp(int state)
{
    switch(state)
    {
        case 3: // Over temperature
            // Implement actions for over temperature, e.g., reduce output power
            break;
        case 2: // Warning level 2
            // Implement actions for warning level 2, e.g., slight reduction in output
            break;
        case 1: // Warning level 1
            // Implement actions for warning level 1, e.g., monitor closely
            break;
        case 0: // Normal temperature
            // Normal operation, no action needed
            break;
        case -1: // Sensor error
        default:
            // No action needed
            break;
    }
}

void Temperature_Task(void *argument)
{
    NTC_Measurement_Config_t ntc1_config, ntc2_config;
    // Load configurations for two NTC channels
    ntc1_config = NTC_ChannelConfig(&hadc1, ADC_CHANNEL_0, 4096, 3.3f, 4700.0f, 10000.0f, 3950.0f);
    ntc2_config = NTC_ChannelConfig(&hadc1, ADC_CHANNEL_1, 4096, 3.3f, 4700.0f, 10000.0f, 3950.0f);
    
    for (;;) {
        // Read temperatures from both NTC channels
        temp_ntc1 = NTC_GetTemperature(&ntc1_config);
        temp_ntc2 = NTC_GetTemperature(&ntc2_config);
        // Update temperature state based on readings
        temp_state = Overtemp_Check(temp_ntc1, temp_ntc2);
        osDelay(10); // Delay for 10ms
        // Send temperature state to PC via UART
        sprintf(msg_temp, "Current Temperature: <1> %f <2> %f;\r\nCurrent State: %d\r\n", temp_ntc1, temp_ntc2, temp_state);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg_temp, strlen(msg_temp), 250);
        osDelay(2000); // Delay for 2 seconds before next reading
    }
}

