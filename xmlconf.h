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
    QDomDocument *confDoc, *calibrationDoc, *modbusDoc;
    QFile *confFile, *calibrationFile, *modbusConfFile;



    explicit XmlConf(QString &fullConfig_in, QString &fullcalibration_in, QString &fullModbus_in, QObject *parent = 0);
    int setConfFile(QString confFile_in);
    int setCalibrationFile(QString confFile_in);
    int parseConfig(MeasurData *measureData_in);
    int recordCalibrations(MeasurData *measureData_in);

    QString &fullConfig, &fullcalibration, &fullModbus;

signals:
    void signalLog(QString);

public slots:

};

#endif // XMLCONF_H
