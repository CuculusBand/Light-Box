/**
  * @file    ntc_temperature.c
  * @brief   Implementation of temperature measurement using NTC thermistor
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <math.h>
#include "stm32g0xx_hal_adc.h"
#include "stm32g0xx_hal_gpio.h"

#include "ntc_temperature.h"
extern uint16_t adc_buffer[3];

// Configuration for NTC temperature measurement
/**
    * @brief    Initialize the NTC measurement configuration
    * @note     This function sets up the NTC measurement parameters including ADC handle, channel, resolution, reference voltage, divider resistor, NTC resistor at 25°C, and B value.
    * @param    hadc            Pointer to the ADC handle
    * @param    hadc_channel    ADC channel number
    * @param    hadc_resolution ADC resolution (e.g., 4096 for 12-bit ADC)
    * @param    hadc_DMAindex   ADC DMA index
    * @param    vref            Reference voltage (V)
    * @param    r_ref           Divider resistor (Ω)
    * @param    r_ntc_25c       NTC resistor value at 25°C (Ω)
    * @param    b_value         B value of the NTC thermistor
    * @return   Initialized NTC_Measurement_Config_t structure
    */
NTC_Measurement_Config_t NTC_ChannelConfig(ADC_HandleTypeDef* hadc, uint32_t hadc_channel, float hadc_resolution, uint16_t hadc_DMAindex, float vref, float r_ref, float r_ntc_25c, float b_value)
{
    NTC_Measurement_Config_t config = Default_NTC_Measurement_Config;
    config.adc_handle = hadc;
    config.adc_channel = hadc_channel;
    config.adc_resolution = hadc_resolution;
    config.adc_DMAindex = hadc_DMAindex;
    config.vref = vref;
    config.r_ref = r_ref;
    config.r_ntc_25c = r_ntc_25c;
    config.b_value = b_value;
    return config;  // return the initialized configuration
}

// Get ADC value and calculate NTC resistance
/**
    * @brief    Get the resistance of the NTC thermistor
    * @note     This function reads the ADC value, calculates the voltage across the NTC thermistor.
    * @param    config  Pointer to the NTC_Measurement_Config_t structure
    * @return   Calculated NTC resistance in ohms, -1.0f if an error occurs
    */
float NTC_GetResistance(NTC_Measurement_Config_t* config)
{
    uint32_t adc_value;
    float resistance;
    // ADC_ChannelConfTypeDef sConfig = {0};
    
    if (config->adc_handle == NULL) {
        return -1.0f;  // invalid ADC handle
    }

    uint16_t DMAindex = config->adc_DMAindex;
    adc_value = adc_buffer[DMAindex]; 
    
    // Calculate the voltage across the NTC thermistor
    float Vadc = (float)adc_value * config->vref / config->adc_resolution; 

    // Check if the voltage is within a valid range
    if (Vadc < 0.001f || Vadc > (config->vref - 0.001f)) {
        return -1.0f;  // invalid voltage reading
    }
    // Calculate the NTC resistance using the voltage divider formula
    // R_ntc = (Vadc * R_ref) / (Vref - Vadc)
    resistance = (Vadc * config->r_ref) / (config->vref - Vadc);
    return resistance;
}

// Calculate temperature from NTC resistance
/**
 * @brief   Calculate the temperature from the NTC resistance
 * @note    This function uses the Steinhart-Hart equation to calculate the temperature in Celsius.
 * @param   config  Pointer to the NTC_Measurement_Config_t structure
 * @return  Calculated temperature in degrees Celsius, or -1.0f if an error occurs
 */
float NTC_GetTemperature(NTC_Measurement_Config_t* config)
{
    float resistance, temperature;
    // Get the NTC resistance
    resistance = NTC_GetResistance(config);
    
    if (resistance == -1.0f) {
        return -999.0;        // error in resistance measurement
    }
    if (resistance >= 320000.0f) {
        return -273.15f;    // invalid resistance(>=320k), return absolute zero
    }
    if (resistance < 480.0f) {
        return 110.0f;        // invalid resistance(<0.48k), return a high temperature
    }
    
    // Calculate temperature
    // T(K) = 1 / (1/T_ref + (1/B) * log(R/R_ntc_25c))
    float temperature_ref_kelvin = 25.0f + 273.15f;  // K(@25°C)
    temperature = 1.0f / (1.0f/temperature_ref_kelvin + (1.0f/config->b_value) * logf(resistance/config->r_ntc_25c));
    temperature -= 273.15f;  // convert Kelvin to Celsius

    return temperature;
}