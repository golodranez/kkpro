#ifndef DATA_T_H
#define DATA_T_H

#include <QString>
#include <QBitArray>

class AbstractSensor
{
public:

private:
    float data;
    float prev_data;
    int min_range;
    int max_range;
    int status;
};


class data_t
{
public:
    data_t();

    float data;
    quint16 bit_amount;
    QBitArray bit;
    float prev_data;
    int group;
    float step_delta;
    float a_coeff;
    float b_coeff;
    float over_min;
    float over_max;
    float min_range;
    float max_range;
    int status;
    int mb_status;
    int type;
    bool filled;
    QString sensor_name;
    QString comp_name;
    int mmv_num;
    int can_frame_id;
    unsigned short error_count;
};

#endif // DATA_T_H
