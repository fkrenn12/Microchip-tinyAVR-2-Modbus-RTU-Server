#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/main/modbus_rtu/server.c src/main/modbus_rtu/function_01_read_coils.c src/main/modbus_rtu/function_02_read_discrete_inputs.c src/main/modbus_rtu/function_03_read_holding_registers.c src/main/modbus_rtu/function_04_read_input_registers.c src/main/modbus_rtu/function_05_write_single_coil.c src/main/modbus_rtu/function_06_write_single_holding_register.c src/main/modbus_rtu/function_15_write_multiple_coils.c src/main/modbus_rtu/function_16_write_multiple_holding_registers.c src/main/adc.c src/main/main.c src/main/pwm.c src/main/uart.c src/main/configuration.c src/main/register.c src/main/update_register.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main/modbus_rtu/server.o ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o ${OBJECTDIR}/src/main/adc.o ${OBJECTDIR}/src/main/main.o ${OBJECTDIR}/src/main/pwm.o ${OBJECTDIR}/src/main/uart.o ${OBJECTDIR}/src/main/configuration.o ${OBJECTDIR}/src/main/register.o ${OBJECTDIR}/src/main/update_register.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main/modbus_rtu/server.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d ${OBJECTDIR}/src/main/adc.o.d ${OBJECTDIR}/src/main/main.o.d ${OBJECTDIR}/src/main/pwm.o.d ${OBJECTDIR}/src/main/uart.o.d ${OBJECTDIR}/src/main/configuration.o.d ${OBJECTDIR}/src/main/register.o.d ${OBJECTDIR}/src/main/update_register.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main/modbus_rtu/server.o ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o ${OBJECTDIR}/src/main/adc.o ${OBJECTDIR}/src/main/main.o ${OBJECTDIR}/src/main/pwm.o ${OBJECTDIR}/src/main/uart.o ${OBJECTDIR}/src/main/configuration.o ${OBJECTDIR}/src/main/register.o ${OBJECTDIR}/src/main/update_register.o

# Source Files
SOURCEFILES=src/main/modbus_rtu/server.c src/main/modbus_rtu/function_01_read_coils.c src/main/modbus_rtu/function_02_read_discrete_inputs.c src/main/modbus_rtu/function_03_read_holding_registers.c src/main/modbus_rtu/function_04_read_input_registers.c src/main/modbus_rtu/function_05_write_single_coil.c src/main/modbus_rtu/function_06_write_single_holding_register.c src/main/modbus_rtu/function_15_write_multiple_coils.c src/main/modbus_rtu/function_16_write_multiple_holding_registers.c src/main/adc.c src/main/main.c src/main/pwm.c src/main/uart.c src/main/configuration.c src/main/register.c src/main/update_register.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny3224
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/main/modbus_rtu/server.o: src/main/modbus_rtu/server.c  .generated_files/flags/default/5ab81be569a21696604f8cfb29631332be165f88 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/server.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/server.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/server.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/server.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/server.o -o ${OBJECTDIR}/src/main/modbus_rtu/server.o src/main/modbus_rtu/server.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o: src/main/modbus_rtu/function_01_read_coils.c  .generated_files/flags/default/a7cdf833aad7db8295f1353ca66f4a7be20860d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o src/main/modbus_rtu/function_01_read_coils.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o: src/main/modbus_rtu/function_02_read_discrete_inputs.c  .generated_files/flags/default/3038b828bbf2bd15c5f53d4db86e414a36fc417 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o src/main/modbus_rtu/function_02_read_discrete_inputs.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o: src/main/modbus_rtu/function_03_read_holding_registers.c  .generated_files/flags/default/cb47bcd41d2edecb365337473722660121abf02d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o src/main/modbus_rtu/function_03_read_holding_registers.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o: src/main/modbus_rtu/function_04_read_input_registers.c  .generated_files/flags/default/308d7d106d834530c3221a16b395a8b4369bcec6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o src/main/modbus_rtu/function_04_read_input_registers.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o: src/main/modbus_rtu/function_05_write_single_coil.c  .generated_files/flags/default/df99ef282937e503206cb5ef67aa28aafdc39a79 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o src/main/modbus_rtu/function_05_write_single_coil.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o: src/main/modbus_rtu/function_06_write_single_holding_register.c  .generated_files/flags/default/aaadc955887f1426dc86ef11a1ab00f000b62a20 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o src/main/modbus_rtu/function_06_write_single_holding_register.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o: src/main/modbus_rtu/function_15_write_multiple_coils.c  .generated_files/flags/default/5067ddbe8cf9c48b2e1b26b0760c5615744eb8f3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o src/main/modbus_rtu/function_15_write_multiple_coils.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o: src/main/modbus_rtu/function_16_write_multiple_holding_registers.c  .generated_files/flags/default/d1b5abf815376f6389bf8535fca208324a7195a7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o src/main/modbus_rtu/function_16_write_multiple_holding_registers.c 
	
