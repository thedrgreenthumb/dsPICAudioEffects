#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "AlgosDef.h"

paramRepresentation_t GetParamType(paramValType_t type)
{
	switch(type)
	{
		case FLOAT_VALUE_TYPE:
		case FLOAT_VALUE_DB_TYPE:
		case FLOAT_VALUE_FREQ_TYPE:
			return FLOAT_TYPE_PARAM;

		case INT_VALUE_TYPE:
		case INT_VALUE_DB_TYPE:
		case INT_VALUE_FREQ_TYPE:
		case INT_MILLISECONDS_TYPE:
		case INT_BPM_TYPE:
		case PERCENTS_TYPE:
			return INT_TYPE_PARAM;

		case UNKNOWN_TYPE:
			return UNKNOWN_TYPE_PARAM;

		default:
			return UNKNOWN_TYPE_PARAM;
	}
}

error_t SetFloatParam(param_t* par, const char* paramName, paramValType_t type, float carVal, float maxVal, float minVal, float step)
{
	if(GetParamType(type) != FLOAT_TYPE_PARAM)
		return PARAM_STRUCTURE_ERROR;

	par->parType = type;
	par->curVal.fVal = carVal;
	par->maxVal.fVal = maxVal;
	par->minVal.fVal = minVal;
	par->stepVal.fVal = step;

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
	strcpy(par->paramName, paramName);
#endif

	return CheckParam(par);
}

error_t SetIntParam(param_t* par, const char* paramName, paramValType_t type, int carVal, int maxVal, int minVal, int step)
{
	if(GetParamType(type) != INT_TYPE_PARAM)
			return PARAM_STRUCTURE_ERROR;

		par->parType = type;
		par->curVal.iVal = carVal;
		par->maxVal.iVal = maxVal;
		par->minVal.iVal = minVal;
		par->stepVal.iVal = step;

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
	strcpy(par->paramName, paramName);
#endif

		return CheckParam(par);
}

error_t CopyParam(param_t* dst, const param_t* src)
{
	error_t err = ERROR_OK;

#ifdef DEBUG_MODE
	if(dst == NULL || src == NULL)
		return NULL_PTR_ERROR;

	err = CheckParam(src);
	if(err)
		return err;
#endif

	memcpy((void*)dst, (void*)src, sizeof(param_t));

	return err;
}

char* GetParamUnits(param_t* par)
{
	char *ptr = NULL;
	switch(par->parType)
	{
		case FLOAT_VALUE_TYPE:
			return ptr = "";
		case FLOAT_VALUE_DB_TYPE:
			return ptr = "dB";
		case FLOAT_VALUE_FREQ_TYPE:
			return ptr = "Hz";
		case INT_VALUE_TYPE:
			return ptr = "";
		case INT_VALUE_DB_TYPE:
			return ptr = "dB";
		case INT_VALUE_FREQ_TYPE:
			return ptr = "Hz";
		case INT_MILLISECONDS_TYPE:
			return ptr = "ms";
		case INT_BPM_TYPE:
			return ptr = "bpm";
		case PERCENTS_TYPE:
			return ptr = "%";
		case UNKNOWN_TYPE:
			return ptr = "unknown";
	}

	return NULL;
}
