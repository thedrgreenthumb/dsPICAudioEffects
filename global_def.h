/**********************************************************************
* FileName:        global_def.h
* FileVersion      1.01
* 
* Architecture:    _
* IDE:             _
* Compiller:       _
*
* License:         MIT
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

#define ALGOS_LIB_VERSION ("dsPIC:С30:0.01")

//Microchip fractional library
#include <libq.h>

#ifndef NULL
    #define NULL ((void*)0)
#endif

//Building modes
#define DEBUG_MODE                                                               //General debug mode
#define SIGNALS_CHECKING_MODE                                                    //Check clipping in signal passes
#define ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE                                 //Clean up Algo operation buf if Algo switched
#define IGNORE_NUMBER_OF_CLIPPED_SAMPLES (1000)                                  //Do not stop processing if samples are clipped

#define MAX_NUMBER_OF_PARAMS (1)
#define MAX_PARAMETER_VAL 9

//Computation constants 
#define PI (3.14159)
#define MIN_3_DB (0.707)

//Custom types
typedef _Q15 sample_t;           
typedef _Q16 d_sample_t;      
typedef _Q15 coef_t;   		  
typedef _Q16 d_coef_t;   		

typedef _Q15* p_buffer_t;
typedef unsigned int* p_counter_t;
typedef unsigned int counter_t;
typedef unsigned int bufLength_t;
typedef unsigned int addressGap_t;

#include "runner/runner_errors.h"

#endif //GLOBALMACRO_H