${OBJECTDIR}/src/main/adc.o: src/main/adc.c  .generated_files/flags/default/45055381b102d31905f802b876141b3e86e90eed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/adc.o.d 
	@${RM} ${OBJECTDIR}/src/main/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/adc.o.d" -MT "${OBJECTDIR}/src/main/adc.o.d" -MT ${OBJECTDIR}/src/main/adc.o -o ${OBJECTDIR}/src/main/adc.o src/main/adc.c 
	
${OBJECTDIR}/src/main/main.o: src/main/main.c  .generated_files/flags/default/82a241f3eeb4ea3bc0c35ceddc37e1cba33b52f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/main.o.d 
	@${RM} ${OBJECTDIR}/src/main/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/main.o.d" -MT "${OBJECTDIR}/src/main/main.o.d" -MT ${OBJECTDIR}/src/main/main.o -o ${OBJECTDIR}/src/main/main.o src/main/main.c 
	
${OBJECTDIR}/src/main/pwm.o: src/main/pwm.c  .generated_files/flags/default/7c263e7472bea46ac3edf478039cd29b64f2f98e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/main/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/pwm.o.d" -MT "${OBJECTDIR}/src/main/pwm.o.d" -MT ${OBJECTDIR}/src/main/pwm.o -o ${OBJECTDIR}/src/main/pwm.o src/main/pwm.c 
	
${OBJECTDIR}/src/main/uart.o: src/main/uart.c  .generated_files/flags/default/6d0d75356360fe8c87becbdf88f4279112ec93a8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/uart.o.d 
	@${RM} ${OBJECTDIR}/src/main/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/uart.o.d" -MT "${OBJECTDIR}/src/main/uart.o.d" -MT ${OBJECTDIR}/src/main/uart.o -o ${OBJECTDIR}/src/main/uart.o src/main/uart.c 
	
${OBJECTDIR}/src/main/configuration.o: src/main/configuration.c  .generated_files/flags/default/9696c71665ce75463885b17320e8f833a1e86694 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/configuration.o.d 
	@${RM} ${OBJECTDIR}/src/main/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/configuration.o.d" -MT "${OBJECTDIR}/src/main/configuration.o.d" -MT ${OBJECTDIR}/src/main/configuration.o -o ${OBJECTDIR}/src/main/configuration.o src/main/configuration.c 
	
${OBJECTDIR}/src/main/register.o: src/main/register.c  .generated_files/flags/default/a23562545189c332f45af77238a49e3fbf15cfa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/register.o.d 
	@${RM} ${OBJECTDIR}/src/main/register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/register.o.d" -MT "${OBJECTDIR}/src/main/register.o.d" -MT ${OBJECTDIR}/src/main/register.o -o ${OBJECTDIR}/src/main/register.o src/main/register.c 
	
${OBJECTDIR}/src/main/update_register.o: src/main/update_register.c  .generated_files/flags/default/51986fee898dbf3e1b6cb4f133ecd8c192fa65c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/update_register.o.d 
	@${RM} ${OBJECTDIR}/src/main/update_register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/update_register.o.d" -MT "${OBJECTDIR}/src/main/update_register.o.d" -MT ${OBJECTDIR}/src/main/update_register.o -o ${OBJECTDIR}/src/main/update_register.o src/main/update_register.c 
	
else
${OBJECTDIR}/src/main/modbus_rtu/server.o: src/main/modbus_rtu/server.c  .generated_files/flags/default/7a99d45be40c615c097db6667d60343101059592 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/server.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/server.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/server.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/server.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/server.o -o ${OBJECTDIR}/src/main/modbus_rtu/server.o src/main/modbus_rtu/server.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o: src/main/modbus_rtu/function_01_read_coils.c  .generated_files/flags/default/59ba6f88db78d8b0f2aed35fc5b0ce2cedc07901 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_01_read_coils.o src/main/modbus_rtu/function_01_read_coils.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o: src/main/modbus_rtu/function_02_read_discrete_inputs.c  .generated_files/flags/default/8392f017efaf6820485304d9ad0f2698d045d589 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_02_read_discrete_inputs.o src/main/modbus_rtu/function_02_read_discrete_inputs.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o: src/main/modbus_rtu/function_03_read_holding_registers.c  .generated_files/flags/default/fa7ab8aa2daa83f23d1da79921a8ab375277b8e9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_03_read_holding_registers.o src/main/modbus_rtu/function_03_read_holding_registers.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o: src/main/modbus_rtu/function_04_read_input_registers.c  .generated_files/flags/default/b59f7fd40121b46b5a63462e486530b8cfb7299e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_04_read_input_registers.o src/main/modbus_rtu/function_04_read_input_registers.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o: src/main/modbus_rtu/function_05_write_single_coil.c  .generated_files/flags/default/967504b244e656c178adf854e7d14d71e92e05a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_05_write_single_coil.o src/main/modbus_rtu/function_05_write_single_coil.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o: src/main/modbus_rtu/function_06_write_single_holding_register.c  .generated_files/flags/default/9ddbd9d33ac839be8c414f4caac377e528f36c08 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_06_write_single_holding_register.o src/main/modbus_rtu/function_06_write_single_holding_register.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o: src/main/modbus_rtu/function_15_write_multiple_coils.c  .generated_files/flags/default/e2891149560763f2df741fc38a03a59143e36582 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_15_write_multiple_coils.o src/main/modbus_rtu/function_15_write_multiple_coils.c 
	
