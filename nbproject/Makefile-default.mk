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
SOURCEFILES_QUOTED_IF_SPACED=src/main/adc.c src/main/crc.c src/main/main.c src/main/modbusServer.c src/main/pwm.c src/main/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main/adc.o ${OBJECTDIR}/src/main/crc.o ${OBJECTDIR}/src/main/main.o ${OBJECTDIR}/src/main/modbusServer.o ${OBJECTDIR}/src/main/pwm.o ${OBJECTDIR}/src/main/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main/adc.o.d ${OBJECTDIR}/src/main/crc.o.d ${OBJECTDIR}/src/main/main.o.d ${OBJECTDIR}/src/main/modbusServer.o.d ${OBJECTDIR}/src/main/pwm.o.d ${OBJECTDIR}/src/main/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main/adc.o ${OBJECTDIR}/src/main/crc.o ${OBJECTDIR}/src/main/main.o ${OBJECTDIR}/src/main/modbusServer.o ${OBJECTDIR}/src/main/pwm.o ${OBJECTDIR}/src/main/uart.o

# Source Files
SOURCEFILES=src/main/adc.c src/main/crc.c src/main/main.c src/main/modbusServer.c src/main/pwm.c src/main/uart.c



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
${OBJECTDIR}/src/main/adc.o: src/main/adc.c  .generated_files/flags/default/45055381b102d31905f802b876141b3e86e90eed .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/adc.o.d 
	@${RM} ${OBJECTDIR}/src/main/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/adc.o.d" -MT "${OBJECTDIR}/src/main/adc.o.d" -MT ${OBJECTDIR}/src/main/adc.o -o ${OBJECTDIR}/src/main/adc.o src/main/adc.c 
	
${OBJECTDIR}/src/main/crc.o: src/main/crc.c  .generated_files/flags/default/f99c43ee2a1096441258d43bb1fbdd2ddb71d927 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/crc.o.d 
	@${RM} ${OBJECTDIR}/src/main/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/crc.o.d" -MT "${OBJECTDIR}/src/main/crc.o.d" -MT ${OBJECTDIR}/src/main/crc.o -o ${OBJECTDIR}/src/main/crc.o src/main/crc.c 
	
${OBJECTDIR}/src/main/main.o: src/main/main.c  .generated_files/flags/default/82a241f3eeb4ea3bc0c35ceddc37e1cba33b52f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/main.o.d 
	@${RM} ${OBJECTDIR}/src/main/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/main.o.d" -MT "${OBJECTDIR}/src/main/main.o.d" -MT ${OBJECTDIR}/src/main/main.o -o ${OBJECTDIR}/src/main/main.o src/main/main.c 
	
${OBJECTDIR}/src/main/modbusServer.o: src/main/modbusServer.c  .generated_files/flags/default/ec9e4a546f40687aa43f71bedff1b367c09ece62 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/modbusServer.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbusServer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbusServer.o.d" -MT "${OBJECTDIR}/src/main/modbusServer.o.d" -MT ${OBJECTDIR}/src/main/modbusServer.o -o ${OBJECTDIR}/src/main/modbusServer.o src/main/modbusServer.c 
	
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
	
else
${OBJECTDIR}/src/main/adc.o: src/main/adc.c  .generated_files/flags/default/94bedf44cfc27b13b52e1fe695ed9b0e1b72fb15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/adc.o.d 
	@${RM} ${OBJECTDIR}/src/main/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/adc.o.d" -MT "${OBJECTDIR}/src/main/adc.o.d" -MT ${OBJECTDIR}/src/main/adc.o -o ${OBJECTDIR}/src/main/adc.o src/main/adc.c 
	
${OBJECTDIR}/src/main/crc.o: src/main/crc.c  .generated_files/flags/default/8981b08fbcba2656163a7823c90acb37280cb37 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/crc.o.d 
	@${RM} ${OBJECTDIR}/src/main/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/crc.o.d" -MT "${OBJECTDIR}/src/main/crc.o.d" -MT ${OBJECTDIR}/src/main/crc.o -o ${OBJECTDIR}/src/main/crc.o src/main/crc.c 
	
${OBJECTDIR}/src/main/main.o: src/main/main.c  .generated_files/flags/default/704705a1db72d2aef0770555c052e6695c14f9e5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/main.o.d 
	@${RM} ${OBJECTDIR}/src/main/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/main.o.d" -MT "${OBJECTDIR}/src/main/main.o.d" -MT ${OBJECTDIR}/src/main/main.o -o ${OBJECTDIR}/src/main/main.o src/main/main.c 
	
${OBJECTDIR}/src/main/modbusServer.o: src/main/modbusServer.c  .generated_files/flags/default/74112f218b3ce4ed36847cd5af56fb46e617a6b0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/src/main" 
	@${RM} ${OBJECTDIR}/src/main/modbusServer.o.d 
	@${RM} ${OBJECTDIR}/src/main/modbusServer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O2 -fno-common -funsigned-char -funsigned-bitfields -DF_CPU=20000000 -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3 -mno-const-data-in-progmem     -MD -MP -MF "${OBJECTDIR}/src/main/modbusServer.o.d" -MT "${OBJECTDIR}/src/main/modbusServer.o.d" -MT ${OBJECTDIR}/src/main/modbusServer.o -o ${OBJECTDIR}/src/main/modbusServer.o src/main/modbusServer.c 
	
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
