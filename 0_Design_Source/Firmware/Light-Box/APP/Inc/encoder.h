/**
  * @file    encoder.h
  * @brief   function prototypes for the encoder.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODER_H
#define __ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"

#include "mixedlight_switch.h"

/* limits for brightness and color temperature */
#define BRIGHTNESS_MIN    0.0f  // 0.0 ~ 1.0 range for brightness
#define BRIGHTNESS_MAX    1.0f  // 0.0 ~ 1.0 range for brightness
#define CCT_LEVEL_MIN     0.0f  // 0.0 ~ 1.0 range for color temperature
//  #define CCT_LEVEL_MAX     1.0f  // 0.0 ~ 1.0 range for color temperature
#define CCT_LEVEL_MAX     0.8f  // 0.0 ~ 0.8 range for color temperature
/* Encoder step size */
#define ENCODER_BASE_STEP 0.005f  // base step size
#define ENCODER_MAX_STEP  0.1f    // max step size
/* Type definition for parameters used in PWM */
typedef void (*EncoderCallback)(float brightness, float cct_level);

/* Encoder initialization */
void Encoder_Init(TIM_HandleTypeDef *htim_encoder);
/* Register callback function for encoder updates */
void Encoder_RegisterCallback(EncoderCallback cb);
/* Update encoder state */
void Encoder_Update(TIM_HandleTypeDef *htim_encoder);
/* Get current brightness */
float Encoder_GetBrightness(void);
/* Get color temperature levels */
float Encoder_GetCCTLevel(void);
/* Get current encoder mode */
EncoderMode_t Encoder_GetMode(void);

#ifdef __cplusplus
}
#endif

#endif /* __ENCODER_H */