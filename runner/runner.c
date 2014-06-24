#include "../global_def.h"
#include "runner.h"

error_t runner_add_effect(runner_t* rr, void* data,
        error_t (*set_params)(void*, unsigned int, unsigned int),
        error_t (*process)(void*, p_buffer_t, p_buffer_t))
{
    if(rr->number_of_initialized_effects >= MAX_NUMBER_OF_ALGOS)
        return NUMBER_OF_INITIALIZED_EFFECTS_OUT_OF_LIMIT;
    rr->algos_array[rr->number_of_initialized_effects].data = data;
    rr->algos_array[rr->number_of_initialized_effects].p_set_params = set_params;
    rr->algos_array[rr->number_of_initialized_effects].p_process = process;
    rr->number_of_initialized_effects++;

    return ERROR_OK;
}

error_t runner_set_effect_num(runner_t* runner, unsigned int num)
{
    if(num < runner->number_of_initialized_effects)
    {
        runner->current_num = num;
        return ERROR_OK;
    }
    else
    {
        runner->error_state = UNEXPECTED_ALGORITHM_NUMBER;
        return runner->error_state;
    }
}

error_t runner_set_param(runner_t* runner, unsigned int num, unsigned int param)
{
    void* effect_data = runner->algos_array[runner->current_num].data;
    runner->error_state = runner->algos_array[runner->current_num].p_set_params(effect_data, num, param);
    return runner->error_state;
}

error_t runner_sbs_process(runner_t* runner, p_buffer_t in, p_buffer_t out)
{
    void* effect_data = runner->algos_array[runner->current_num].data;
    runner->error_state = runner->algos_array[runner->current_num].p_process(effect_data, in, out);
    return runner->error_state;
}

error_t runner_get_state(runner_t* runner)
{
    return runner->error_state;
}
