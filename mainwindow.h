#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtNetwork>
#include <sched.h>
#include "can.h"
#include "canthread.h"
#include "log.h"
#include "framelog.h"
#include "xmlconf.h"
#include "modbusserver.h"
#include "mobus_conf.h"




class MainClass : public QObject
{
    Q_OBJECT
    
public:
    MainClass(QObject *parent = 0);
    ~MainClass();

    MeasurData measurData;

    Can *can, *can1;
    CanThread *canThread, *canThread1;
    Log *log;
    FrameLog *frameLog;
    ModBusServer *mbServer;
    QThread *mbThread;

private:
    XmlConf *confHandler;
    QString fullConfig;
    QVector <Reg_t> HoldingsRegs;
    QVector <Reg_t> InputsRegs;
    QMutex dataMutex;
    QMutex sendMutex;
    QMutex sendCANMutex;

    QMap <unsigned long, data_t> mDataforModBus;


    QTcpServer *confServer;
    QTcpSocket *confSocket;

    int mvv_errors;



public slots:
    //void initData();
    void proceedData();
    void checkCommand();
    void kvitCommand();
    void kvitChCommand(int channel);
    void disableChCommand(int channel);

    void confConnection();

    void calcPressure(int i);
    void calcTempK(int i);
    void calcTempB(int i);
    void calcTR(int i);
    void calcOffset(int i);
    void proceedStatus(int i);
    void proceedGroup();
    void pressureDiffToZero();
};

#endif // MAINWINDOW_H
