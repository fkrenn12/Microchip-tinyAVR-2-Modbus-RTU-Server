#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint16_t modbus_crc16(uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif // CRC_H