#ifndef MODBUSSLAVEDEVICE_H
#define MODBUSSLAVEDEVICE_H

#include <QObject>
#include <QtNetwork>

#include "mobus_conf.h"
#include <convertregisters.h>

#define READ_TIMEOUT (20000)

class ModBusSlaveDevice : public QThread
{
    Q_OBJECT
public:
    explicit ModBusSlaveDevice(int id, QVector<Reg_t> &Inputs_reg_in, QVector<Reg_t> &Holdings_reg_in, QObject *parent = 0);
    //int setSocket(QTcpSocket *tcpSocket_in);
    int setSocketDescriptor(int socket, int id_in);
    bool status;
    bool stopSlave_doneStatus;
    void setMBData(QVector<Reg_t> &Inputs_reg_in, QVector<Reg_t> &Holdings_reg_in);
    void setMutex(QMutex *mutex_in);
    void setHoldingMutex(QMutex *mutex_in);
    int state();


signals:
    void disconnectSignal(int);
    void logReady(QString);

public slots:
    void stopSlave();
    void run();

private:
    QTcpSocket *tcpSocket;
    int sock_desc;
    int id;
    QVector <Reg_t> &Inputs_reg;
    QVector <Reg_t> &Holdings_reg;
    QMutex *mutex;
    QString peerIP;

    void readHoldings(QByteArray &regInfo);
    void readInputsRegs(QByteArray &regInfo);
    void writeHoldings(QByteArray &regInfo);




};

#endif // MODBUSSLAVEDEVICE_H
