#ifndef ALGOS_RUNNER_H_
#define ALGOS_RUNNER_H_

#include "AlgosDef.h"
#include "Algos/Errors/AlgosErrors.h"
#include "Algos/Params/AlgosParams.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUMBER_OF_ALGOS (32)

#define RUNNER_INIT_NOT_REQUIRED (0xFFFFFFFF)

typedef struct _algo_t
{
	void *algoStruct;
	
	//Pointers to Algo functions without init
	error_t (*pSetParams)(void* str, paramNum_t paramNumber, float val);
	error_t (*pSetParams)(void* str, paramNum_t paramNumber, int val);
	error_t	(*pGetParams)(void* str, paramNum_t paramNumber, param_t* val);
	error_t	(*p_process)(void* str, pBuffer_t in, pBuffer_t out);
	void (*pGetInfo)(algoInfo_t* algoInfo);

} algo_t;

typedef struct _algoRunner_t
{
	error_t errorState;
	sample_t* operationBuf;
	algo_t algosArray[MAX_NUMBER_OF_ALGOS];
	unsigned int currentAlgoNum;
	unsigned int numberOfInitilasedAlgos;

} algoRunner_t;

error_t AlgosRunnerInit(algoRunner_t* aRunner, char** algosNames, unsigned int numberOfAlgos, sample_t* operationBuf);
error_t AlgosRunnerDeInit(algoRunner_t* aRunner);

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
error_t AlgosRunnerSetAlgoByName(algoRunner_t* aRunner, char* algoName);
#endif

error_t AlgosRunnerSetAlgoByNumber(algoRunner_t* aRunner, unsigned int algoNum);

#ifdef ALGOS_DESCRIPTION_SUPPORT_MODE
error_t AlgosRunnerGetAlgoInfo(algoRunner_t* aRunner, algoInfo_t* algoInfo);
#endif


error_t AlgosRunnerGetParamDescription(algoRunner_t* aRunner, unsigned int paramNum,
		paramRepresentation_t* paramType, char* name, char* units, float* curVal, float* maxVal, float* minVal, float* step);
error_t AlgosRunnerSetParam(algoRunner_t* aRunner, unsigned int paramNum, float fParam);
error_t AlgosRunnerSetParam(algoRunner_t* aRunner, unsigned int paramNum, unsigned int iParam);
error_t AlgosRunnerGetParam(algoRunner_t* aRunner, unsigned int paramNum, float* fParam);

error_t AlgoSampleProcess(algoRunner_t* aRunner, pBuffer_t in, pBuffer_t out);
error_t AlgosRunnerBlockProcess(algoRunner_t* aRunner, unsigned int numberOfSamples, pBuffer_t* inputBuf, pBuffer_t* outputBuf);

error_t AlgosRunnerGetCurrentState();
char* AlgosRunnerGetCurrentStateAsString();

#ifdef __cplusplus
}
#endif

#endif //ALGOS_RUNNER_H_

