#ifndef FRAMELOG_H
#define FRAMELOG_H

#include <QObject>
#include <QFile>
#include <QTextStream>

class FrameLog : public QObject
{
    Q_OBJECT
public:
    explicit FrameLog(QObject *parent = 0);

private:
    QFile logFile;
    QTextStream logOut;

signals:

public slots:
    void writeLog(QString logMessage);

};

#endif // FRAMELOG_H
