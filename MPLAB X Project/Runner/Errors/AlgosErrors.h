/**********************************************************************
* FileName:        ErrorsDef.h
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

#ifndef ERROR_H
#define ERROR_H

//Errors list
typedef enum
{
	ERROR_OK, //No errors, all okay
	NULL_PTR_ERROR,
	MAX_BUF_SIZE_EXCEEDING_ERROR,
	//Initialization
	ALGORITHM_INITIALIZATION_ERROR,
	//Range checking
	CHECK_RANGE_ERROR,
	INPUT_CLIPPING,
	NODE_CLIPPING,
	OUTPUT_CLIPPING,
	//Algorithm control
	ALGORITHM_SETUP_ERROR,
	UNEXPECTED_ALGORITHM_NUMBER,
	PARAM_STRUCTURE_ERROR,
	PARAMS_SET_ERROR,
	PARAMS_GET_ERROR,
	UNEXPECTED_PARAM_VALUE,
	UNEXPECTED_PARAM_NUMBER,

	OTHER_ERROR = 0xFFFF
} error_t;

#ifdef __cplusplus
extern "C" {
#endif
char* ReturnErrorString(error_t errVal);
#ifdef __cplusplus
}
#endif

#endif // ERROR_H

