#ifndef _BTCPARSER_BASE58_H_
#define _BTCPARSER_BASE58_H_

#include <cstdint>

// big_endian indicates whether the input is big-endian or not.
// char * dest is ended with '\0'
uint32_t EncodeBase58(const uint8_t *src, uint32_t length, bool big_endian,
                  char *dest, uint32_t buf_len);

// big_endian indicates whether the output is big-endian or not.
// char * src should be ended with '\0'
uint32_t DecodeBase58(const char *src, bool big_endian, uint8_t *dest,
                      uint32_t buf_len);

#endif
