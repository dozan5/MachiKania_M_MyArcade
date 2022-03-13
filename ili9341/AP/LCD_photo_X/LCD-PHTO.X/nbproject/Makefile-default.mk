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
FINAL_IMAGE=${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../CustomDisplayDriver.c ../FSIO.c ../ImageDecoder.c ../JpegDecoder.c ../LCDdriver.c ../SD-SPI.c ../jidctint.c ../photo.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o ${OBJECTDIR}/_ext/1472/FSIO.o ${OBJECTDIR}/_ext/1472/ImageDecoder.o ${OBJECTDIR}/_ext/1472/JpegDecoder.o ${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/SD-SPI.o ${OBJECTDIR}/_ext/1472/jidctint.o ${OBJECTDIR}/_ext/1472/photo.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o.d ${OBJECTDIR}/_ext/1472/FSIO.o.d ${OBJECTDIR}/_ext/1472/ImageDecoder.o.d ${OBJECTDIR}/_ext/1472/JpegDecoder.o.d ${OBJECTDIR}/_ext/1472/LCDdriver.o.d ${OBJECTDIR}/_ext/1472/SD-SPI.o.d ${OBJECTDIR}/_ext/1472/jidctint.o.d ${OBJECTDIR}/_ext/1472/photo.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o ${OBJECTDIR}/_ext/1472/FSIO.o ${OBJECTDIR}/_ext/1472/ImageDecoder.o ${OBJECTDIR}/_ext/1472/JpegDecoder.o ${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/SD-SPI.o ${OBJECTDIR}/_ext/1472/jidctint.o ${OBJECTDIR}/_ext/1472/photo.o

# Source Files
SOURCEFILES=../CustomDisplayDriver.c ../FSIO.c ../ImageDecoder.c ../JpegDecoder.c ../LCDdriver.c ../SD-SPI.c ../jidctint.c ../photo.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX370F512H
MP_LINKER_FILE_OPTION=,--script="..\app_p32MX370F512H.ld"
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
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o: ../CustomDisplayDriver.c  .generated_files/flags/default/3076da043e5486d4e4d2077012b6b112c02a210b .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o.d" -o ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o ../CustomDisplayDriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/FSIO.o: ../FSIO.c  .generated_files/flags/default/db07938df0de8e05b246ceb04e9fde57b38c32f .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/FSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FSIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/FSIO.o.d" -o ${OBJECTDIR}/_ext/1472/FSIO.o ../FSIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/ImageDecoder.o: ../ImageDecoder.c  .generated_files/flags/default/b3424d505ab02d374c38a456eaee5f9996069fec .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ImageDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ImageDecoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/1472/ImageDecoder.o ../ImageDecoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/JpegDecoder.o: ../JpegDecoder.c  .generated_files/flags/default/3a445ab06470582796428f265cb5883592319d8f .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/JpegDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/JpegDecoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/1472/JpegDecoder.o ../JpegDecoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/eeed71825179da0e4effa14acfad421f3a1e9a96 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/SD-SPI.o: ../SD-SPI.c  .generated_files/flags/default/24dfaf22ed56d3fae8edd8a29337f37239deb4ad .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SD-SPI.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/SD-SPI.o.d" -o ${OBJECTDIR}/_ext/1472/SD-SPI.o ../SD-SPI.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/jidctint.o: ../jidctint.c  .generated_files/flags/default/8a44d6a88950348a81d195e5771c9f08d4df6969 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/jidctint.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/jidctint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/jidctint.o.d" -o ${OBJECTDIR}/_ext/1472/jidctint.o ../jidctint.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/photo.o: ../photo.c  .generated_files/flags/default/4d5a0f26903ac66cbfbc21ee3dea2bf5d1b00131 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/photo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/photo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/photo.o.d" -o ${OBJECTDIR}/_ext/1472/photo.o ../photo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o: ../CustomDisplayDriver.c  .generated_files/flags/default/a4cabc5cae3c6e1aa5ce6112592ee5d566f09775 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o.d" -o ${OBJECTDIR}/_ext/1472/CustomDisplayDriver.o ../CustomDisplayDriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/FSIO.o: ../FSIO.c  .generated_files/flags/default/b2cbea0dd65c96bc0b8e44b2e524355778330118 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/FSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FSIO.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/FSIO.o.d" -o ${OBJECTDIR}/_ext/1472/FSIO.o ../FSIO.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/ImageDecoder.o: ../ImageDecoder.c  .generated_files/flags/default/ee498052b3347c322d8f0eb5201fcf24f35d6d63 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/ImageDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/ImageDecoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/1472/ImageDecoder.o ../ImageDecoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/JpegDecoder.o: ../JpegDecoder.c  .generated_files/flags/default/b8eeebf87d060300362c8ce65d8fea455784cba2 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/JpegDecoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/JpegDecoder.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/1472/JpegDecoder.o ../JpegDecoder.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/1507512235e2fe4ed0c459e3281d6ac396b2e408 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/SD-SPI.o: ../SD-SPI.c  .generated_files/flags/default/3789d97763ca3b827d508cde38cd7e3a5f2a9bcc .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SD-SPI.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/SD-SPI.o.d" -o ${OBJECTDIR}/_ext/1472/SD-SPI.o ../SD-SPI.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/jidctint.o: ../jidctint.c  .generated_files/flags/default/24fad21758db1d90e2a85ca6ac75f9fa31d1c893 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/jidctint.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/jidctint.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/jidctint.o.d" -o ${OBJECTDIR}/_ext/1472/jidctint.o ../jidctint.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/photo.o: ../photo.c  .generated_files/flags/default/e00a961b1bc19744020ce3a3c38cec282251b927 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/photo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/photo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/photo.o.d" -o ${OBJECTDIR}/_ext/1472/photo.o ../photo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	
else
${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/LCD-PHTO.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
