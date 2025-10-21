// C
#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

#include <inttypes.h>
#include "defines.h"
#include <util/delay.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CFG_MODBUS_ID = 0,
    CFG_UART_BAUDRATE = 1,
    CFG_SIZE = 2 // number of configuration registers
} ConfigurationRegisterIndex;

typedef struct {
    // uint8_t  modbus_id;
    uint32_t uart_baudrate;
    uint16_t registers[(ConfigurationRegisterIndex)CFG_SIZE];
    uint8_t  sizeOfConfigurationRegisters; 
} Configuration;

// Loaders
uint32_t load_baudrate_from_eeprom(void);
uint8_t  load_modbus_id_from_eeprom(void);
void     load_modbus_server_name_from_eeprom(uint8_t* nameBuffer, uint8_t bufferSize);


// Savers 
void store_baudrate_to_eeprom(uint32_t baudrate);
void store_modbus_id_to_eeprom(uint8_t modbus_id);
void store_modbus_server_name_to_eeprom(const uint8_t* name);

void eeprom_erase(void);
void handle_reset_enable_pin(void);

#ifdef __cplusplus
}
#endif

#endif // EEPROM_CONFIG_H
