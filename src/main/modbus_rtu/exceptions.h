#ifndef MODBUS_EXCEPTIONS_H
#define MODBUS_EXCEPTIONS_H

// Modbus Exception Codes
#define MB_EXCEPTION_ILLEGAL_FUNCTION        1  // Function code not supported
#define MB_EXCEPTION_ILLEGAL_DATA_ADDRESS    2  // Address not available
#define MB_EXCEPTION_ILLEGAL_DATA_VALUE      3  // Value not allowed
#define MB_EXCEPTION_SLAVE_DEVICE_FAILURE    4  // Device failure
#define MB_EXCEPTION_ACKNOWLEDGE             5  // Acknowledge
#define MB_EXCEPTION_SLAVE_DEVICE_BUSY       6  // Device busy
#define MB_EXCEPTION_MEMORY_PARITY_ERROR     8  // Memory parity error
#define MB_EXCEPTION_GATEWAY_PATH_UNAVAILABLE 10 // Gateway path unavailable
#define MB_EXCEPTION_GATEWAY_TARGET_FAILED   11 // Gateway target device failed to respond

#endif // MODBUS_EXCEPTIONS_H





