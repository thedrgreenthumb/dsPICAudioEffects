/**********************************************************************
* FileName:        AlgosDef.h
* FileVersion      1.01
* 
* Architecture:    _
* IDE:             _
* Compiller:       _
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         Algos library
* ProjectVersion:  _
* Date:            260513
**********************************************************************/

#ifndef _GLOBALMACRO_H
#define _GLOBALMACRO_H

//Microchip fractional library
#include <libq.h>

#define ALGOS_LIB_VERSION ("dsPIC:0.01")

#ifndef NULL
	#define NULL ((void*)0)
#endif

//Building modes
#define DEBUG_MODE                                                               //General debug mode
#define SIGNALS_CHECKING_MODE                                                    //Check clipping in signal passes
#define ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE                                 //Clean up Algo operation buf if Algo switched
#define IGNORE_NUMBER_OF_CLIPPED_SAMPLES (1000)                                  //Do not stop processing if samples are clipped

//Configuration constants
static const unsigned int algosDefcfgdat[2] = {
#include "./AlgosDef.cfgdat"
};

#define SAMPLING_FREQUENCY (algosDefcfgdat[0])
#define ALGO_OPERATION_BUFFER_SIZE (algosDefcfgdat[1])

#define MAX_NUMBER_OF_PARAMS (1)
#define MAX_PARAMETER_VAL 9

//Computation constants 
#define PI (3.14159)
#define MIN_3_DB (0.707)

//Library types
typedef _Q15 sample_t;           //Signal passes, normal range: 
typedef _Q16 d_sample_t;         //Signal passes, dual range
typedef _Q15 coef_t;   		     //Any coefficients
typedef _Q16 d_coef_t;   		 //Any coefficients

typedef float floatValue_t;		 //Normal range: 0..1.0, see AlgosChecks.h
typedef int intValue_t;
typedef float floatValueDB_t;
typedef int intValueDB_t;
typedef float floatFrequency_t;
typedef int intFrequency_t;
typedef int intMilliseconds_t;
typedef unsigned int bpm_t;
typedef unsigned int percernts_t;

typedef unsigned int numberOfAudioChannels_t;

typedef float* pBuffer_t;
typedef unsigned int* pCounter_t;
typedef unsigned int counter_t;
typedef unsigned int bufLength_t;
typedef unsigned int addressGap_t;

//Initialization modes
typedef enum {DEFAULT_INIT, CUSTOM_INIT} initialization_t;

//This enum uses for paramsUnion_t manipulations, see AlgosParams.h
typedef enum{
	UNKNOWN_TYPE_PARAM,
	FLOAT_TYPE_PARAM,
	INT_TYPE_PARAM
} paramRepresentation_t;

typedef unsigned int paramNum_t; //Current parameter number
typedef unsigned int numberOfParams_t; //Can not be more than MAX_NUMBER_OF_PARAMS

//Algorithm description structure
typedef struct _algoInfo_t
{
	numberOfAudioChannels_t numberOfInputChannels;
	numberOfAudioChannels_t numberOfOutputChannels;
	numberOfParams_t numOfParams;
	
} algoInfo_t;

//!!! Move all this headers and sources quite to Algos Runner to separate folder
#include "Algos/Errors/AlgosErrors.h"
#include "Algos/Params/AlgosParams.h"
#include "Algos/Checks/AlgosChecks.h"



#endif //GLOBALMACRO_H

