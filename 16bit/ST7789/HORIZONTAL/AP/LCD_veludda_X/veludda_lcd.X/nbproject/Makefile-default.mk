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
FINAL_IMAGE=${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../bmpdata.s ../graphlib2.c ../musicdata.c ../rotatevideo_lcd.c ../veludda_chars.c ../veludda_lcd.c ../LCDdriver.c ../recscore.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/bmpdata.o ${OBJECTDIR}/_ext/1472/graphlib2.o ${OBJECTDIR}/_ext/1472/musicdata.o ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o ${OBJECTDIR}/_ext/1472/veludda_chars.o ${OBJECTDIR}/_ext/1472/veludda_lcd.o ${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/recscore.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/bmpdata.o.d ${OBJECTDIR}/_ext/1472/graphlib2.o.d ${OBJECTDIR}/_ext/1472/musicdata.o.d ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o.d ${OBJECTDIR}/_ext/1472/veludda_chars.o.d ${OBJECTDIR}/_ext/1472/veludda_lcd.o.d ${OBJECTDIR}/_ext/1472/LCDdriver.o.d ${OBJECTDIR}/_ext/1472/recscore.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/bmpdata.o ${OBJECTDIR}/_ext/1472/graphlib2.o ${OBJECTDIR}/_ext/1472/musicdata.o ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o ${OBJECTDIR}/_ext/1472/veludda_chars.o ${OBJECTDIR}/_ext/1472/veludda_lcd.o ${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/recscore.o

# Source Files
SOURCEFILES=../bmpdata.s ../graphlib2.c ../musicdata.c ../rotatevideo_lcd.c ../veludda_chars.c ../veludda_lcd.c ../LCDdriver.c ../recscore.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX370F512H
MP_LINKER_FILE_OPTION=,--script="..\app_p32MX370F512H.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/bmpdata.o: ../bmpdata.s  .generated_files/flags/default/46d2bee751f526a10d0bb9a64d58f8cc3bee79dc .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bmpdata.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG   -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/_ext/1472/bmpdata.o ../bmpdata.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--gdwarf-2,-MD="${OBJECTDIR}/_ext/1472/bmpdata.o.d" 
	
else
${OBJECTDIR}/_ext/1472/bmpdata.o: ../bmpdata.s  .generated_files/flags/default/9e716b2a443a964fb2fea5b3b2f4b7241827fce4 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/bmpdata.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${OBJECTDIR}/_ext/1472/bmpdata.o ../bmpdata.s  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),--gdwarf-2,-MD="${OBJECTDIR}/_ext/1472/bmpdata.o.d" 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/graphlib2.o: ../graphlib2.c  .generated_files/flags/default/6fb58218c9845848569bc7ebe2916deab72d4eeb .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/graphlib2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/graphlib2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/graphlib2.o.d" -o ${OBJECTDIR}/_ext/1472/graphlib2.o ../graphlib2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/musicdata.o: ../musicdata.c  .generated_files/flags/default/541335820800d9e250e90518c9346b8260ecc740 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/musicdata.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/musicdata.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/musicdata.o.d" -o ${OBJECTDIR}/_ext/1472/musicdata.o ../musicdata.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o: ../rotatevideo_lcd.c  .generated_files/flags/default/c51a5ac8537c1a76b051db35a702ee67af9ecf83 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o.d" -o ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o ../rotatevideo_lcd.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/veludda_chars.o: ../veludda_chars.c  .generated_files/flags/default/8d4bccc782502e4127d62cac98a195c4d63eedd3 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_chars.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_chars.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/veludda_chars.o.d" -o ${OBJECTDIR}/_ext/1472/veludda_chars.o ../veludda_chars.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/veludda_lcd.o: ../veludda_lcd.c  .generated_files/flags/default/eea5fc71e15bdaf51589471cacd8597a22f711ea .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_lcd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/veludda_lcd.o.d" -o ${OBJECTDIR}/_ext/1472/veludda_lcd.o ../veludda_lcd.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/7804a207e293a9817434ffa0bede198b0604d44e .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/recscore.o: ../recscore.c  .generated_files/flags/default/b0bbfb4c12581e7c6cebc6349eda3fa3fc1c80f5 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/recscore.o.d" -o ${OBJECTDIR}/_ext/1472/recscore.o ../recscore.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/graphlib2.o: ../graphlib2.c  .generated_files/flags/default/58007edeef6ca18965f910a12cc122547a02579f .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/graphlib2.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/graphlib2.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/graphlib2.o.d" -o ${OBJECTDIR}/_ext/1472/graphlib2.o ../graphlib2.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/musicdata.o: ../musicdata.c  .generated_files/flags/default/f26aa7057a1e209b28c0140acd22a0aa32aa7e63 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/musicdata.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/musicdata.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/musicdata.o.d" -o ${OBJECTDIR}/_ext/1472/musicdata.o ../musicdata.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o: ../rotatevideo_lcd.c  .generated_files/flags/default/160c780be8de22c51586a38962d628055f08de81 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o.d" -o ${OBJECTDIR}/_ext/1472/rotatevideo_lcd.o ../rotatevideo_lcd.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/veludda_chars.o: ../veludda_chars.c  .generated_files/flags/default/660a06af513ff3f90a070cf22083a23096164c13 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_chars.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_chars.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/veludda_chars.o.d" -o ${OBJECTDIR}/_ext/1472/veludda_chars.o ../veludda_chars.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/veludda_lcd.o: ../veludda_lcd.c  .generated_files/flags/default/af91355a14f1b69cfec47946a35cb8b08ddc6a4f .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/veludda_lcd.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/veludda_lcd.o.d" -o ${OBJECTDIR}/_ext/1472/veludda_lcd.o ../veludda_lcd.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/2cceda7a5c40d93c029d6c74b8d64dbaeb64f8e3 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/recscore.o: ../recscore.c  .generated_files/flags/default/8f3c0078f15e44409ee79206c2745b371520c2bc .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/recscore.o.d" -o ${OBJECTDIR}/_ext/1472/recscore.o ../recscore.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC0275F  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	
else
${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/veludda_lcd.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
