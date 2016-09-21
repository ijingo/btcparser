#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "base58.h"

#define MAX_BIG_NUMBER_BYTES 256

static uint8_t buf[MAX_BIG_NUMBER_BYTES];

// an incomplete implementation for Big Number

class BigNum {
 public:
  BigNum(const uint8_t* src, uint32_t len) : length(len) {
    assert(len < MAX_BIG_NUMBER_BYTES);
    memset(data, 0, MAX_BIG_NUMBER_BYTES);
    memcpy(data, src, len);
  }

  explicit BigNum(uint32_t len) : length(len) {
    memset(data, 0, MAX_BIG_NUMBER_BYTES);
  }

  BigNum(const BigNum& b) {
    memcpy(data, b.data, b.length);
    length = b.length;
  }

  enum BigNumCmp { LARGER = 1, EQUAL = 0, LESSER = -1 };

  // operation with BigNum
  BigNum& operator=(const BigNum& b);
  BigNum& operator+=(const BigNum& b);
  BigNum& operator-=(const BigNum& b);

  // operation with uint8_t
  BigNum& operator*=(const uint8_t b);
  BigNum& operator-=(const uint8_t b);

  // operation with 58
  void Div58();
  uint8_t Mod58();

  // compare to BigNum
  bool operator==(const BigNum& b);
  bool operator>(const BigNum& b);
  bool operator>=(const BigNum& b);
  bool operator<(const BigNum& b);
  bool operator<=(const BigNum& b);

  // compare to 58
  BigNumCmp CompareTo58();

  void normalize();
  void PowUInt8(uint8_t base, uint8_t exp);
  // padding with num zeros
  void PaddingZero(uint32_t num) {
    length += num;
    assert(length < MAX_BIG_NUMBER_BYTES);
    memset(data + length - num, 0, num);
  }
  uint32_t PaddingZeroNum() {
    uint32_t ret = 0;
    for (int32_t x = length - 1; x >= 0; --x) {
      if (data[x] == 0)
        ++ret;
      else
        break;
    }
    return ret;
  }
  uint8_t MostSignificant() { return data[length - 1]; }

  uint32_t get_length() const { return length; }

  friend uint32_t EncodeBase58(const uint8_t* src, uint32_t length,
                               bool big_endian, char* dest, uint32_t buf_len);

  friend uint32_t DecodeBase58(const char* src, bool big_endian, uint8_t* dest,
                               uint32_t buf_len);

 private:
  uint8_t data[MAX_BIG_NUMBER_BYTES];
  uint32_t length;
};

BigNum::BigNumCmp BigNum::CompareTo58() {
  if (length > 1) return LARGER;
  if (data[0] > 58) return LARGER;
  if (data[0] < 58) return LESSER;
  return EQUAL;
}

bool BigNum::operator==(const BigNum& b) {
  if (length != b.length) return false;
  for (uint32_t i = 0; i < length; ++i) {
    if (data[i] != b.data[i]) return false;
  }
  return true;
}

bool BigNum::operator>(const BigNum& b) {
  if (length > b.length) return true;
  if (length == b.length) {
    for (uint32_t i = 0; i < length; ++i) {
      if (data[i] == b.data[i]) continue;
      if (data[i] > b.data[i])
        return true;
      else
        return false;
    }
  }
  return false;
}

bool BigNum::operator>=(const BigNum& b) {
  return (this->operator==(b) || this->operator>(b));
}

bool BigNum::operator<(const BigNum& b) {
  return (!(this->operator==(b)) && !(this->operator>(b)));
}

bool BigNum::operator<=(const BigNum& b) { return !(this->operator>(b)); }

void BigNum::normalize() {
  while (length > 1 && !data[length - 1]) --length;
}

BigNum& BigNum::operator+=(const BigNum& b) {
  if (length < b.length) {
    memset(data + length, 0, b.length - length);
    length = b.length;
  }
  bool overflow = 0;
  uint8_t x = 0;
  for (; x < length; ++x) {
    data[x] += b.data[x] + overflow;
    overflow = (data[x] < (b.data[x] + overflow)) ? 1 : 0;
  }
  while (overflow && x < length) overflow = !(++data[x++]);
  if (overflow) {
    ++length;
    assert(length < MAX_BIG_NUMBER_BYTES);
    data[length - 1] = 1;
  }
  return *this;
}

BigNum& BigNum::operator-=(const BigNum& b) {
  uint32_t x;
  bool underflow = 0;
  for (x = 0; x < length; ++x) {
    uint16_t tmp = underflow + b.data[x];
    underflow = data[x] < tmp;
    data[x] -= static_cast<uint8_t>(tmp);
  }
  if (underflow) --data[x];
  normalize();
  return *this;
}

BigNum& BigNum::operator*=(const uint8_t b) {
  if (b == 0) {
    length = 1;
    data[0] = 0;
    return *this;
  }
  if (length == 1 && data[0] == 0) return *this;
  uint16_t tmp = 0;
  uint32_t x = 0;
  for (; x < length; ++x) {
    tmp += data[x] * b;
    data[x] = static_cast<uint8_t>(tmp);
    tmp >>= 8;
  }
  if (tmp) {
    ++length;
    assert(length < MAX_BIG_NUMBER_BYTES);
    data[x] = static_cast<uint8_t>(tmp);
  }
  return *this;
}

