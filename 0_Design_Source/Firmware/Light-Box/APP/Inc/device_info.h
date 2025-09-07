/**
  * @file    device_info.h
  * @brief   function prototypes for the device_info.c file
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_INFO_H
#define __DEVICE_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Private variables ---------------------------------------------------------*/
// UID array
extern uint32_t uid[3];        // 96 bit STM32 UID
extern char UID_Base32[21];    // 20 chars for Base32 + 1 for null terminator
extern char UID_Base64URL[17]; // 16 chars for Base64URL + 1 for null terminator

// Byte order selection
typedef enum {
    UID_ENDIAN_LITTLE, // low byte first
    UID_ENDIAN_BIG     // high byte first
} ByteOrder;

// ChipInfo structure
typedef struct {
    const char* chip_series_name;        // chip series
    uint32_t    chip_uid_base_addr; // UID address
} ChipInfo_t;

// Database of chips' UID address
static const ChipInfo_t chip_database[] = {
    // STM32F0 series
    {"STM32F0XX", 0x1FFFF7AC},
    // STM32F1 series
    {"STM32F1XX", 0x1FFFF7E8},
    // STM32F2 series
    {"STM32F2xx", 0x1FFF7A10},   
    // STM32F3 series
    {"STM32F3xx", 0x1FFFF7AC},
    // STM32F4 series
    {"STM32F4xx", 0x1FFF7A10},
    // STM32F7 series
    {"STM32F7xx", 0x1FF0F420},
    // STM32H7 series
    {"STM32H7xx", 0x1FF1E800},
    // STM32G0 series
    {"STM32G0xx", 0x1FFF7590},
    // STM32G4 series
    {"STM32G4xx", 0x1FFF7590},    
    // STM32L0 series
    {"STM32L0xx", 0x1FF80050},
    // STM32L1 series
    {"STM32L1xx", 0x1FF80050},
    // STM32L4 series
    {"STM32L4xx", 0x1FFF7590},
    // STM32L5 series
    {"STM32L5xx", 0x0BFA0590},  
    // STM32WB series
    {"STM32WBxx", 0x1FFF7590},
    // Other chip series...
    // Unknown chip
    {"Unknown", 0x00000000}
};

/* Private function prototypes -----------------------------------------------*/
// Get UID address of current chip
uint32_t Get_UID_address(const char* chip_series_name);
// Get UID of the chip from specific address
void Get_Chip_UID(uint32_t addr);
// Convert 96-bit STM32 UID into a Base32 string
void UID_To_Base32(const uint32_t uid[3], ByteOrder endian);
// Convert 96-bit STM32 UID into a Base64URL string
void UID_To_Base64URL(const uint32_t uid[3], ByteOrder endian);

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_INFO_H */
