

void check_for_errors(bool (* areThereErrors)(), int flagError)
{
    if (areThereErrors())
    {
        // seta flag de thread da main task com a flag flagError
        // seta flag de estado com a flag flagError
        osThreadFlagsSet(id_da_main_task, flagError);
        osEventFlagsSet(ECU_control_event_id, flagError);
    }
    else
    {
        osEventFlagsClear(ECU_control_event_id, flagError);
        // limpa flag de estado flagError
    }
}


