#include <stdlib.h>
#include <string.h>

#include "Algos/Errors/AlgosErrors.h"
#include "AlgosRunner/AlgosRunner.h"
#include "AlgosRunner/AlgosRunnerInitFuncs.h"

//!!Name like a string
#define AlgosRunnerAdd_(NAME) \
if(!strcmp("#NAME#", algosNames[i])) \
	if(!AlgosRunnerInit_##NAME(aRunner, &aRunner->algosArray[i])) \
		successfullyAddedAlgos++;



error_t AlgosRunnerInit(algoRunner_t* aRunner, char** algosNames, unsigned int numberOfAlgos, sample_t* operationBuf)
{
	unsigned int successfullyAddedAlgos = 0;
	unsigned int i = 0;

	//Set operation buffer
	if(operationBuf == NULL)
		//Operation buffer for algorithms
		aRunner->operationBuf = (sample_t*)malloc(ALGO_OPERATION_BUFFER_SIZE*sizeof(sample_t));
	else
		aRunner->operationBuf = operationBuf;
	
	//Init it
	for(i = 0; i < ALGO_OPERATION_BUFFER_SIZE; i++)
		aRunner->operationBuf[i] = 0;

	//Initialize current algo number
	aRunner->currentAlgoNum = 0;

	for(i = 0; i < numberOfAlgos; i++)
	{
//Bypass
/*
		if(!strcmp("Bypass", algosNames[i]))
			if(!AlgosRunnerInit_Bypass(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;
*/

AlgosRunnerAdd_("Bypass");

//Reverberators
		//Dattorros
		if(!strcmp("DattorrosProgenitor01", algosNames[i]))
			if(!AlgosRunnerInit_DattorrosProgenitor01(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("ColorationFilter01", algosNames[i]))
			if(!AlgosRunnerInit_ColorationFilter01(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("ColorationFilter02", algosNames[i]))
			if(!AlgosRunnerInit_ColorationFilter02(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("ColorationFilter15", algosNames[i]))
			if(!AlgosRunnerInit_ColorationFilter15(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("ColorationFilter25", algosNames[i]))
			if(!AlgosRunnerInit_ColorationFilter25(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("DF2SO5BandEQ01", algosNames[i]))
			if(!AlgosRunnerInit_DF2SO5BandEQ01(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("DF28O10BandEQ01", algosNames[i]))
			if(!AlgosRunnerInit_DF28O10BandEQ01(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("GardnersSmallRoom05", algosNames[i]))
			if(!AlgosRunnerInit_GardnersSmallRoom05(aRunner, &aRunner->algosArray[i]))
				successfullyAddedAlgos++;

		if(!strcmp("GardnersMediumRoom05", algosNames[i]))
			if(!AlgosRunnerInit_GardnersMediumRoom05(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

		if(!strcmp("GardnersLargeRoom05", algosNames[i]))
			if(!AlgosRunnerInit_GardnersLargeRoom05(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;


		//Freeverb3
		if(!strcmp("Freeverb301", algosNames[i]))
			if(!AlgosRunnerInit_Freeverb301(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;




		if(!strcmp("FDNWithDelayLines01", algosNames[i]))
			if(!AlgosRunnerInit_FDNWithDelayLines01(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

		if(!strcmp("FDNWithAPFilters01", algosNames[i]))
			if(!AlgosRunnerInit_FDNWithAPFilters01(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

	/*
		if(!strcmp("FDNWithAPFilters02", algosNames[i]))
			if(!AlgosRunnerInit_FDNWithAPFilters02(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

		if(!strcmp("FDNWithAPFilters03", algosNames[i]))
			if(!AlgosRunnerInit_FDNWithAPFilters03(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;
					*/

		if(!strcmp("APDiffuser01", algosNames[i]))
			if(!AlgosRunnerInit_APDiffuser01(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;
		/*
		if(!strcmp("APDiffuser15", algosNames[i]))
			if(!AlgosRunnerInit_APDiffuser15(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

		if(!strcmp("APDiffuser25", algosNames[i]))
			if(!AlgosRunnerInit_APDiffuser25(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;
				*/
		if(!strcmp("Schoeders2Based05", algosNames[i]))
			if(!AlgosRunnerInit_Schoeders2Based05(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

		if(!strcmp("BassEnhanser01", algosNames[i]))
			if(!AlgosRunnerInit_BassEnhanser01(aRunner, &aRunner->algosArray[i]))
					successfullyAddedAlgos++;

	}

	if(numberOfAlgos != successfullyAddedAlgos)
		return ALGORITHM_INITIALIZATION_ERROR;
	else 
	{
		aRunner->numberOfInitilasedAlgos = successfullyAddedAlgos;
		return ERROR_OK;
	}
}

error_t AlgosRunnerDeInit(algoRunner_t* aRunner)
{
	unsigned int i = 0;
	for(i = 0; i < aRunner->numberOfInitilasedAlgos; i++)
		free((void*)aRunner->algosArray[i].algoStruct);

	free(aRunner->operationBuf);

	return ERROR_OK;
}

