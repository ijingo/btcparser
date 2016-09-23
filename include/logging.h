/**
 * adjusted from https://github.com/jratcliff63367/blockchain21
 */
#ifndef _BTCPARSER_LOGGING_H_
#define _BTCPARSER_LOGGING_H_

#include <cstdint>

// Utility and helper functions to print and log output
const char *formatNumber(int32_t number);
const char *getDateString(uint32_t t);
const char *getTimeString(uint32_t timeStamp);
void printReverseHash(const uint8_t hash[32]);
void logMessage(const char *fmt, ...);
void logBitcoinAddress(const uint8_t address[25]);
const char *getBitcoinAddressAscii(const uint8_t address[25]);
uint32_t getKey(void);

#endif
