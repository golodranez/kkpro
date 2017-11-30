#ifndef MEASURDATA_H
#define MEASURDATA_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <data_t.h>

class MeasurData : public QObject
{
    Q_OBJECT
public:
    explicit MeasurData(QObject *parent = 0);
    //data_t *md, *md_buff;
    QVector <unsigned long> frameMap; // value - can_id
    QMap <QString, unsigned long> sensorNameMap; // key - sensor name, value - can_id
    QMap <unsigned long, data_t> mData; // key - can_id, value - data_t
    QMap <unsigned long, data_t> buff;  // mData buff
    QMap <unsigned long, data_t> dataToSend;
    bool LVS_mode;
    int load_counter;

    int group_amount;
    QVector <float> groupSIG;
    QVector < QVector <int> > groupMap;
    QVector <float> groupMean;

    void fillSensorName();

signals:

    
public slots:


    
};

#endif // MEASURDATA_H
