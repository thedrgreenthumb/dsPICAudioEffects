#ifndef ALGOS_PARAMS_H
#define ALGOS_PARAMS_H

#include "AlgosDef.h"

//Control parameter representation
typedef union _paramsUnion_t
{
	float fVal;
	int iVal;
} paramsUnion_t;

typedef enum _paramValType_t{
	UNKNOWN_TYPE,
	FLOAT_VALUE_TYPE,
	INT_VALUE_TYPE,
	FLOAT_VALUE_DB_TYPE,
	INT_VALUE_DB_TYPE,
	FLOAT_VALUE_FREQ_TYPE,
	INT_VALUE_FREQ_TYPE,
	INT_MILLISECONDS_TYPE,
	INT_BPM_TYPE,
	PERCENTS_TYPE
} paramValType_t;

#define ALGOS_PARAM_NAME_LENGHT (64)
#define ALGOS_PARAM_UNITS_LENGHT (16)
#define ALGOS_PARAM_VALUES_LENGHT (8)

typedef struct _param_t
{
	paramValType_t parType;

	paramsUnion_t curVal;
	paramsUnion_t maxVal;
	paramsUnion_t minVal;
	paramsUnion_t stepVal;

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
	char paramName[ALGOS_PARAM_NAME_LENGHT];
#endif

} param_t;

#define DEFAULT_FLOAT_PARAMS_STEP (0.01)
#define DEFAULT_INT_PARAMS_STEP (5)

error_t SetFloatParam(param_t* par, const char* paramName, paramValType_t type, float carVal, float maxVal, float minVal, float step);
error_t SetIntParam(param_t* par, const char* paramName, paramValType_t type, int carVal, int maxVal, int minVal, int step);

error_t CopyParam(param_t* dst, const param_t* src);

paramRepresentation_t GetParamType(paramValType_t type);
char* GetParamUnits(param_t* par);

#endif //ALGOS_PARAMS_H
