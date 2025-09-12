/**
  * @file    data_format.c
  * @brief   Implementation of data format conversion or restructuring
  */

/* Includes ------------------------------------------------------------------*/
#include "data_format.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Convert float to ascii string with specified decimal places
/**
    * @brief    Convert a floating-point number to an ASCII string with specified decimal places
    * @param    buf: Pointer to the output buffer
    * @param    num: The floating-point number to convert
    * @param    decimals: Number of decimal places to include
    * @note     Used for displaying float values when sprintf does not support %f format
    */ 
void float2ascii(char *buf, float num, int decimals)
{
    // Deal with negative numbers
    if (num < 0) {
        *buf++ = '-';   // add negative sign ahead of the string
        num = -num;     // make number positive
    }
    // Limit decimals to a maximum of 9 and a minimum of 0
    if (decimals > 9) {
        decimals = 9;
    } else if (decimals < 0) {
        decimals = 0;
    }
    // Rounding to the specified decimal places
    float scale = powf(10.0f, decimals); 
    int scaled = (int)(num * scale + 0.5f);
    // Integer and fractional parts
    int int_part = scaled / (int)scale;
    int frac_part = scaled % (int)scale;
    // Print integer part
    sprintf(buf, "%d", int_part);               // use sprintf to convert integer part to string
    while (*buf != '\0') buf++;                 // move to the end of the string to append fractional part
                                                // '\0' is the string terminator
    // Print fractional part
    if (decimals > 0) {                         // if decimals > 0, print fractional part
        // Handle fractional part
        if (decimals > 0) {
            // Add decimal point first
            *buf++ = '.';
            // Handle leading zeros in fractional part
            sprintf(buf, "%0*d", decimals, frac_part);
        }
    }
}