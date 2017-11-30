#ifndef MODBUSSERVER_H
#define MODBUSSERVER_H

#include <QThread>
#include <QtNetwork>
#include "modbusslavedevice.h"
#include "log.h"

#define MAX_CONNECTIONS (10)

struct SocketPooll
{
    QTcpSocket *socket;
    QThread *thread;
    ModBusSlaveDevice *mbSlave;
};

class ModBusServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ModBusServer(QVector<Reg_t> &Input_reg_in, QVector<Reg_t> &Holdings_reg_in, QObject *parent = 0);

    void setMutex(QMutex *mutex_in);
    void incomingConnection(int handle);


signals:

public slots:
    void dismisClient(int i);
    void clearMBPool();

private:
    Log *log;
    SocketPooll socketPool[MAX_CONNECTIONS];
    int active_connections;
    int server_status;

    QMutex connectMutex;

};

#endif // MODBUSSERVER_H