BigNum& BigNum::operator-=(const uint8_t b) {
  uint8_t carry = b;
  uint32_t x = 0;
  for (; data[x] < carry; ++x) {
    data[x] = 0xff - carry + data[x] + 0x01;
    carry = 1;
  }
  data[x] -= carry;
  normalize();
  return *this;
}

void BigNum::PowUInt8(uint8_t base, uint8_t exp) {
  length = 1;
  data[0] = 1;
  for (uint32_t x = 0; x < exp; ++x) *this *= base;
}

void BigNum::Div58() {
  if (length == 1 && data[0] == 0) return;
  uint16_t tmp = 0;
  for (int32_t x = length - 1; x >= 0; --x) {
    tmp <<= 8;
    tmp |= data[x];
    buf[x] = static_cast<uint8_t>(tmp / 58);
    tmp -= buf[x] * 58;
  }
  if (buf[length - 1] == 0) --length;
  memmove(data, buf, length);
}

// method presented here: http://stackoverflow.com/a/10441333/238411
uint8_t BigNum::Mod58() {
  uint16_t result = 0;
  for (int32_t x = length - 1; x >= 0; --x) {
    result *= (0x100 % 58);
    result %= 58;
    result += data[x] % 58;
    result %= 58;
  }
  return static_cast<uint8_t>(result);
}
//   0         1         2         3         4         5
//   01234567890123456789012345678901234567890123456789012345678
static const char Base58Alphabet[] =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

void DecodeBase58(BigNum* big_num, const char* src_str) {
  BigNum num(1);
  uint32_t slen = static_cast<uint32_t>(strlen(src_str));
  for (int32_t x = slen - 1; x >= 0; --x) {
    uint8_t idx = src_str[x];
    if (idx != 49) {          // If not 1
      if (src_str[x] < 58) {  // Numbers
        idx -= 49;
      } else if (src_str[x] < 73) {  // A-H
        idx -= 56;
      } else if (src_str[x] < 79) {  // J-N
        idx -= 57;
      } else if (src_str[x] < 91) {  // P-Z
        idx -= 58;
      } else if (src_str[x] < 108) {  // a-k
        idx -= 64;
      } else {  // m-z
        idx -= 65;
      }
      num.PowUInt8(58, static_cast<uint8_t>(slen - 1 - x));
      num *= idx;
      *big_num += num;
    }
  }
  // Got BigNumber from base-58 string. Add zeros on end.
  uint8_t zeros = 0;
  for (uint32_t x = 0; x < slen; x++) {
    if (src_str[x] == '1')
      zeros++;
    else
      break;
  }
  if (zeros) big_num->PaddingZero(zeros);
}

bool EncodeBase58(BigNum* big_num, char* dest_str, uint32_t max_len) {
  uint32_t x = 0;
  for (; x < big_num->PaddingZeroNum(); ++x) dest_str[x] = '1';
  uint32_t zeros = x;
  uint8_t mod;
  for (; big_num->CompareTo58() >= 0; ++x) {
    mod = big_num->Mod58();
    if (big_num->get_length() < x + 3)
      if (max_len < x + 3) return false;
    dest_str[x] = Base58Alphabet[mod];
    big_num->Div58();
  }
  dest_str[x++] = Base58Alphabet[big_num->MostSignificant()];

  for (uint8_t y = 0; y < (x - zeros) / 2; y++) {
    char tmp = dest_str[y + zeros];
    dest_str[y + zeros] = dest_str[x - y - 1];
    dest_str[x - y - 1] = tmp;
  }
  dest_str[x] = '\0';
  return true;
}

uint32_t DecodeBase58(const char* src, bool big_endian, uint8_t* dest,
                      uint32_t buf_len) {
  uint32_t ret = 0;
  BigNum bn(0);
  DecodeBase58(&bn, src);
  ret = bn.get_length();
  if (big_endian)
    for (uint32_t i = 0; i < ret; ++i) dest[i] = bn.data[ret - 1 - i];
  else
    memcpy(dest, bn.data, ret);
  return ret;
}

uint32_t EncodeBase58(const uint8_t* src, uint32_t length, bool big_endian,
                      char* dest, uint32_t buf_len) {
  uint8_t hash[length];
  if (big_endian)
    for (uint32_t i = 0; i < length; ++i) hash[i] = src[length - i - 1];
  else
    memcpy(hash, src, length);
  BigNum bn(hash, length);
  EncodeBase58(&bn, dest, buf_len);
  return bn.get_length();
}

/*
int main() {
  uint8_t src[] = {0X00, 0X7E, 0X1B, 0X61, 0X15, 0X95, 0X8A, 0X63, 0X06,
                   0X3F, 0XC9, 0XF1, 0X29, 0X51, 0X85, 0X8D, 0XC8, 0X86,
                   0X4F, 0XDC, 0X01, 0XA8, 0X93, 0XC1, 0XD5};
  char dest[100];
  EncodeBase58(src, 25, true, dest, 100);
  printf("%s\n", dest);
  DecodeBase58(dest, true, src, 25);
  for (int i = 0; i < 25; ++i)
    printf("%02X ", src[i]);
  printf("\n");

  return 0;
}
*/
