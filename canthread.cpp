#include "canthread.h"

CanThread::CanThread(Can *canObj_in)
{
    canObj = canObj_in;
 //   setPriority(HighestPriority);
}

void CanThread::run()
{
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_RR);

    if(sched_setscheduler(0, SCHED_RR, &param) == -1)
    {
        perror("sched_setscheduler failed");
        exit(-1);
    }

    canObj->runRecvCicle();
    exec();
}

