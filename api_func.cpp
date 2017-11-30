#include "api_func.h"

QString getMVVUnit(unsigned frame_id)
{
    return QString("id = %1 channel = %2").
            arg(frame_id & 31).
            arg((frame_id >> 5) & 31);
}

int getMVVChannelID(int frame_id)
{
    return (frame_id >> 5) & 31;
}

int getMVVCanID(int frame_id)
{
    return frame_id & 31;
}


int getAVCanFrameId(int MVV_can_id, int ch_number)
{
    return (int)((1 << 10) | ((ch_number & 31) << 5) | (MVV_can_id & 31));
}

int getDICanFrameId(int MVV_can_id)
{
    return (int)((0 << 10) | ((14 & 31) << 5) | (MVV_can_id & 31));
}
