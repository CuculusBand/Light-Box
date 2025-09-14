/**
  * @file    temperature_monitor.c
  * @brief   Implementation of temperature monitoring task using NTC thermistors
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "data_format.h"
#include "beep.h"
#include "encoder.h"
#include "pwm_app.h"
#include "ntc_temperature.h"
#include "temperature_monitor.h"

/* Private variables ---------------------------------------------------------*/
NTC_Measurement_Config_t ntc1_config, ntc2_config;  // NTC config
extern  ADC_HandleTypeDef hadc1;    // ADC handle defined in main.c
float   warn_temp_lev1 = 55.0f;     // Set warning temperature level 1 - 55
float   warn_temp_lev2 = 68.0f;     // Set warning temperature level 2 - 68
float   safe_temp = 75.0f;          // Set maximum safe temperature - 75
int     temp_state = 0;             // Temperature state variable, -1: unrealistic low temperature or sensor error, 0: normal, 1: warn1, 2: warn2, 3: overtemp
float   temp_ntc1;                  // Temperature from NTC1
float   temp_ntc2;                  // Temperature from NTC2
char    msg_temp[128];                                 // Buffer for UART messages
// Limitation of brightness based on temperature
static float temperature_limit_factor = 1.0f; // 1.0 = 100%
// Last temperature state to avoid redundant UART messages
static int last_state = -2;

// Evaluate NTC temperature level
int NTC_Temp_Level_Check(float tempN)
{
    if (tempN >= safe_temp){
        return 3; // Over safe temperature
    }
    else if (tempN >= warn_temp_lev2){
        return 2; // Warning level 2
    }
    else if (tempN >= warn_temp_lev1){
        return 1; // Warning level 1
    }
    else if (tempN <= -25.0){
        return -1; // unrealistic low temperature or sensor error
    }
    else{
        return 0; // Normal temperature
    }
}

// Evaluate system temperature level
int SYS_Temp_Level_Check(float temps[], int num_sensors)
{
    int num_error_sensors = 0;
    int max_temp_stete = 0;
    if (num_sensors <= 0){
        return -2;  // Check the number of sensors
    }
    for (int j = 0; j < num_sensors; j++){
        int sensor_temp_level = NTC_Temp_Level_Check(temps[j]);
        if (sensor_temp_level == -1){
            num_error_sensors++;
        }
        if(sensor_temp_level > max_temp_stete){
            max_temp_stete = sensor_temp_level;
        }
    }
    return (num_error_sensors > 0) ? -1 : max_temp_stete;
}

// Limit output based on temperature state
void Output_Temp_Limit(int state)
{
    if (state != last_state) {
        last_state = state;
        switch(state){
            case 3: // Over temperature
                temperature_limit_factor = 0.0f;
                break;
            case 2: // Warning level 2
                temperature_limit_factor = 0.55f;
                break;
            case 1: // Warning level 1
                temperature_limit_factor = 0.90f;
                break;
            case 0: // Normal temperature
                // Normal operation, no action needed
                temperature_limit_factor = 1.00f;
                break;
            case -1: // Sensor error
                temperature_limit_factor = 0.45f;
                break;
            case -2: // No sensor
                temperature_limit_factor = 0.50f;
                break;
            default:
                // No action needed
                break;
        }
        // Update PWM output if temeprature state changes
        PWM_App_Update(Encoder_GetBrightness(), Encoder_GetCCTLevel());
    }
}

// Allow other files to get the temperature_limit_factor
float Temperature_Get_Limit(void)
{
    return temperature_limit_factor;
}

// Config temperature monitoring channels
void Temperature_Channel_Config(void *argument)
{
    // Load configurations for two NTC channels
    ntc1_config = NTC_ChannelConfig(&hadc1, ADC_CHANNEL_0, 4096, 1, 3.3f, 4700.0f, 10000.0f, 3950.0f);
    ntc2_config = NTC_ChannelConfig(&hadc1, ADC_CHANNEL_1, 4096, 2, 3.3f, 4700.0f, 10000.0f, 3950.0f);
}

// Temperature monitoring task
void Temperature_Monitor(void *argument)
{
        // Read resistances from both NTC channels
        float resistance1 = NTC_GetResistance(&ntc1_config);
        float resistance2 = NTC_GetResistance(&ntc2_config);
        // Read temperatures from both NTC channels
        temp_ntc1 = NTC_GetTemperature(&ntc1_config);
        temp_ntc2 = NTC_GetTemperature(&ntc2_config);
        // Delay for 2ms
        osDelay(5);
        // Update temperature state based on readings
        float temps[] = {temp_ntc1, temp_ntc2};
        int temp_state = SYS_Temp_Level_Check(temps, 2);
        // Limit output power based on the temp state
        Output_Temp_Limit(temp_state);
        // Delay for 10ms
        osDelay(5);
        // Send temperature state to PC via UART (factory test mode)
        if(factory == 1) {
            sprintf(msg_temp, "Current State: %d -- Current Temperature:\r\n<1>%0.3fΩ %0.2f°C\r\n<2>%0.3fΩ %0.2f°C\r\n", 
                temp_state, resistance1, temp_ntc1, resistance2, temp_ntc2);
            HAL_UART_Transmit(&huart1, (uint8_t*)msg_temp, strlen(msg_temp), 250);
        }
}