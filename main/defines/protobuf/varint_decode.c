#include <stdint.h>
#include "varint_decode.h"

static const char MSB = 0x80;
static const char MSBALL = ~0x7F;

static const uint32_t N1 = 128;
static const uint32_t N2 = 16384;
static const uint32_t N3 = 2097152;
static const uint32_t N4 = 268435456;

//4294967295


uint32_t smp_varint_encoding_length(uint32_t n) {
  return (
      n < N1 ? 1
    : n < N2 ? 2
    : n < N3 ? 3
    : n < N4 ? 4
    : 5
  );
}

uint8_t * smp_varint_encode(uint8_t * ptr, uint32_t n) {

    while (n & MSBALL) {
        *ptr++ = (n & 0xFF) | MSB;
        n = n >> 7;
    }
    *ptr++ = n;
    return ptr;
}

uint8_t * smp_varint_decode(uint8_t * ptr, uint32_t * n) {

  uint32_t result = 0;
  int bits = 0;
  while (*ptr & MSB) {
    result += ( (uint32_t) *ptr++ & 0x7F) << bits;
    bits += 7;
  }
  result += ( (uint32_t) *ptr++ & 0x7F) << bits;
  *n = result;
  return ptr;
}
