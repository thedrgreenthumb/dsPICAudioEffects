#ifndef ALGOS_RUNNER_H_
#define ALGOS_RUNNER_H_

#include "../global_def.h"
#include "runner_errors.h"

#define MAX_NUMBER_OF_ALGOS (8)

typedef struct _algo_t
{
    void* data;
    //Pointers to effects functions without init()
    error_t (*p_set_params)(void* data, unsigned int num, unsigned int val);
    error_t (*p_process)(void* data, p_buffer_t in, p_buffer_t out);

} algo_t;

typedef struct _runner_t
{
    error_t error_state;
    sample_t* operation_buf;
    algo_t algos_array[MAX_NUMBER_OF_ALGOS];
    unsigned int current_num;
    unsigned int number_of_initialized_effects;

} runner_t;

error_t runner_add_effect(runner_t* rr, void* data,
        error_t (*set_params)(void*, unsigned int, unsigned int),
        error_t (*process)(void*, p_buffer_t, p_buffer_t));
error_t runner_set_effect_num(runner_t* runner, unsigned int num);
error_t runner_set_param(runner_t* runner, unsigned int num, unsigned int param);
error_t runner_sbs_process(runner_t* runner, p_buffer_t in, p_buffer_t out);
error_t runner_get_state();

#endif //ALGOS_RUNNER_H_

