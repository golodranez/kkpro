#include "modbusserver.h"
#include "mobus_conf.h"
#include "unistd.h"

ModBusServer::ModBusServer(QVector<Reg_t> &Input_reg_in, QVector<Reg_t> &Holdings_reg_in, QObject *parent) :
    QTcpServer(parent)
{

    QTimer *clearTimer = new QTimer;
    connect(clearTimer, SIGNAL(timeout()), this, SLOT(clearMBPool()));
    clearTimer->start(1000);


    log = new Log("/opt/sntermo/tcp.log");

    for(int i = 0; i < MAX_CONNECTIONS; i++)
    {
        socketPool[i].mbSlave = new ModBusSlaveDevice(i, Input_reg_in, Holdings_reg_in);
        socketPool[i].mbSlave->status = SLAVE_FREE;
        connect(socketPool[i].mbSlave, SIGNAL(logReady(QString)), log, SLOT(writeLog(QString)));
    }
    active_connections = 0;

    this->setMaxPendingConnections(MAX_CONNECTIONS-1);

    if(this->listen(QHostAddress::Any, 502) == 0)
    {
        qDebug() << "listen error" << this->errorString();
         server_status = 0;
    }
    else
    {
        qDebug() << "listen" ;
        server_status = 1;
    }
}



void ModBusServer::setMutex(QMutex *mutex_in)
{
    for(int i = 0; i < MAX_CONNECTIONS; i++)
    {
         socketPool[i].mbSlave->setMutex(mutex_in);
    }
}

void ModBusServer::incomingConnection(int handle)
{
    //qDebug() << 1;
    //usleep(10000);
    if(active_connections < MAX_CONNECTIONS)
    {
        connectMutex.lock();
        for(int i = 0; i < MAX_CONNECTIONS; i++)
        {
            if(socketPool[i].mbSlave->status == SLAVE_FREE)
            {
          //      qDebug() << 2;
                socketPool[i].mbSlave->setSocketDescriptor(handle, i);
                socketPool[i].mbSlave->status = SLAVE_STARTED;
                active_connections++;
                connect(socketPool[i].mbSlave, SIGNAL(disconnectSignal(int)), this, SLOT(dismisClient(int)));
                socketPool[i].mbSlave->start();

        //        qDebug() << i << " new Connection\t " <<  active_connections;
                //usleep(500000);
                break;
            }
        }
        connectMutex.unlock();

    }
    else
    {

      //  qDebug() << 3;
        usleep(500000);
       /* QTcpSocket tmpS;
        tmpS.setSocketDescriptor(handle);
        tmpS.disconnectFromHost();
        tmpS.close();*/
        //qDebug() << "HUI";

    }
}


void ModBusServer::dismisClient(int i)
{
    qDebug() << 4;
    connectMutex.lock();
    disconnect(socketPool[i].mbSlave, SIGNAL(disconnectSignal(int)), this, SLOT(dismisClient(int)));
    active_connections--;
  //  qDebug() << "state: " << socketPool[i].mbSlave->state();
  //  qDebug() << "ac - " << active_connections;
    connectMutex.unlock();
}

void ModBusServer::clearMBPool()
{
    //qDebug() << "CLEAR";
    connectMutex.lock();
   // for(int i = 0; i < MAX_CONNECTIONS; i++)
   // {
      /*  if((socketPool[i].mbSlave->status == SLAVE_FREE) && (socketPool[i].mbSlave->state() != NULL))
        {
            active_connections--;
            qDebug() << "clear connet";
        }*/
    //}

    int g = 0;
    for(int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if(socketPool[i].mbSlave->status == SLAVE_FREE)
        {
          //  qDebug() << "Free slave numb" << i;
            g++;
        }
    }
    active_connections = MAX_CONNECTIONS - g;
    //qDebug() << "AC - " << active_connections;
    connectMutex.unlock();
}
