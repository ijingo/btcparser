#ifndef _BTCPARSER_CRC32_H
#define _BTCPARSER_CRC32_H

/*************************************************************************************//**
*
* @brief CRC32 helper functions
*
*****************************************************************************************/

#include <cstdint>

uint32_t CRC32(uint8_t *buffer, uint32_t len, uint32_t seed);


#endif
