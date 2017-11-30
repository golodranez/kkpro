#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QString logfilename, QObject *parent = 0);
    
private:
    QFile logFile;
    QTextStream logOut;
    QString logName;
    QDateTime date_time;
    QString time_str;

signals:
    
public slots:
    void writeLog(QString logMessage);


    
};

#endif // LOG_H
