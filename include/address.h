#ifndef _BTCPARSER_ADDRESS_H_
#define _BTCPARSER_ADDRESS_H_

// The public portion of the bitcoin ECDSA key is always 65 bytes long; the
// first byte is a hard coded 0x4 followed by the two 32 bytes of
// the elliptic curve (32 bytes X) (32 bytes Y)
//
// Following is how the public key is converted into the 25 byte Bitcoin address
// in base58 encoding.
//
// Step #0 : Gnenerate a 65 bytes public key, indicated as S0.
// Step #1 : Perform SHA-256 hash of the 65 bytes S0, we get 32 bytes of S1.
// Step #2 : Perform RIPEMD-160 hash on the 32 byte S1 producing a 20 byte S2.
// Step #3 : Add a 'version' bytoe of zero to the front of S2, then we get 21
// byte S3.
// Step #4 : Perform SHA-256 hash of previous step (21 byte S3 -> 32 byte S4).
// Step #5 : Perform SHA-256 hash of previous step again (32 byte S4 -> 32 byte
// S5).
// Step #6 : Add the leading 4 byte of S5 as the checksum to the end of 21 byte
// S3 gotten from Step #3. Now we have a 25 byte S6.
// Step #7 : Convert the result S6 to ASCII using Base58-Check encoding. Note
// that, the S6 now is in big-endian.
// Referring to https://en.bitcoin.it/wiki/Base58Check_encoding
//
#include <cstdint>
// Concerts an ASCII Bitcoin address (Base58 encoded) into the 25 byte version.
bool ASCIIToAddress(const char* src, uint8_t dest[25]);
// Converts a 25 byte Bitcoin adress into the ASCII version (Base58).
// char * dest is '\0' ended.
bool AddressToASCII(const uint8_t src[25], char* dest, uint32_t max_len);
// Converts a full 65 byte ECDSA public key into ASCII version.
// char * dest is '\0' ended.
bool PublicKeyToASCII(const uint8_t src[65], char* dest, uint32_t max_len);
// Converts a 33 byte compressed ECDSA public key into ASCII version.
bool CompressedPublicKeyToASCII(const uint8_t src[33], char* dest,
                                uint32_t max_len);
// Convert a full 65 byte ECDSA public key into the 25 byte version (with a 1
// byte version header and 4 byte checksum at the end).
bool PublicKeyToAddress(const uint8_t src[65], uint8_t dest[25]);
// Converts a 33 byte compressed ECDSA public key into the 25 byte version (with
// a 1 byte version header and 4 byte checksum at the end).
bool CompressedPublicKeyToAddress(const uint8_t src[33], uint8_t dest[25]);

// Add 1 byte header and 4 byte checksum to 20 byte RIPEMD160 address and get 25
// byte address.
void RIPEMD160ToAddress(const uint8_t src[20], uint8_t dest[25]);
void RIPEMD160ToScriptAddress(const uint8_t src[20], uint8_t dest[25]);

#endif
