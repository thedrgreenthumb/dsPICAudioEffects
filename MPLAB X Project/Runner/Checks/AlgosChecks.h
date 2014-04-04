#include "AlgosDef.h"

//Define signal thresholds
#define UPPER_SIGNAL_CHECKING_THRESHOLD (1.0)
#define LOWER_SIGNAL_CHECKING_THRESHOLD (-1.0)

#define UPPER_DEFAULT_FLOAT_VALUE_TYPE_THRESHOLD (1.0)
#define LOWER_DEFAULT_FLOAT_VALUE_TYPE_THRESHOLD (0)

error_t CheckRange(float minVal, float maxVal, float val);
error_t CheckArrayRange(float minVal, float maxVal, float *buf, bufLength_t length, unsigned int *elementNum);

error_t CheckParam(const param_t* param);
