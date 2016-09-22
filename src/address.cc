#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "address.h"
#include "base58.h"
#include "CRC32.h"
#include "SHA256.h"
#include "RIPEMD160.h"

bool BitcoinPublicKeyToAddress(const uint8_t src[65], uint8_t dest[25]) {
  // src[0] equals to 0x04 means an uncompressed ECDSA publick key, see
  // https://bitcoin.org/en/developer-guide#public-key-formats.
  if (src[0] == 0x04) {
    uint8_t hash[32];

    computeSHA256(src, 65, hash);
    dest[0] = 0;
    computeRIMPED160(hash, 32, dest + 1);
    computeSHA256(dest, 21, hash);
    computeSHA256(hash, 32, hash);
    dest[21] = hash[0];
    dest[22] = hash[1];
    dest[23] = hash[2];
    dest[24] = hash[3];

    return true;
  }
  return false;
}

bool BitcoinCompressedPublicKeyToAddress(const uint8_t src[33], uint8_t dest[25]) {
  // src[0] equals to 0x02 / 0x03 means a compressed ECDSA
  // publick key, see
  // https://bitcoin.org/en/developer-guide#public-key-formats.
  if (src[0] == 0x02 || src[0] == 0x03) {
    uint8_t hash[32];

    computeSHA256(src, 33, hash);
    dest[0] = 0;
    computeRIMPED160(hash, 32, dest + 1);
    computeSHA256(dest, 21, hash);
    computeSHA256(hash, 32, hash);
    dest[21] = hash[0];
    dest[22] = hash[1];
    dest[23] = hash[2];
    dest[24] = hash[3];

    return true;
  }
  return false;
}

bool BitcoinPublicKeyToASCII(const uint8_t src[65], char* dest, uint32_t max_len) {
  dest[0] = '\0';
  uint8_t hash[25];
  if (BitcoinPublicKeyToAddress(src, hash)) {
    EncodeBase58(hash, 25, true, dest, max_len);
    return true;
  }
  return false;
}

bool BitcoinCompressedPublicKeyToASCII(const uint8_t src[33], char* dest,
                                uint32_t max_len) {
  dest[0] = '\0';
  uint8_t hash[25];
  if (BitcoinCompressedPublicKeyToAddress(src, hash)) {
    EncodeBase58(hash, 25, true, dest, max_len);
    return true;
  }
  return false;
}

bool BitcoinASCIIToAddress(const char* src, uint8_t dest[25]) {
  uint32_t len = DecodeBase58(src, true, dest, 25);
  if (len == 25) {
    uint8_t checksum[32];
    computeSHA256(dest, 21, checksum);
    computeSHA256(checksum, 32, checksum);
    if (dest[21] == checksum[0] && dest[22] == checksum[1] &&
        dest[23] == checksum[2] && dest[24] == checksum[3])
      return true;
  }
  return false;
}

void BitcoinRIPEMD160ToAddress(const uint8_t src[20], uint8_t dest[25]) {
  uint8_t hash[32];
  dest[0] = '\0';
  memcpy(dest + 1, src, 20);
  computeSHA256(dest, 21, hash);
  computeSHA256(hash, 32, hash);
  dest[21] = hash[0];
  dest[22] = hash[1];
  dest[23] = hash[2];
  dest[24] = hash[3];
}
void BitcoinRIPEMD160ToScriptAddress(const uint8_t src[20], uint8_t dest[25]) {
  BitcoinRIPEMD160ToAddress(src, dest);
}

bool BitcoinAddressToASCII(const uint8_t src[25], char* dest, uint32_t max_len) {
  EncodeBase58(src, 25, true, dest, max_len);
  return true;
}
