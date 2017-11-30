#include "modbusslavedevice.h"
#include "api_func.h"

ModBusSlaveDevice::ModBusSlaveDevice(int id_in, QVector<Reg_t> &Inputs_reg_in, QVector<Reg_t> &Holdings_reg_in, QObject *parent) :
    QThread(parent), Inputs_reg(Inputs_reg_in), Holdings_reg(Holdings_reg_in)
{
    id = id_in;
}

int ModBusSlaveDevice::setSocketDescriptor(int socket, int id_in)
{

    sock_desc = socket;
    id = id_in;

}


void ModBusSlaveDevice::run()
{
  /*  struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_RR);

    if(sched_setscheduler(0, SCHED_RR, &param) == -1)
    {
        perror("sched_setscheduler failed");
        exit(-1);
    }
    */

    tcpSocket = new QTcpSocket;
    tcpSocket->setSocketDescriptor(sock_desc);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(stopSlave()));
    //connect(tcpSocket, SIGNAL(disconnected()), this, SLLogOT(stopSlave()));
    peerIP = tcpSocket->peerAddress().toString();
    emit logReady("New Connection from " + peerIP + " ID - " + QString::number(id));

    stopSlave_doneStatus = false;



    while(tcpSocket->waitForReadyRead(READ_TIMEOUT))
    {
        QByteArray recvData = tcpSocket->readAll();

        char func_numb = recvData.at(7);
        switch(func_numb)
        {
            case(0x03): // read Holdings
            //    qDebug() << "holdings";
                readHoldings(recvData);
                break;

            case(0x04): // read Inputs Registers
                readInputsRegs(recvData);
           //     qDebug() << "input r";
                break;

            case(0x10): // write Holdings Registers
                writeHoldings(recvData);
                //qDebug() << "input w";
                break;

            default:
                emit logReady("Unknow modbus function " + QString::number(func_numb) + " from " + peerIP);
                break;

        }


        if(status == SLAVE_FREE)
        {
            break;
        }
    }
    //qDebug() << "timeout disconnect\n";
    //tcpSocket->disconnectFromHost();
    tcpSocket->close();
    delete tcpSocket;
    tcpSocket = NULL;
    //tcpSocket->disconnectFromHost();
    status = SLAVE_FREE;
  //  stopSlave();

}

void ModBusSlaveDevice::readHoldings(QByteArray &regInfo)
{
    char a[2];
    a[0] = regInfo.at(9);
    a[1] = regInfo.at(8);
    qint16 start_reg;
    memset(&start_reg, '\0', sizeof(qint16));
    memcpy(&start_reg, a, 2);

    a[0] = regInfo.at(11);
    a[1] = regInfo.at(10);
    qint16 amount_reg;
    memset(&amount_reg, '\0', sizeof(qint16));
    memcpy(&amount_reg, a, 2);

//    qDebug() << start_reg << " " << amount_reg;


//    qDebug() << "send regs";
    QByteArray toSend;
    toSend.append(regInfo.at(0)); // transaction
    toSend.append(regInfo.at(1));
    toSend.append(regInfo.at(2)); // protocol
    toSend.append(regInfo.at(3));

    quint16 recvsize = amount_reg * 2 + 3;
    QByteArray tmp;
    tmp.append((char *)&recvsize, sizeof(quint16));
    toSend.append(tmp.at(1));
    toSend.append(tmp.at(0));
    tmp.clear();

    toSend.append(1);
    toSend.append(0x03);
    toSend.append(amount_reg * 2);

    mutex->lock();
    int max_reg_for_read;
    if(amount_reg+start_reg > Holdings_reg.size())
    {
       max_reg_for_read = Holdings_reg.size();
    }else
    {
        max_reg_for_read = amount_reg+start_reg;
    }

    for(int i = start_reg; i < max_reg_for_read; i++)
    {
      //  qDebug() << "start " << i;
        toSend.append(Holdings_reg[i].reg.at(0));
        toSend.append(Holdings_reg[i].reg.at(1));
    }
    mutex->unlock();
//    qDebug() << "before send";

    int i = tcpSocket->write(toSend);
//    qDebug() << "write " << i;



}

void ModBusSlaveDevice::readInputsRegs(QByteArray &regInfo)
{
    char a[2];
    a[0] = regInfo.at(9);
    a[1] = regInfo.at(8);
    qint16 start_reg;
    memset(&start_reg, '\0', sizeof(qint16));
    memcpy(&start_reg, a, 2);

    a[0] = regInfo.at(11);
    a[1] = regInfo.at(10);
    qint16 amount_reg;
    memset(&amount_reg, '\0', sizeof(qint16));
    memcpy(&amount_reg, a, 2);

    //qDebug() << start_reg << " " << amount_reg;


  //  qDebug() << "send regs";
    QByteArray toSend;
    toSend.append(regInfo.at(0)); // transaction
    toSend.append(regInfo.at(1));
    toSend.append(regInfo.at(2)); // protocol
    toSend.append(regInfo.at(3));

    quint16 recvsize = amount_reg * 2 + 3;
    QByteArray tmp;
    tmp.append((char *)&recvsize, sizeof(quint16));
    toSend.append(tmp.at(1));
    toSend.append(tmp.at(0));
    tmp.clear();

    toSend.append(1);
    toSend.append(0x04);
    toSend.append(amount_reg * 2);

    mutex->lock();
    int max_reg_for_read;
    if(amount_reg+start_reg > Inputs_reg.size())
    {
       max_reg_for_read = Inputs_reg.size();
    }else
    {
        max_reg_for_read = amount_reg+start_reg;
    }

    for(int i = start_reg; i < max_reg_for_read; i++)
    {
      //  qDebug() << "start " << i;
        toSend.append(Inputs_reg[i].reg.at(0));
        toSend.append(Inputs_reg[i].reg.at(1));
    }
    mutex->unlock();
    //qDebug() << "before send";

    int i = tcpSocket->write(toSend);
    //qDebug() << i;
}

