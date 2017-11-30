#ifndef CAN_H
#define CAN_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include "can_200api.h"
//#include "can_iei/commdef.h"
#include "can_iei/pci_1680.h"

#define EOK 0

#include <QObject>
#include "measurdata.h"
#include "api_func.h"


//#define ELCUS_200PCI  // select CAN-card type
#define DRPC_100


#define CAN_SPEED 1000

class Can : public QObject
{
    Q_OBJECT

private:
    int device; /* Board id_num */
    int speed;

    int hCan; /* Board descriptor */
    int i; /* cicle counter */
    char name[256]; /* Board name*/
    struct CAN_info_t conf; /* CAN board info */


    QString logMess;

    MeasurData *mData;
    QMutex *mutex;





public:
    explicit Can(int dev, MeasurData *mData_in, QObject *parent = 0);
    int init(int can_mode);
    int runRecvCicle();

    int Read(CAN_data &data);
    int Read_DRPC(CAN_PACKET &data);
    int Write(const  CAN_data &data);
    int Write_DRPC(CAN_PACKET &data);
    void setMutex(QMutex *mutex_in);
    
signals:
    void logReady(QString logMessage);
    void frameLogReady(QString logMessage);
    
public slots:
    void txCicle();
    
};

#endif // CAN_H
