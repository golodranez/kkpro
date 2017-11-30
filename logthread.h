#ifndef LOGTHREAD_H
#define LOGTHREAD_H

#include <QThread>
#include <log.h>

class LogThread : public QThread
{
    Q_OBJECT
public:
    explicit LogThread(Log *Log_in, QObject *parent = 0);
    void run();
private:
    Log *logH;

signals:
    
public slots:
    
};

#endif // LOGTHREAD_H
