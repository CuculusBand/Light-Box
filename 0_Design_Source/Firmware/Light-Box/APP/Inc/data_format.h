/**
  * @file    data_format.h
  * @brief   function prototypes for the data_format.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_FORMAT_H
#define __DATA_FORMAT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private function prototypes -----------------------------------------------*/
// Convert float to ascii string with specified decimal places
void float2ascii(char *buf, float num, int decimals);

#ifdef __cplusplus
}
#endif

#endif /* __DATA_FORMAT_H */
