#include "logthread.h"

LogThread::LogThread(Log *Log_in, QObject *parent) :
    QThread(parent)
{
    logH = Log_in;
}

void LogThread::run()
{
    //logH->w
}
