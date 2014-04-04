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
#include "AlgosDef.h"

char* ReturnErrorString(error_t errVal)
{
	char *ptr = NULL;
	
	switch(errVal)
	{
		case ERROR_OK:
			ptr = "No error";
			break;
		case NULL_PTR_ERROR:
			ptr = "NULL pointer error";
			break;
		case MAX_BUF_SIZE_EXCEEDING_ERROR:
			ptr = "Buffer size exceeding";
			break;
		case ALGORITHM_INITIALIZATION_ERROR:
			ptr = "Algorithm initialization error";
			break;
		case CHECK_RANGE_ERROR:
			ptr = "Variable out of range";
			break;
		case INPUT_CLIPPING:
			ptr = "Input clipping";
			break;
		case NODE_CLIPPING:
			ptr = "Node clipping";
			break;
		case OUTPUT_CLIPPING:
			ptr = "Output clipping";
			break;

		case ALGORITHM_SETUP_ERROR:
			ptr = "Can not set Algo";
			break;
		case UNEXPECTED_ALGORITHM_NUMBER:
			ptr = "Unexpected algorithm number";
			break;
		case PARAM_STRUCTURE_ERROR:
			ptr = "Param structure corrupted";
			break;
		case PARAMS_SET_ERROR:
			ptr = "Param set error";
			break;
		case PARAMS_GET_ERROR:
			ptr = "Param get error";
			break;
		case UNEXPECTED_PARAM_VALUE:
			ptr = "Unexpected param value";
			break;
		case UNEXPECTED_PARAM_NUMBER:
			ptr = "Unexpected param number";
			break;

		case OTHER_ERROR:
			ptr = "Not described error";
			break;
		default:
			ptr = "Not error index";
			break;
	}
	
	return (ptr);
}
	
