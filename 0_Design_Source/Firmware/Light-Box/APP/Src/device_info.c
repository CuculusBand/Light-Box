/**
  * @file    device_info.c
  * @brief   Implementation of the generation, writing or reading device information
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32g0xx.h"
#include "main.h"

#include "device_info.h"

uint32_t uid[3];        // 96 bit STM32 UID
char UID_Base32[21];    // 20 chars for Base32 + 1 for null terminator
char UID_Base64URL[17]; // 16 chars for Base64URL + 1 for null terminator

// Encode alphabet table
static const char base32_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static const char base64url_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// Get Chip ID of current chip
uint32_t Get_UID_address(const char* chip_series_name)
{
    // Check if the current chip series name is defined
    if(chip_series_name == NULL) return 0;
    // Search UID address in database
    for(int i = 0; i < sizeof(chip_database)/sizeof(chip_database[0]); i++)
    {
        // Compare two strings
        if(strcmp(chip_database[i].chip_series_name, chip_series_name) == 0)
        {
            return chip_database[i].chip_uid_base_addr;
        }
    }
    // Return 0x00000000 if no data matches the definition
    return 0x00000000;
}

// Get UID address of current chip
void Get_Chip_UID(uint32_t addr)
{
    // Check if address is empty
    if(addr == 0) {
        uid[0] = uid[1] = uid[2] = 0;
        return;
    }
    // Read UID
    uid[0] = *(volatile uint32_t*)(addr + 0);
    uid[1] = *(volatile uint32_t*)(addr + 4);
    uid[2] = *(volatile uint32_t*)(addr + 8);
}

// Convert 96-bit STM32 UID into a Base32 string
void UID_To_Base32(const uint32_t uid[3], ByteOrder endian)
{
    uint8_t bytes[12];
    int i;
    // Convert uid[3] to 12-byte array
    for (i = 0; i < 3; i++) {
        if (endian == UID_ENDIAN_LITTLE) {
            // Little-endian: low byte first
            bytes[i*4 + 0] = (uid[i] >> 0) & 0xFF;
            bytes[i*4 + 1] = (uid[i] >> 8) & 0xFF;
            bytes[i*4 + 2] = (uid[i] >> 16) & 0xFF;
            bytes[i*4 + 3] = (uid[i] >> 24) & 0xFF;
        } else {
            // Big-endian: high byte first
            bytes[i*4 + 0] = (uid[i] >> 24) & 0xFF;
            bytes[i*4 + 1] = (uid[i] >> 16) & 0xFF;
            bytes[i*4 + 2] = (uid[i] >> 8) & 0xFF;
            bytes[i*4 + 3] = (uid[i] >> 0) & 0xFF;
        }
    }
    // Current bit
    int bit_index = 0;
    // Encode 12 bytes to 20 Base32 chars (1 char = 5 bits)
    for (i = 0; i < 20; i++) {
        // Byte of current bit
        int byte_index = bit_index / 8;
        // Start bit position in the current byte
        int offset = bit_index % 8;
        // Splice two bytes
        // Take the upper 8 bits
        uint16_t chunk = bytes[byte_index] << 8;
        // Check if there is only 1 byte left
        if (byte_index + 1 < 12)
            chunk |= bytes[byte_index + 1];
        // Take the lowest 5 bits
        // For the last bit, the upper 4 bits are automatically filled with 0
        chunk = (chunk >> (16 - 5 - offset)) & 0x1F;
        // Search for char in the table
        UID_Base32[i] = base32_table[chunk];
        // Move to next 5 bits
        bit_index += 5;
    }
    UID_Base32[20] = '\0'; // Null terminate
}

// Convert 96-bit STM32 UID into a Base36 string
void UID_To_Base64URL(const uint32_t uid[3], ByteOrder endian)
{
    uint8_t bytes[12];
    int i, pos = 0;
    // Convert uid[3] → 12-byte array
    for (i = 0; i < 3; i++) {
        if (endian == UID_ENDIAN_LITTLE) {
        // Little-endian: low byte first
        bytes[i*4 + 0] = (uid[i] >> 0) & 0xFF;
        bytes[i*4 + 1] = (uid[i] >> 8) & 0xFF;
        bytes[i*4 + 2] = (uid[i] >> 16) & 0xFF;
        bytes[i*4 + 3] = (uid[i] >> 24) & 0xFF;
        } else {
            // Big-endian: high byte first
            bytes[i*4 + 0] = (uid[i] >> 24) & 0xFF;
            bytes[i*4 + 1] = (uid[i] >> 16) & 0xFF;
            bytes[i*4 + 2] = (uid[i] >> 8) & 0xFF;
            bytes[i*4 + 3] = (uid[i] >> 0) & 0xFF;
        }
    } 
    // Encode every 3 bytes → 4 Base64 chars (1 char = 6 bits)
    for (i = 0; i < 12; i += 3) {
        uint32_t triple = (bytes[i] << 16) | (bytes[i+1] << 8) | bytes[i+2];
        UID_Base64URL[pos++] = base64url_table[(triple >> 18) & 0x3F];
        UID_Base64URL[pos++] = base64url_table[(triple >> 12) & 0x3F];
        UID_Base64URL[pos++] = base64url_table[(triple >> 6) & 0x3F];
        UID_Base64URL[pos++] = base64url_table[triple & 0x3F];
    }
    UID_Base64URL[pos] = '\0'; // Null terminate
}