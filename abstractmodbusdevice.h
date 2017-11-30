#ifndef ABSTRACTMODBUSDEVICE_H
#define ABSTRACTMODBUSDEVICE_H

#include <QObject>
#include <QtCore>

class AbstractModbusDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractModbusDevice(QObject *parent = 0);
    virtual int getDeviceType() = 0;

signals:

public slots:

};

#endif // ABSTRACTMODBUSDEVICE_H