void ModBusSlaveDevice::writeHoldings(QByteArray &regInfo)
{
    char a[2];
    a[0] = regInfo.at(9);
    a[1] = regInfo.at(8);
    qint16 start_reg;
    memset(&start_reg, '\0', sizeof(qint16));
    memcpy(&start_reg, a, 2);

    a[0] = regInfo.at(11);
    a[1] = regInfo.at(10);
    qint16 amount_reg;
    memset(&amount_reg, '\0', sizeof(qint16));
    memcpy(&amount_reg, a, 2);

//    qDebug() << "START & AMOUNT - "  << start_reg << " " << amount_reg;
    int reg_bytes = regInfo.at(12);
    mutex->lock();
    int max_reg_for_read;
    if(amount_reg+start_reg > Holdings_reg.size())
    {
//	qDebug() << "ERROR reg amount";
       max_reg_for_read = Holdings_reg.size();
    }else
    {
        max_reg_for_read = amount_reg+start_reg;
    }

    int g = 0;
//    qDebug() << "START & MAX_REG - "  << start_reg << " " << max_reg_for_read;
    for(int i = start_reg; i < max_reg_for_read; i++)
    {
     //   qDebug() << "start " << i;
        Holdings_reg[i].reg[0] = regInfo[g + 13];
        Holdings_reg[i].reg[1] = regInfo[g + 13 + 1];

       // qDebug() << "reg 0" << ConvertRegisters::toShort(Holdings_reg, 0);
       // qDebug() << "reg 1" << ConvertRegisters::toShort(Holdings_reg, 1);
        //Holdings_reg[i].data()->reg[0] = regInfo[g + 13];
        //Holdings_reg[i].data()->reg[1] = regInfo[g + 13 + 1];
        g+=2;
      //  toSend.append(Inputs_reg->at(i).reg.at(0));
      //  toSend.append(Inputs_reg->at(i).reg.at(1));
    }
    mutex->unlock();


  //  qDebug() << "send regs";
    QByteArray toSend;
    toSend.append(regInfo.at(0)); // transaction
    toSend.append(regInfo.at(1));
    toSend.append(regInfo.at(2)); // protocol
    toSend.append(regInfo.at(3));

    quint16 recvsize = 6; //amount_reg * 2 + 2;
//    qDebug() << "size - "  << recvsize;
    
    QByteArray tmp;
    tmp.append((char *)&recvsize, sizeof(quint16));
    toSend.append(tmp.at(1));
    toSend.append(tmp.at(0));
    tmp.clear();

    toSend.append(1);
    toSend.append(0x10); // func num
    tmp.clear();
    tmp.append((char *)&start_reg, sizeof(quint16));
    toSend.append(tmp.at(1));
    toSend.append(tmp.at(0));
    tmp.clear();
    tmp.append((char *)&amount_reg, sizeof(quint16));
    toSend.append(tmp.at(1));
    toSend.append(tmp.at(0));
    //qDebug() << "before send";

//    qDebug() << "START & AMOUNT before send - "  << start_reg << " " << amount_reg;

    int i = tcpSocket->write(toSend);
}


void ModBusSlaveDevice::setMBData(QVector<Reg_t> &Inputs_reg_in, QVector<Reg_t> &Holdings_reg_in)
{
    Inputs_reg = Inputs_reg_in;
    Holdings_reg = Holdings_reg_in;
}

void ModBusSlaveDevice::setMutex(QMutex *mutex_in)
{
    mutex = mutex_in;
}

void ModBusSlaveDevice::setHoldingMutex(QMutex *mutex_in)
{

}

int ModBusSlaveDevice::state()
{
    if(tcpSocket == NULL)
            return NULL;
    else
        return 9;
}

void ModBusSlaveDevice::stopSlave()
{
    if(stopSlave_doneStatus != true)
    {
        emit logReady("Diconnect from " + peerIP  + " ID - " + QString::number(id));
        stopSlave_doneStatus = true;
      //  qDebug() << "1stopSlave()";
        status = SLAVE_FREE;

        this->usleep(10000);
        emit disconnectSignal(id);
//    delete tcpSocket;
        this->quit();
       // tcpSocket->deleteLater();
    }
}
