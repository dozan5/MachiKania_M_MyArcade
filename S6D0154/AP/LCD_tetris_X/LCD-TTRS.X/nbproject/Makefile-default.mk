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
FINAL_IMAGE=${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../LCDdriver.c ../tetris.c ../tetrisfont.c ../recscore.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/tetris.o ${OBJECTDIR}/_ext/1472/tetrisfont.o ${OBJECTDIR}/_ext/1472/recscore.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/LCDdriver.o.d ${OBJECTDIR}/_ext/1472/tetris.o.d ${OBJECTDIR}/_ext/1472/tetrisfont.o.d ${OBJECTDIR}/_ext/1472/recscore.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/LCDdriver.o ${OBJECTDIR}/_ext/1472/tetris.o ${OBJECTDIR}/_ext/1472/tetrisfont.o ${OBJECTDIR}/_ext/1472/recscore.o

# Source Files
SOURCEFILES=../LCDdriver.c ../tetris.c ../tetrisfont.c ../recscore.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/a8fa1b4fceff4943e9bb682529b7a88af00c7a10 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/tetris.o: ../tetris.c  .generated_files/flags/default/4284eed732a728721d9c2d7d2ab0dda0a10df4ed .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/tetris.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/tetris.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/tetris.o.d" -o ${OBJECTDIR}/_ext/1472/tetris.o ../tetris.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/tetrisfont.o: ../tetrisfont.c  .generated_files/flags/default/66dab57c71dfbf548ce1668ad64d83d0bade6ea3 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/tetrisfont.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/tetrisfont.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/tetrisfont.o.d" -o ${OBJECTDIR}/_ext/1472/tetrisfont.o ../tetrisfont.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/recscore.o: ../recscore.c  .generated_files/flags/default/5a3b53c9cabc72babb39c63b734a9e3491aeda87 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/recscore.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/recscore.o.d" -o ${OBJECTDIR}/_ext/1472/recscore.o ../recscore.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
else
${OBJECTDIR}/_ext/1472/LCDdriver.o: ../LCDdriver.c  .generated_files/flags/default/212253def672d4a93632c926948eafa34e7861b5 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/LCDdriver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/LCDdriver.o.d" -o ${OBJECTDIR}/_ext/1472/LCDdriver.o ../LCDdriver.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/tetris.o: ../tetris.c  .generated_files/flags/default/4b1a49446256ff309ddd652cce374991d7585122 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/tetris.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/tetris.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/tetris.o.d" -o ${OBJECTDIR}/_ext/1472/tetris.o ../tetris.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/tetrisfont.o: ../tetrisfont.c  .generated_files/flags/default/4f0bef49cbff43e43d651b1d82a005fcdc6fd1f4 .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/tetrisfont.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/tetrisfont.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -O1 -MP -MMD -MF "${OBJECTDIR}/_ext/1472/tetrisfont.o.d" -o ${OBJECTDIR}/_ext/1472/tetrisfont.o ../tetrisfont.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)    
	
${OBJECTDIR}/_ext/1472/recscore.o: ../recscore.c  .generated_files/flags/default/dfac893eb4d1e8a7cc1ddecad9ca9f37da176fff .generated_files/flags/default/1d447ed2c61cac8a5a27d2ab970f84c7592e516e
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
${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	
else
${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../app_p32MX370F512H.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" 
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/LCD-TTRS.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
