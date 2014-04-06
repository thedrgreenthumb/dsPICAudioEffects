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
//The compilation for 128GP706 was needed
//#define CUSTOM_DSC_MODEL

#ifdef CUSTOM_DSC_MODEL
    #include <p33FJ128GP706.h>
#else
    #include <p33FJ256GP506.h>
#endif

#include <libq.h>

void InitLEDsAndBUTs(void);
void INTxIOInit(void);
void LM4811PinsInit(void);
void LM4811SetVolUP(int VolVal);
void LM4811SetVolDOWN(int VolVal);

int EffectsInit(_Q15* algorithms_buffer, unsigned int* sub_bufs_sizes, unsigned int number_of_gaps);
