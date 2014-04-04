#include "AlgosDef.h"

error_t CheckRange(float minVal, float maxVal, float val)
{
	if (minVal > val || val > maxVal)
		return CHECK_RANGE_ERROR;
	else
		return ERROR_OK;
}

error_t CheckArrayRange(float minVal, float maxVal, float *buf, bufLength_t length, unsigned int *elementNum)
{
	unsigned int i = 0;
	for(i = 0; i < length; i++)
		if (CheckRange(minVal, maxVal, buf[i]))
		{
			*elementNum = i;
			return CHECK_RANGE_ERROR;
		}
	
	return ERROR_OK;	
}

error_t CheckParam(const param_t* param)
{
	//Check float types
	if(param->parType == FLOAT_VALUE_TYPE)
		if(param->minVal.fVal > param->maxVal.fVal ||
				param->minVal.fVal > param->curVal.fVal ||
				param->curVal.fVal > param->maxVal.fVal ||
				param->stepVal.fVal > param->maxVal.fVal)
			return PARAM_STRUCTURE_ERROR;

	//Check integer types
	if(param->parType == INT_VALUE_TYPE)
		if(param->minVal.iVal > param->maxVal.iVal ||
				param->minVal.iVal > param->curVal.iVal ||
				param->curVal.iVal > param->maxVal.iVal ||
				param->stepVal.iVal > param->maxVal.iVal)
			return PARAM_STRUCTURE_ERROR;

	if(param->parType == UNKNOWN_TYPE)
		return PARAM_STRUCTURE_ERROR;

	return ERROR_OK;
}

