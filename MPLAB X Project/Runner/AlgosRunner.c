#include <stdlib.h>
#include <string.h>

#include "AlgosDef.h"

#include "AlgosRunner.h"

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
error_t AlgosRunnerSetAlgoByName(algoRunner_t* aRunner, char* algoName)
{
	algoInfo_t currentInfo;
	unsigned int i = 0;
#ifdef ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE
	unsigned int j = 0;
#endif

	//Set Algo by name
	if(algoName != NULL)
	{
		for(i = 0; i < aRunner->numberOfInitilasedAlgos; i++)
		{
			aRunner->algosArray[i].pGetInfo(&currentInfo);
			if(!strcmp(algoName, currentInfo.algoName))
			{

#ifdef ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE
				if(aRunner->currentAlgoNum != i)
					for(j = 0; j < ALGO_OPERATION_BUFFER_SIZE; j++)
						aRunner->operationBuf[j] = 0;

				aRunner->currentAlgoNum = i;
#else
				aRunner->currentAlgoNum = i;
#endif			
				return ERROR_OK;
			}
		}
		aRunner->errorState = ALGORITHM_INITIALIZATION_ERROR;
		return aRunner->errorState;
	}
	return ERROR_OK;
}
#endif

error_t AlgosRunnerSetAlgoByNumber(algoRunner_t* aRunner, unsigned int algoNum)
{

#ifdef ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE
	unsigned int i = 0;
#endif

#ifdef DEBUG_MODE
	if(algoNum > aRunner->numberOfInitilasedAlgos)
	{
		aRunner->errorState = UNEXPECTED_ALGORITHM_NUMBER;
		return aRunner->errorState;
	}
#endif	

#ifdef ALGOS_RUNNER_CLEAN_OPERATION_BUFFER_MODE
	if(aRunner->currentAlgoNum != algoNum)
		for(i = 0; i < ALGO_OPERATION_BUFFER_SIZE; i++)
			aRunner->operationBuf[i] = 0;

	aRunner->currentAlgoNum = algoNum;
#else
	aRunner->currentAlgoNum = algoNum;
#endif

	return ERROR_OK;
}

error_t AlgosRunnerGetAlgoInfo(algoRunner_t* aRunner, algoInfo_t* algoInfo)
{
	aRunner->algosArray[aRunner->currentAlgoNum].pGetInfo(algoInfo);
	return ERROR_OK;
}

error_t AlgosRunnerGetParamDescription(algoRunner_t* aRunner, unsigned int paramNum,
		paramRepresentation_t* paramType, char* name, char* units, float* curVal, float* maxVal, float* minVal, float* step)
{
	error_t err = ERROR_OK;
	param_t curParam;

	err = aRunner->algosArray[aRunner->currentAlgoNum].pGetParams(aRunner->algosArray[aRunner->currentAlgoNum].algoStruct, paramNum, &curParam);

	*paramType = GetParamType(curParam.parType);

	strcpy(name, curParam.paramName);
	strcpy(units, GetParamUnits(&curParam));

	if(*paramType == FLOAT_TYPE_PARAM)
	{
		AlgosRunnerGetParam(aRunner, paramNum, curVal);
		*maxVal = curParam.maxVal.fVal;
		*minVal = curParam.minVal.fVal;
		*step = curParam.stepVal.fVal;
	}
	else if(*paramType == INT_TYPE_PARAM)
	{
		AlgosRunnerGetParam(aRunner, paramNum, curVal);
		*maxVal = curParam.maxVal.iVal;
		*minVal = curParam.minVal.iVal;
		*step = curParam.stepVal.iVal;
	}
	else
	{
		aRunner->errorState = err;
		return err;
	}
	return ERROR_OK;
}

error_t AlgosRunnerSetParam(algoRunner_t* aRunner, unsigned int paramNum, float fParam)
{
	return aRunner->algosArray[aRunner->currentAlgoNum].pSetParams(aRunner->algosArray[aRunner->currentAlgoNum].algoStruct, paramNum, fParam);
}

error_t AlgosRunnerGetParam(algoRunner_t* aRunner, unsigned int paramNum, float* fParam)
{
	error_t err = ERROR_OK;
	param_t curParam;
	paramRepresentation_t parRep = UNKNOWN_TYPE_PARAM;

	err = aRunner->algosArray[aRunner->currentAlgoNum].pGetParams(aRunner->algosArray[aRunner->currentAlgoNum].algoStruct, paramNum, &curParam);

	parRep = GetParamType(curParam.parType);

	if(parRep == FLOAT_TYPE_PARAM)
		*fParam = curParam.curVal.fVal;
	else if(parRep == INT_TYPE_PARAM)
		*fParam = curParam.curVal.iVal;
	else
	{
		aRunner->errorState = err;
		return err;
	}
	return ERROR_OK;
}

error_t AlgoSampleProcess(algoRunner_t* aRunner, pBuffer_t in, pBuffer_t out)
{
	aRunner->errorState = aRunner->algosArray[aRunner->currentAlgoNum].pProcess(aRunner->algosArray[aRunner->currentAlgoNum].algoStruct, in, out);
	return aRunner->errorState;
}

error_t AlgosRunnerBlockProcess(algoRunner_t* aRunner, unsigned int numberOfSamples, pBuffer_t* inputBuf, pBuffer_t* outputBuf)
{
#ifdef IGNORE_NUMBER_OF_CLIPPED_SAMPLES
	unsigned int sampleIgnoringCounter = 0;
#endif

	unsigned int i = 0;
	for(i = 0; i < numberOfSamples; i++)
	{
		aRunner->errorState = aRunner->algosArray[aRunner->currentAlgoNum].pProcess(aRunner->algosArray[aRunner->currentAlgoNum].algoStruct, inputBuf[i], outputBuf[i]);

#ifdef IGNORE_NUMBER_OF_CLIPPED_SAMPLES
		if(aRunner->errorState)
		{
			sampleIgnoringCounter++;
			if(sampleIgnoringCounter >= IGNORE_NUMBER_OF_CLIPPED_SAMPLES)
				break;
		}
		else
			sampleIgnoringCounter = 0;
#else
		if(aRunner->errorState)
			break;
#endif

	}
		
	return aRunner->errorState;
}

error_t AlgosRunnerGetCurrentState()
{		
	return aRunner->errorState;
}

char* AlgosRunnerGetCurrentStateAsString()
{		
	return ReturnErrorString(aRunner->errorState);
}

