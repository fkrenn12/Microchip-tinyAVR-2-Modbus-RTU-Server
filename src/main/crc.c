#include "crc.h"

uint16_t modbus_crc16(uint8_t *buf, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (uint8_t bit = 0; bit < 8; bit++)
            crc = (crc & 0x0001) ? (crc >> 1) ^ 0xA001 : crc >> 1;
    }
    return crc;
}