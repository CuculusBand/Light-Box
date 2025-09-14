/**
  * @file    ntc_temperature.h
  * @brief   function prototypes for the ntc_temperature.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NTC_TEMPERATURE_H
#define __NTC_TEMPERATURE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <math.h>
#include "stm32g0xx_hal_adc.h"
#include "stm32g0xx_hal_gpio.h"

/* Temperature measurement parameters */
typedef struct {
    ADC_HandleTypeDef* adc_handle;      // ADC handle
    uint32_t  adc_channel;              // ADC channel
    float     adc_resolution;           // ADC resolution
    uint16_t  adc_DMAindex;             // ADC DMA index
    float     vref;                     // Reference voltage (V)
    float     r_ref;                    // Divider resistor (Ω)
    float     r_ntc_25c;                // NTC resistor(@25°C) (Ω)
    float     b_value;                  // B
} NTC_Measurement_Config_t;

#define Default_NTC_Measurement_Config \
{ \
    .adc_handle = NULL, \
    .adc_channel = 0, \
    .adc_resolution = 4096.0f, \
    .adc_DMAindex = 0, \
    .vref = 3.3f, \
    .r_ref = 4700.0f, \
    .r_ntc_25c = 10000.0f, \
    .b_value = 3950.0f \
}

/* Config parameters for NTC temperature measurement */
NTC_Measurement_Config_t NTC_ChannelConfig(ADC_HandleTypeDef* hadc, uint32_t hadc_channel, float hadc_resolution, uint16_t adc_DMAindex, float vref, float r_ref, float r_ntc_25c, float b_value);
/* Get ADC value */
float NTC_GetResistance(NTC_Measurement_Config_t* config);
/* Calculate temperature from NTC resistance */
float NTC_GetTemperature(NTC_Measurement_Config_t* config);

#ifdef __cplusplus
}
#endif

#endif /* __NTC_TEMPERATURE_H */