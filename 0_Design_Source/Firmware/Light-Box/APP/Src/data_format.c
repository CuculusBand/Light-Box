/**
  * @file    data_format.c
  * @brief   Implementation of data format conversion or restructuring
  */

/* Includes ------------------------------------------------------------------*/
#include "data_format.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    // deal with negative numbers
    if (num < 0) {
        *buf++ = '-';   // add negative sign ahead of the string
        num = -num;     // make number positive
    }
    // integer and fractional parts
    int int_part = (int)num;                    // separate interger part from float
    float frac_part = num - (float)int_part;    // get fractional part by subtraction
    // print integer part
    sprintf(buf, "%d", int_part);               // use sprintf to convert integer part to string
    while (*buf != '\0') buf++;                 // move to the end of the string to append fractional part
                                                // '\0' is the string terminator
    // print fractional part
    if (decimals > 0) {                         // if decimals > 0, print fractional part
        *buf++ = '.';                           // add decimal point first
        // handle fractional part
        for (int i = 0; i < decimals; i++) {    // loop for number of decimal places
            frac_part *= 10.0f;                 // shift left by one decimal place
            int digit = (int)frac_part;         // get the integer part which is the next digit
            *buf++ = '0' + digit;               // convert digit to character and store in buffer ('0'=ASCII 48, this step calculates the ASCII of the digit)
                                                // write the digit into buf[x] and move to buf[x+1]
            frac_part -= digit;                 // remove the integer part, keep the fractional part for next digit
        }
        *buf = '\0';                            // null-terminate in buf[decimals+1]
    }
}