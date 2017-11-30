#ifndef XMLCONF_H
#define XMLCONF_H

#include <QObject>
#include <QtCore>
#include <QDomDocument>
#include <QDomNodeList>
#include "measurdata.h"
#include "api_func.h"

class XmlConf : public QObject
{
    Q_OBJECT

public:
    QDomDocument *confDoc;
    QFile *confFile;



    explicit XmlConf(QString &fullConfig_in, QObject *parent = 0);
    int setConfFile(QString confFile_in);
    int parseConfig(MeasurData *measureData_in);

    QString &fullConfig;

signals:
    void signalLog(QString);

public slots:

};

#endif // XMLCONF_H
