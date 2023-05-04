#ifndef _SMP_VARINT_H_
#define _SMP_VARINT_H_

#ifdef __cplusplus
extern "C" {
#endif

uint8_t * smp_varint_encode(uint8_t * buffer, uint32_t value);
uint8_t * smp_varint_decode(uint8_t * buffer, uint32_t * value);
uint32_t smp_varint_encoding_length(uint32_t value);

#ifdef __cplusplus
}
#endif

#endif
