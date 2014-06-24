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
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../runner/runner.c ../dspic33_dsc_sk1/WM8510CodecDrv.c ../dspic33_dsc_sk1/sk1_Initialization.c ../dspic33_dsc_sk1/sk1_main.c ../effects.c ../foundation.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/835468705/runner.o ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o ${OBJECTDIR}/_ext/354150267/sk1_main.o ${OBJECTDIR}/_ext/1472/effects.o ${OBJECTDIR}/_ext/1472/foundation.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/835468705/runner.o.d ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d ${OBJECTDIR}/_ext/354150267/sk1_main.o.d ${OBJECTDIR}/_ext/1472/effects.o.d ${OBJECTDIR}/_ext/1472/foundation.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/835468705/runner.o ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o ${OBJECTDIR}/_ext/354150267/sk1_main.o ${OBJECTDIR}/_ext/1472/effects.o ${OBJECTDIR}/_ext/1472/foundation.o

# Source Files
SOURCEFILES=../runner/runner.c ../dspic33_dsc_sk1/WM8510CodecDrv.c ../dspic33_dsc_sk1/sk1_Initialization.c ../dspic33_dsc_sk1/sk1_main.c ../effects.c ../foundation.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128GP706
MP_LINKER_FILE_OPTION=,-Tp33FJ128GP706.gld
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
${OBJECTDIR}/_ext/835468705/runner.o: ../runner/runner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/835468705 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o.d 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o.ok ${OBJECTDIR}/_ext/835468705/runner.o.err 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/835468705/runner.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/835468705/runner.o.d" -o ${OBJECTDIR}/_ext/835468705/runner.o ../runner/runner.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o: ../dspic33_dsc_sk1/WM8510CodecDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.ok ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d" -o ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o ../dspic33_dsc_sk1/WM8510CodecDrv.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/sk1_Initialization.o: ../dspic33_dsc_sk1/sk1_Initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.ok ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d" -o ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o ../dspic33_dsc_sk1/sk1_Initialization.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/sk1_main.o: ../dspic33_dsc_sk1/sk1_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o.ok ${OBJECTDIR}/_ext/354150267/sk1_main.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/sk1_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/sk1_main.o.d" -o ${OBJECTDIR}/_ext/354150267/sk1_main.o ../dspic33_dsc_sk1/sk1_main.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/1472/effects.o: ../effects.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o.ok ${OBJECTDIR}/_ext/1472/effects.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/effects.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/1472/effects.o.d" -o ${OBJECTDIR}/_ext/1472/effects.o ../effects.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/1472/foundation.o: ../foundation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o.ok ${OBJECTDIR}/_ext/1472/foundation.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/foundation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/1472/foundation.o.d" -o ${OBJECTDIR}/_ext/1472/foundation.o ../foundation.c    -fast-math -legacy-libc
	
else
${OBJECTDIR}/_ext/835468705/runner.o: ../runner/runner.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/835468705 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o.d 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o.ok ${OBJECTDIR}/_ext/835468705/runner.o.err 
	@${RM} ${OBJECTDIR}/_ext/835468705/runner.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/835468705/runner.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/835468705/runner.o.d" -o ${OBJECTDIR}/_ext/835468705/runner.o ../runner/runner.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o: ../dspic33_dsc_sk1/WM8510CodecDrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.ok ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o.d" -o ${OBJECTDIR}/_ext/354150267/WM8510CodecDrv.o ../dspic33_dsc_sk1/WM8510CodecDrv.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/sk1_Initialization.o: ../dspic33_dsc_sk1/sk1_Initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.ok ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/sk1_Initialization.o.d" -o ${OBJECTDIR}/_ext/354150267/sk1_Initialization.o ../dspic33_dsc_sk1/sk1_Initialization.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/354150267/sk1_main.o: ../dspic33_dsc_sk1/sk1_main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/354150267 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o.d 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o.ok ${OBJECTDIR}/_ext/354150267/sk1_main.o.err 
	@${RM} ${OBJECTDIR}/_ext/354150267/sk1_main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/354150267/sk1_main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/354150267/sk1_main.o.d" -o ${OBJECTDIR}/_ext/354150267/sk1_main.o ../dspic33_dsc_sk1/sk1_main.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/1472/effects.o: ../effects.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o.ok ${OBJECTDIR}/_ext/1472/effects.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/effects.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/effects.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/1472/effects.o.d" -o ${OBJECTDIR}/_ext/1472/effects.o ../effects.c    -fast-math -legacy-libc
	
${OBJECTDIR}/_ext/1472/foundation.o: ../foundation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o.ok ${OBJECTDIR}/_ext/1472/foundation.o.err 
	@${RM} ${OBJECTDIR}/_ext/1472/foundation.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/foundation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -ffunction-sections -Wall -msmall-code -mlarge-data -mconst-in-code -MMD -MF "${OBJECTDIR}/_ext/1472/foundation.o.d" -o ${OBJECTDIR}/_ext/1472/foundation.o ../foundation.c    -fast-math -legacy-libc
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "..\..\..\Program Files\Microchip\MPLAB C30\lib\libq-coff.a"     -fast-math -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--stack=16,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "..\..\..\Program Files\Microchip\MPLAB C30\lib\libq-coff.a"     -fast-math -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1,--stack=16,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/dspic33_dsc_sk1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=coff
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