${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o: src/main/modbus_rtu/function_16_write_multiple_holding_registers.c  .generated_files/flags/default/163ea71317b47d34897b03ab6ec18a60713e2a09 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main/modbus_rtu" 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d" -MT "${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o.d" -MT ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o -o ${OBJECTDIR}/src/main/modbus_rtu/function_16_write_multiple_holding_registers.o src/main/modbus_rtu/function_16_write_multiple_holding_registers.c 
	
${OBJECTDIR}/src/main/adc.o: src/main/adc.c  .generated_files/flags/default/94bedf44cfc27b13b52e1fe695ed9b0e1b72fb15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/adc.o.d 
	@${RM} ${OBJECTDIR}/src/main/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/adc.o.d" -MT "${OBJECTDIR}/src/main/adc.o.d" -MT ${OBJECTDIR}/src/main/adc.o -o ${OBJECTDIR}/src/main/adc.o src/main/adc.c 
	
${OBJECTDIR}/src/main/main.o: src/main/main.c  .generated_files/flags/default/704705a1db72d2aef0770555c052e6695c14f9e5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/main.o.d 
	@${RM} ${OBJECTDIR}/src/main/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/main.o.d" -MT "${OBJECTDIR}/src/main/main.o.d" -MT ${OBJECTDIR}/src/main/main.o -o ${OBJECTDIR}/src/main/main.o src/main/main.c 
	
${OBJECTDIR}/src/main/pwm.o: src/main/pwm.c  .generated_files/flags/default/4f52fbdae561834e70a996d302c6e4c8c5bf849f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/main/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/pwm.o.d" -MT "${OBJECTDIR}/src/main/pwm.o.d" -MT ${OBJECTDIR}/src/main/pwm.o -o ${OBJECTDIR}/src/main/pwm.o src/main/pwm.c 
	
${OBJECTDIR}/src/main/uart.o: src/main/uart.c  .generated_files/flags/default/1f9b90babc1715fa7b8c890e540874cf9b00af39 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/uart.o.d 
	@${RM} ${OBJECTDIR}/src/main/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/uart.o.d" -MT "${OBJECTDIR}/src/main/uart.o.d" -MT ${OBJECTDIR}/src/main/uart.o -o ${OBJECTDIR}/src/main/uart.o src/main/uart.c 
	
${OBJECTDIR}/src/main/configuration.o: src/main/configuration.c  .generated_files/flags/default/d53aa8801dae8a78871fc57c24240bd3f4960a72 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/configuration.o.d 
	@${RM} ${OBJECTDIR}/src/main/configuration.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/configuration.o.d" -MT "${OBJECTDIR}/src/main/configuration.o.d" -MT ${OBJECTDIR}/src/main/configuration.o -o ${OBJECTDIR}/src/main/configuration.o src/main/configuration.c 
	
${OBJECTDIR}/src/main/register.o: src/main/register.c  .generated_files/flags/default/d935a3982711b932efb2f069807584c778d969 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/register.o.d 
	@${RM} ${OBJECTDIR}/src/main/register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/register.o.d" -MT "${OBJECTDIR}/src/main/register.o.d" -MT ${OBJECTDIR}/src/main/register.o -o ${OBJECTDIR}/src/main/register.o src/main/register.c 
	
${OBJECTDIR}/src/main/update_register.o: src/main/update_register.c  .generated_files/flags/default/dbcf5f9a1fec07a1cdefda5bdf8b0c64593e91c0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/update_register.o.d 
	@${RM} ${OBJECTDIR}/src/main/update_register.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/update_register.o.d" -MT "${OBJECTDIR}/src/main/update_register.o.d" -MT ${OBJECTDIR}/src/main/update_register.o -o ${OBJECTDIR}/src/main/update_register.o src/main/update_register.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -gdwarf-3 -mno-const-data-in-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -gdwarf-3 -mno-const-data-in-progmem     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}\\avr-objcopy -O ihex "${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/Microchip-tinyAVR-2-Modbus-RTU-Server.${IMAGE_TYPE}.hex"
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
