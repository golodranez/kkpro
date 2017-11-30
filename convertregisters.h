#ifndef CONVERTREGISTERS_H
#define CONVERTREGISTERS_H

#include <QObject>
#include "mobus_conf.h"

class ConvertRegisters : public QObject
{
    Q_OBJECT
public:
    explicit ConvertRegisters(QObject *parent = 0);
    static qint16 toShort(QVector<Reg_t> &regs, int target_reg);
    static qint32 toInt(QVector<Reg_t> &regs, int target_reg);
    static float  toFloat(QVector<Reg_t> &regs, int target_reg);
    static float  toFloatFromInverse(QVector<Reg_t> &regs, int target_reg);

    static int floatToModbus(float value, quint16 target_reg, QVector<Reg_t> &regs);
    static int floatToInvFlModbus(float value, quint16 target_reg, QVector<Reg_t> &regs);
    static int shortToModbus(qint16 value, quint16 target_reg, QVector<Reg_t> &regs);

signals:

public slots:

};

#endif // CONVERTREGISTERS_H
