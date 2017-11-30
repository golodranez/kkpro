#include <QDebug>
#include "measurdata.h"

MeasurData::MeasurData(QObject *parent) :
    QObject(parent)
{
    const int MAX_CH_NUMB = 8;


}

void MeasurData::fillSensorName()
{
    for(int i = 0; i < 5; i++)
    {
        sensorNameMap.insert(mData[frameMap[i]].sensor_name, frameMap[i]);
    }
}
