#ifndef ALGOS_RUNNER_INIT_FUNCS_H_
#define ALGOS_RUNNER_INIT_FUNCS_H_

#include "AlgosRunner.h"
/*
error_t AlgosRunnerInit_Bypass(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_ColorationFilter01(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_ColorationFilter02(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_ColorationFilter15(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_ColorationFilter25(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_DF2SO5BandEQ01(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_DF28O10BandEQ01(algoRunner_t* aRunner, algo_t* algosStruct);


//error_t AlgosRunnerInit_GardnersSmallRoom05(algoRunner_t* aRunner, algo_t* algosStruct);
//error_t AlgosRunnerInit_GardnersMediumRoom05(algoRunner_t* aRunner, algo_t* algosStruct);
//error_t AlgosRunnerInit_GardnersLargeRoom05(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_DattorrosProgenitor01(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_Freeverb301(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_GardnersSmallRoom05(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_GardnersMediumRoom05(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_GardnersLargeRoom05(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_BassEnhanser01(algoRunner_t* aRunner, algo_t* algosStruct);



error_t AlgosRunnerInit_FDNWithDelayLines01(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_FDNWithAPFilters01(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_APDiffuser01(algoRunner_t* aRunner, algo_t* algosStruct);
error_t AlgosRunnerInit_APDiffuser15(algoRunner_t* aRunner, algo_t* algosStruct);

error_t AlgosRunnerInit_Schoeders2Based05(algoRunner_t* aRunner, algo_t* algosStruct);
*/


//Initialization functions macro definition
#define AlgosRunnerInit_(NAME) \
error_t  AlgosRunnerInit_##NAME (algoRunner_t* aRunner, algo_t* algosStruct) \
{ \
	error_t errorState = ERROR_OK; \
	NAME* STR = (NAME*)malloc(sizeof(NAME)); \
	errorState = NAME ## Init(STR, aRunner->operationBuf, DEFAULT_INIT); \
	if(errorState) return errorState; \
	algosStruct->algoStruct = (void*)STR; \
	algosStruct->pSetParams = STR->pSetParams; \
	algosStruct->pGetParams = STR->pGetParams; \
	algosStruct->pProcess = STR->pProcess; \
	algosStruct->pGetInfo = STR->pGetInfo; \
	return errorState; \
}

AlgosRunnerInit_(Bypass);
AlgosRunnerInit_(DattorrosProgenitor01);

AlgosRunnerInit_(Freeverb301);

AlgosRunnerInit_(ColorationFilter01);
AlgosRunnerInit_(ColorationFilter02);
AlgosRunnerInit_(ColorationFilter15);
AlgosRunnerInit_(ColorationFilter25);

AlgosRunnerInit_(DF2SO5BandEQ01);
AlgosRunnerInit_(DF28O10BandEQ01);

AlgosRunnerInit_(GardnersSmallRoom05);
AlgosRunnerInit_(GardnersMediumRoom05);
AlgosRunnerInit_(GardnersLargeRoom05);

AlgosRunnerInit_(Schoeders2Based05);

AlgosRunnerInit_(FDNWithDelayLines01);
AlgosRunnerInit_(FDNWithAPFilters01);

AlgosRunnerInit_(APDiffuser01);
AlgosRunnerInit_(APDiffuser15);

AlgosRunnerInit_(BassEnhanser01);


#endif //ALGOS_RUNNER_INIT_FUNCS_H_
