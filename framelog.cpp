#include "framelog.h"
#include <QDateTime>

FrameLog::FrameLog(QObject *parent) :
    QObject(parent)
{
}


void FrameLog::writeLog(QString logMessage)
{
    logFile.setFileName("/opt/sntermo/framelog.txt");
    if( !logFile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text) )
        return;
    logOut.setDevice(&logFile);
 //   QDateTime date_time = QDateTime::currentDateTime();
  /*  QString time_str = QString("%1.%2 - %3:%4:%5.%6").
            arg(date_time.date().day()).
            arg(date_time.date().month()).
            arg(date_time.time().hour()).
            arg(date_time.time().minute()).
            arg(date_time.time().second()).
            arg(date_time.time().msec());*/

    //logMessage.append(" ");

    logOut << logMessage << "\n";
    logFile.close();
}
