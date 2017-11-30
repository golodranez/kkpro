#include <QDebug>
#include "convertregisters.h"
//#include "modbusmasterdevice.h"
//#include <arpa/inet.h>



ConvertRegisters::ConvertRegisters(QObject *parent) :
    QObject(parent)
{
}

qint16 ConvertRegisters::toShort(QVector <Reg_t> &regs, int target_reg)
{
    char a[2];
    a[0] = regs[target_reg].reg.at(1);
    a[1] = regs[target_reg].reg.at(0);

    qint16 tt;
    memset(&tt, '\0', sizeof(qint16));
    memcpy(&tt, a, 2);
    //qint16 result = (qint16)ntohs(tt);
    //qDebug() << result;
   // return result;
    return tt;
}

qint32 ConvertRegisters::toInt(QVector<Reg_t> &regs, int target_reg)
{
    char a[4];
    a[0] = regs[target_reg].reg.at(1);
    a[1] = regs[target_reg].reg.at(0);
    a[2] = regs[target_reg + 1].reg.at(1);
    a[3] = regs[target_reg + 1].reg.at(0);
    int result;
    memset(&result, '\0', sizeof(quint32));
    memcpy(&result, a, sizeof(quint32));
    //qDebug() << result;
    return result;

}

float ConvertRegisters::toFloat(QVector<Reg_t> &regs, int target_reg)
{
    char a[4];
    a[0] = regs[target_reg].reg.at(1);
    a[1] = regs[target_reg].reg.at(0);
    a[2] = regs[target_reg + 1].reg.at(1);
    a[3] = regs[target_reg + 1].reg.at(0);
    float result;
    memset(&result, '\0', sizeof(quint32));

    memcpy(&result, a, sizeof(quint32));

    //qDebug() << result;
    return result;

}

float ConvertRegisters::toFloatFromInverse(QVector<Reg_t> &regs, int target_reg)
{
    char a[4];
    a[0] = regs[target_reg+1].reg.at(1);
    a[1] = regs[target_reg+1].reg.at(0);
    a[2] = regs[target_reg].reg.at(1);
    a[3] = regs[target_reg].reg.at(0);
    float result;
    memset(&result, '\0', sizeof(quint32));
    memcpy(&result, a, sizeof(quint32));
    //qDebug() << result;
    return result;
}

int ConvertRegisters::floatToModbus(float value, quint16 target_reg, QVector<Reg_t> &regs)
{
    regs[target_reg].reg[0] = ((char*)&value)[1];
    regs[target_reg].reg[1] = ((char*)&value)[0];
    regs[target_reg+1].reg[0] = ((char*)&value)[3];
    regs[target_reg+1].reg[1] = ((char*)&value)[2];
    return 0;
}

int ConvertRegisters::floatToInvFlModbus(float value, quint16 target_reg, QVector<Reg_t> &regs)
{

    regs[target_reg].reg[0] = ((char*)&value)[3];
    regs[target_reg].reg[1] = ((char*)&value)[2];
    regs[target_reg+1].reg[0] = ((char*)&value)[1];
    regs[target_reg+1].reg[1] = ((char*)&value)[0];
    return 0;
}

int ConvertRegisters::shortToModbus(qint16 value, quint16 target_reg, QVector<Reg_t> &regs)
{
    regs[target_reg].reg[0] = ((char*)&value)[1];
    regs[target_reg].reg[1] = ((char*)&value)[0];
    return 0;
}

