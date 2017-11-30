#include "log.h"


Log::Log(QString logfilename, QObject *parent) :
    QObject(parent), logName(logfilename)
{
    logFile.setFileName(logName);
    if( !logFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text) )
        return;
    logOut.setDevice(&logFile);
}

void Log::writeLog(QString logMessage)
{

    date_time = QDateTime::currentDateTime();
    time_str = QString("%1.%2 - %3:%4:%5.%6").
            arg(date_time.date().day()).
            arg(date_time.date().month()).
            arg(date_time.time().hour()).
            arg(date_time.time().minute()).
            arg(date_time.time().second()).
            arg(date_time.time().msec());

    logMessage.append(" " + time_str);

    logOut << logMessage << "\n";
    logOut.flush();


}
