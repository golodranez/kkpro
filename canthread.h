#ifndef CANTHREAD_H
#define CANTHREAD_H

#include <QThread>
#include <can.h>
#include <sched.h>


class CanThread : public QThread
{
public:
    CanThread(Can *canObj_in);
    void run();

private:
    Can *canObj;
};

#endif // CANTHREAD_H
