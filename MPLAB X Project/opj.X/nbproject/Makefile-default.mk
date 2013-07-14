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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../main.c ../Initialization.c ../Processing.c ../Effects.c ../FoundAlgorithms.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/Initialization.o ${OBJECTDIR}/_ext/1472/Processing.o ${OBJECTDIR}/_ext/1472/Effects.o ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/Initialization.o.d ${OBJECTDIR}/_ext/1472/Processing.o.d ${OBJECTDIR}/_ext/1472/Effects.o.d ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/Initialization.o ${OBJECTDIR}/_ext/1472/Processing.o ${OBJECTDIR}/_ext/1472/Effects.o ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o

# Source Files
SOURCEFILES=../main.c ../Initialization.c ../Processing.c ../Effects.c ../FoundAlgorithms.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ128GP802
MP_LINKER_FILE_OPTION=,-Tp33FJ128GP802.gld
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
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.ok ${OBJECTDIR}/_ext/1472/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/1472/Initialization.o: ../Initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Initialization.o.ok ${OBJECTDIR}/_ext/1472/Initialization.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Initialization.o.d" -o ${OBJECTDIR}/_ext/1472/Initialization.o ../Initialization.c  
	
${OBJECTDIR}/_ext/1472/Processing.o: ../Processing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Processing.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Processing.o.ok ${OBJECTDIR}/_ext/1472/Processing.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Processing.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Processing.o.d" -o ${OBJECTDIR}/_ext/1472/Processing.o ../Processing.c  
	
${OBJECTDIR}/_ext/1472/Effects.o: ../Effects.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Effects.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Effects.o.ok ${OBJECTDIR}/_ext/1472/Effects.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Effects.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Effects.o.d" -o ${OBJECTDIR}/_ext/1472/Effects.o ../Effects.c  
	
${OBJECTDIR}/_ext/1472/FoundAlgorithms.o: ../FoundAlgorithms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.ok ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d" -o ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o ../FoundAlgorithms.c  
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.ok ${OBJECTDIR}/_ext/1472/main.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c  
	
${OBJECTDIR}/_ext/1472/Initialization.o: ../Initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Initialization.o.ok ${OBJECTDIR}/_ext/1472/Initialization.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Initialization.o.d" -o ${OBJECTDIR}/_ext/1472/Initialization.o ../Initialization.c  
	
${OBJECTDIR}/_ext/1472/Processing.o: ../Processing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Processing.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Processing.o.ok ${OBJECTDIR}/_ext/1472/Processing.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Processing.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Processing.o.d" -o ${OBJECTDIR}/_ext/1472/Processing.o ../Processing.c  
	
${OBJECTDIR}/_ext/1472/Effects.o: ../Effects.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/Effects.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/Effects.o.ok ${OBJECTDIR}/_ext/1472/Effects.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/Effects.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/Effects.o.d" -o ${OBJECTDIR}/_ext/1472/Effects.o ../Effects.c  
	
${OBJECTDIR}/_ext/1472/FoundAlgorithms.o: ../FoundAlgorithms.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.ok ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -mlarge-arrays -mlarge-code -mlarge-data -mlarge-scalar -mconst-in-code -Os -funroll-loops -fschedule-insns -fschedule-insns2 -MMD -MF "${OBJECTDIR}/_ext/1472/FoundAlgorithms.o.d" -o ${OBJECTDIR}/_ext/1472/FoundAlgorithms.o ../FoundAlgorithms.c  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "..\..\..\..\..\Program Files\Microchip\MPLAB C30\lib\libq-coff.a"    -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PICKIT2=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../Program\ Files/Microchip/MPLAB\ C30/lib/libq-coff.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   "..\..\..\..\..\Program Files\Microchip\MPLAB C30\lib\libq-coff.a"    -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/opj.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=coff
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
