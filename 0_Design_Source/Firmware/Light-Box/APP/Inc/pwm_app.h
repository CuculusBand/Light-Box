/**
  * @file    pwm_app.c
  * @brief   function prototypes for the pwm_app.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWM_APP_H
#define __PWM_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* PWM_App initialization */
void PWM_App_Init(void);
/* PWM_App Stop */
void PWM_App_Stop(void);
/* Update PWM duty cycle based on encoder input */
void PWM_App_Update(float brightness, float cct_level); // brightness: 0.0~1.0, cct_level: 0.0~1.0

#ifdef __cplusplus
}
#endif

#endif /* __PWM_APP_H */