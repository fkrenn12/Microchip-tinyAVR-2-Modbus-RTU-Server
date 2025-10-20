## Overview
## ![modbus logo](documents/modbus-logo.png) RTU Server  
 
## ![microchip](documents/microchip-logo.png) tinyAVR® 2 Microcontroller Family  
## Build with Platformio ![platformio logo](documents/platformio-logo.png) or MPLAB-X  ![mplabx logo](documents/mplabx-logo.png) 
## Overview
Originally, the idea comes from [this repository](https://github.com/angeloc/simplemodbusng). However, if you compare the two projects, you will notice that there are not many similarities left. Nevertheless, I wanted to mention what inspired me.  
This is not a library for modbus, but it is a ready to use application with modbus support for tinyAVR's.  
[The tinyAVR® 2 microcontroller family](https://github.com/fkrenn12/Microchip-tinyAVR-2-Modbus-RTU-Server\/blob\/main\/documents\/AN3456-Getting-Started-with-tinyAVR-2-Family-DS00003456A.pdf) is cost-effective and available in several variants with different memory sizes (4 KB–32 KB) and pin counts (14–24). The generated machine code fits into the microcontroller with 8K program memory.
The code is written in C and designed to be compiled with VSCode + PlatformIO or MPLABX IDE (XC8-compiler)
## Features
### 🚀  Supported MODBUS Data model objects 🚀 
⭐ Coils (bit\-addressable, writable digital outputs)  
⭐ Discrete Inputs (bit\-addressable, read\-only digital inputs)  
⭐ Holding Registers (16\-bit, readable and writable registers)  
⭐ Input Registers (16\-bit, read\-only registers)  

### 🚀 Supported MODBUS function codes 🚀 
⭐ 01 \- Read Coils  
⭐ 02 \- Read Discrete Inputs  
⭐ 03 \- Read Holding Registers  
⭐ 04 \- Read Input Registers  
⭐ 05 \- Write Single Coil  
⭐ 06 \- Write Single Register  
⭐ 15 \- Write Multiple Coils  
⭐ 16 \- Write Multiple Registers  

Review the functions here: [MODBUS specification](https://github.com/fkrenn12/Microchip-tinyAVR-2-Modbus-RTU-Server/blob/main/documents/Modbus_Application_Guide.pdf).


### 🚀 Supported peripheral functionality 🚀
⭐ Digital Outputs (coils)  
⭐ Digital Inputs (discrete inputs)  
⭐ Dual Slope PWM (holding registers)   
⭐ ADC (input registers)  

## Cloning
```bash
git clone https://github.com/fkrenn12/Microchip-tinyAVR-2-Modbus-RTU-Server.git
```
```bash
gh repo clone fkrenn12/Microchip-tinyAVR-2-Modbus-RTU-Server
```

## Workflow
👉  Edit the file `/source/main/config.h` to adjust settings.  
👉  Rebuild the firmware after making changes.  
👉  Flash the newly built binary to the microcontroller’s program memory.  
👉  MODBUS_ID and UART baudrate can be readjusted anytime using a Modbus client or writing into the EEPROM.  

## Adjust settings
### Defining MODBUS-ID and UART baudrate
### `MODBUS-ID`
In the file `config.h`, you can specify the ID on the line `#define MODBUS_ID`. A valid ID is in the range 1 to 247. If the ID is not specified (i.e., the line reads #define MODBUS_ID 0), the ID **must be written** later using a Modbus client or directly into the EEPROM to ensure proper operation in a system.
###### Using Modbus Client
Modbus-ID 247 is reserved for configuring the server.  
To change an ID, a client must send a Write Single Holding Register message.

| Field             | Size    | Value/Range        | Notes                               |
|-------------------|---------|--------------------|-------------------------------------|
| Function code     | 1 byte  | 0x06               | Write Single Holding Register       |
| Register address  | 2 bytes | 0x9C40             | Register address of ModbusID        |
| Register value    | 2 bytes | 0x0001–0x00F7      | value 1 .. 247                      |
| CRC-16            | 2 bytes | Lo, Hi             | Appended by client; Lo byte first   |


###### Write directly into non volatile memory
The open source tool [`pymcuprog`](https://github.com/microchip-pic-avr-tools/pymcuprog) can be used to program the `MODBUS_ID` into the EEPROM.  
The following windows command line is an example to do this:
```bash
pymcuprog write -o 0 -b 1 -l 0xaa -m eeprom --clk 57000 -t uart -u com1 -d attiny3224
```
In the example above, the `MODBUS_ID` is set to 0xAA and COM1 is used with an ATtiny3224. This three values must be adapted to your own requirements. On Windows, the interface name (com) can be easily read from the Device Manager. On 🐧 Linux\-based systems, you can determine the interface name with the command `ls /dev/tty*`.

### `UART baudrate` 
In the file `config.h`, you can specify the UART baud rate. If #define UART_BAUDRATE 0 is set, the default value of `9600 bps` is used. The baud rate can also be written later using a Modbus client or directly into the EEPROM.

###### Using Modbus Client
Modbus-ID 247 is reserved for configuring the server.   
To change an ID, a client must send a Write Single Holding Register message.  

| Field             | Size    | Value/Range        | Notes                               |
|-------------------|---------|--------------------|-------------------------------------|
| Function code     | 1 byte  | 0x06               | Write Single Holding Register       |
| Register address  | 2 bytes | 0x9C41             | Register address of UART Baudrate   |
| Register value    | 2 bytes | 0x0060–0x2710      | ⭐Baudrate/100 as 2byte hex value  | 
| CRC-16            | 2 bytes | Lo, Hi             | Appended by client; Lo byte first   |

⭐**Important:\!\! The baudrate is divided by 100.** See also Table 1 below

###### Write directly into non volatile memory

```bash
pymcuprog write -o 1 -b 2 -l 0x04 0x80 -m eeprom --clk 57000 -t uart -u com1 -d attiny3224 
```
In the example above, the `UART baudrate` is set to 115200 bps and COM1 is used with an ATtiny3224. The sequence `0x04 0x80` determines the UART baud rate / 100. This three values must be adapted to your own requirements. On Windows, the interface name (com) can be easily read from the Device Manager. On 🐧 Linux\-based systems, you can determine the interface name with the command `ls /dev/tty*`.

| Baudrate | Baudrate/100 | Register value   | Note                       |
|---------:|-------------:|------------------|----------------------------|
| 4,800    | 48           | `0x00 0x30`      | lowest allowed value       |
| 9,600    | 96           | `0x00 0x60`      | default - often used       |
| 19,200   | 192          | `0x00 0xC0`      |                            |
| 115,200  | 1152         | `0x04 0x80`      |                            |
| 1,000,000| 10000        | `0x27 0x10`      | only for short connections |

Table 1 shows the possible baud rates. The default value is 9600 bps.

The lowest possible/allowed value is 4800 bps. 

#### Reading the non volatile memory
The contents of the non-volatile memory can be read with the following command.  
```bash
pymcuprog read -o 0 -b 3 -m eeprom --clk 57000 -t uart -u com1 -d attiny3224
```

| Address | Field Name                 | 
|--------:|----------------------------|
| 0x00    | MODBUS\-ID                 | 
| 0x01    | UART Baud Rate High Byte   | 
| 0x02    | UART Baud Rate Low Byte    | 

### Associating MODBUS Data model to microcontrollers peripheral 
In the file `config.h`, you can define the association of the data model to the microcontroller features and peripherals.
#### `COILS` and `INPUT DISCRETES`
`COILS` are mapped to output pins and `INPUT DISCRETES` are mapped to input pins. 


| Number of Pins |
| 24 | 20 | 14 | COIL | INPUT DISCRETES |
|-------:|-------:|-------:|---------------|------|
| x      | x      | x      | PA3 |  PA3    |
| x      | x      | x      | PA4 |  PA4   | 
| x      | x      | x      | PA5 |  PA5   | 
| x      | x      | x      | PA6 |  PA6   | 
| x      | x      | x      | PA7 |  PA7    | 
| x      | x      | x      | PB0 |    PB0    | 
| x      | x      | x      | PB1 | PB1    | 
| x      | x      | x      | RB2 | PB2    | 
| x      | x      | x      | RB3 | RB3     | 
| x      | x      |        | RB4 | RB4     | 
| x      | x      |        | RB5 | RB5     | 
| x      | x      |        | RC0 | RC0   | 
| x      | x      |        | RC1 | RC1     | 
| x      | x      |        | RC2 | RC2     |  
| x      | x      |        | RC3 | RC3    |  
| x      |        |        | RC4 | RC4     |  
| x      |        |        | RC5 | RC5     |  

#### `HOLDING REGISTERS`
The `HOLDING REGISTERS` are mapped to to PWM outputs. The PWM Frequency and duty cycle can be adjusted by writing into the corresponding registers. There are 3 PMW outputs on three dedicated pins (PB0, PB1, PB2) possible. The PWM Frequency is in Hz and the duty cycle is in percent.  The range for the Frequency is 1Hz to 65535Hz and the range for the duty cycle is 0% to 100%.  The PWM signals are generated with **dual slope mode**, therefore a full bridge driver can be realized with the PWM outputs.  

| Number of Pins |
| 24 | 20 | 14 | Holding registers | Value range | Unit |
|-------:|-------:|-------:|------------------:|------------:|------|
| x      | x      | x      | HOLDING_PWM_PB0_DUTY | 0...100  | %|
| x      | x      | x      | HOLDING_PWM_PB1_DUTY | 0...100 | %|
| x      | x      | x      | HOLDING_PWM_PB2_DUTY | 0...100 | %|
| x      | x      | x      | HOLDING_PWM_FREQ | 1...65535 | Hz |


#### `INPUT REGISTERS `
The `INPUT REGISTERS` are mapped to ADC inputs. The ADC resolution is always 12 bits.  
The ADC input pins can be selected by writing into the corresponding registers.  

| Number of Pins |
| 24 | 20 | 14 | Input register | -IN  | +IN | Conversion mode | Value range |
|-------:|-------:|-------:|---------------|------|-----|------------|--------------|
| x      | x      | x      | INPUT_ADC_PA3 | GND    | PA3   | Single ended  | 0...4095 |
| x      | x      | x      | INPUT_ADC_PA4 | GND    | PA4   | Single ended   |0...4095 |
| x      | x      | x      | INPUT_ADC_PA5 | GND    | PA5   | Single ended    |0...4095 |
| x      | x      | x      | INPUT_ADC_PA6 | GND    | PA6   | Single ended    |0...4095 |
| x      | x      | x      | INPUT_ADC_PA7 | GND    | PA7   | Single ended    |0...4095 |
| x      | x      | x      | INPUT_ADC_PB0 | GND    | PB0   | Single ended   |0...4095 |
| x      | x      | x      | INPUT_ADC_PB1 | GND    | PB1   | Single ended    |0...4095 |
| x      | x      |        | INPUT_ADC_PB4 | GND    | PB4   | Single ended    |0...4095 |
| x      | x      |        | INPUT_ADC_PB5 | GND    | PB5   | Single ended    |0...4095 |
| x      | x      |        | INPUT_ADC_PC0 | GND    | PC0   | Single ended    |0...4095 |
| x      | x      |        | INPUT_ADC_PC1 | GND    | PC1   | Single ended   |0...4095 |
| x      | x      |        | INPUT_ADC_PC2 | GND    | PC2   | Single ended    |0...4095 |
| x      | x      |        | INPUT_ADC_PC3 | GND    | PC3   | Single ended    |0...4095 |
| x      | x      | x      | INPUT_ADC_PA4_PA5 | PA4    | PA5   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_PA6_PA7 | PA6    | PA7   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PA4 | ACREF    | PA4   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PA5 | ACREF    | PA5   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PA6 | ACREF    | PA6   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PA7 | ACREF    | PA7   | Differential    |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PB0 | ACREF    | PB0   | Differential     |-2048...2047 |
| x      | x      | x      | INPUT_ADC_ACREF_PB1 | ACREF    | PB1   | Differential    |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PB4 | ACREF    | PB4   | Differential     |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PB5 | ACREF    | PB5   | Differential     |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PC0 | ACREF    | PC0   | Differential     |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PC1 | ACREF    | PC1   | Differential    |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PC2 | ACREF    | PC2   | Differential    |-2048...2047 |
| x      | x      |        | INPUT_ADC_ACREF_PC3 | ACREF    | PC3   | Differential    |-2048...2047 |

##### Single ended
INPUT_ADC_PA3 is a single ended input pin.
##### Differential
INPUT_ADC_PA4_PA5, INPUT_ADC_PA6_PA7, are differential input pins. The first Pin is the negative input and the second Pin is the positive input.
##### Single ended against reference voltage
The reference voltage can be adjusted in the config.h file by changing the value of the reference voltage. 

### Reference Voltage for ADC
|        | VRef in mV | Resolution in mV/DIG| 
|------:|------------------:|------------:|
| ADC_REF_2500MV | 2500 | 0.61035 | 
| ADC_REF_1024MV | 1024 | 0.25 |
| ADC_REF_2048MV | 2048 | 0.5 |
| ADC_REF_4096MV | 4096 | 1 |
| ADC_REF_VDD | VDD | VDD / 4096 |
| ADC_REF_VREFA | VREFA | VREFA / 4096 |

### Reference Voltage for Analog Comperator 
Used for -Input of ADC if Single ended against reference voltage is selected

|  | Base in mV | Vref in mV | 
|-------:|------------------:|------------:|
| AC_REF_2500MV | 2500 | 9.765625 * AC_REFERENCE_DIVIDER |
| AC_REF_1024MV | 1024 | 4 * AC_REFERENCE_DIVIDER |
| AC_REF_2048MV | 2048 | 8 * AC_REFERENCE_DIVIDER |
| AC_REF_4096MV | 4096 | 16 * AC_REFERENCE_DIVIDER |
| AC_REF_VDD | VDD | VDD / 256 * AC_REFERENCE_DIVIDER  |

The `AC_REFERENCE_DIVIDER` is a divider for the reference voltage. It can be also defined in the config.h file. The default value is 128.  
Example:   
```bash
#define AC_REFERENCE AC_REF_1024MV  
#define AC_REFERENCE_DIVIDER 100  
```
Vref = 2048mV / 256 * 125 = **1V**  

## Hardware considerations

Pins **PA1 and PA2** are permanently assigned to RS485 communication via the UART and therefore cannot be used for other purposes. All remaining pins are freely available.

## License
This code is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this code; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

