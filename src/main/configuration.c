// C
#include "configuration.h"
#include <avr/eeprom.h>     // AVR EEPROM API
#include "config.h"        // for UART_BAUDRATE default

// Project-specific offsets (keep in a single place)
#ifndef EEPROM_OFFSET_MODBUS_ID
#define EEPROM_OFFSET_MODBUS_ID ((uint8_t)0x00)
#endif
#ifndef EEPROM_OFFSET_BAUDRATE
#define EEPROM_OFFSET_BAUDRATE  ((uint8_t)0x01)
#endif
//#ifndef EEPROM_OFFSET_MODBUS_SERVER_NAME
//#define EEPROM_OFFSET_MODBUS_SERVER_NAME ((uint8_t)0x03)
//#endif

#ifndef UART_BAUDRATE
#define UART_BAUDRATE 9600u
#endif

Configuration configuration = {
    .uart_baudrate = UART_BAUDRATE,
    .registers = {0u, (uint16_t)(UART_BAUDRATE/100)},
    .sizeOfConfigurationRegisters = (ConfigurationRegisterIndex)CFG_SIZE
};

// Helpers: big-endian 16-bit read/write
static inline uint16_t eeprom_read_u16_bigendian(uint8_t offset) {
    const uint8_t hi = eeprom_read_byte((uint8_t*)(offset + 0));
    const uint8_t lo = eeprom_read_byte((uint8_t*)(offset + 1));
    return (uint16_t)((uint16_t)hi << 8) | (uint16_t)lo;
}
static inline void eeprom_write_u16_bigendian(uint8_t offset, uint16_t value) {
    eeprom_write_byte((uint8_t*)(offset + 0),(uint8_t)(value >> 8));
    eeprom_write_byte((uint8_t*)(offset + 1),(uint8_t)(value & 0xFFu));
}

// Public loaders
uint32_t load_baudrate_from_eeprom(void) {
    const uint16_t raw = eeprom_read_u16_bigendian(EEPROM_OFFSET_BAUDRATE);
    if (raw == 0xFFFFu) return (uint32_t)UART_BAUDRATE;  // not programmed
    uint32_t baud = (uint32_t)raw * 100u; // stored as baud/100
    baud = (baud == 0u) ?  (uint32_t)UART_BAUDRATE : baud; // defensive fallback
    return baud;
}

uint8_t load_modbus_id_from_eeprom(void) {
    uint8_t id = eeprom_read_byte((uint8_t*)EEPROM_OFFSET_MODBUS_ID);
    id = (id == 0xff && MODBUS_ID == 0) ? 254 : id; // 255 not allowed -> set to 254
    id = (id == 0xff && MODBUS_ID != 0) ? MODBUS_ID : id; // set to config.h default if unprogrammed
    return id;
}

/*
void load_modbus_server_name_from_eeprom(uint8_t* nameBuffer, uint8_t bufferSize) {
    if (bufferSize == 0) return; // nothing to do
    for (uint8_t i = 0; i < bufferSize - 1; i++) {
        uint8_t c = eeprom_read_byte((uint8_t*)(EEPROM_OFFSET_MODBUS_SERVER_NAME + i));
        if (c == 0 || c == 0xff) {
            nameBuffer[i] = 0;
            return;
        }
        nameBuffer[i] = c;
    }
    nameBuffer[bufferSize - 1] = 0; // ensure null-termination
}
*/

/*
void store_modbus_server_name_to_eeprom(const uint8_t* name) {
    for (uint8_t i = 0; i < MAX_SERVERNAME_LENGTH; i++) { // limit to 32 bytes
        uint8_t c = name[i];
        eeprom_write_byte((uint8_t*)(EEPROM_OFFSET_MODBUS_SERVER_NAME + i), c);
        if (c == 0 || c == 0xff) break; // null-terminated
    }
}
*/

void store_baudrate_to_eeprom(uint32_t baudrate) {
    // Encode as baud/100, clamp to 16-bit
    uint32_t baud_div100 = baudrate / 100u;
    if (baud_div100 > 0xFFFFu) baud_div100 = 0xFFFFu;
    eeprom_write_u16_bigendian(EEPROM_OFFSET_BAUDRATE, (uint16_t)baud_div100);
}

void store_modbus_id_to_eeprom(uint8_t modbus_id) {
    eeprom_write_byte((uint8_t*)(uintptr_t)EEPROM_OFFSET_MODBUS_ID, modbus_id);
}

void eeprom_erase(void) {
    for (uint8_t i = 0; i < 3; i++) {
            eeprom_write_byte((uint8_t*)(uintptr_t)i, 0xff);
    }
}
