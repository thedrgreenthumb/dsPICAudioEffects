/**********************************************************************
* FileName:        Initialization.h
* FileVersion      1.01
* Dependencies:
* Processor:       dsPIC33FJ256GP506
* Compiler:        C30 v3.21
*
* Design in:       SAL
* Design by:
* Feedback:
*
* Project:         dsPIC DSC Starter Kit 1 Audio Effects Project.
* ProjectVersion:  1.01
**********************************************************************/
#include <p33FJ256GP506.h>

void InitLEDsAndBUTs(void);
void INTxIOInit(void);
void LM4811SetVolUP(int VolVal);
void LM4811SetVolDOWN(int VolVal);
