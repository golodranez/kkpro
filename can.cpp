#include "can.h"
#include "data_t.h"
#include <time.h>
#include <sched.h>

#define NUMOF_MVV 12
#define FRAME_LOG_ON (0)

#include <QDebug>
#include <QString>


void Can::setMutex(QMutex *mutex_in)
{
    mutex = mutex_in;
}

Can::Can(int dev, MeasurData *mData_in, QObject *parent) :
    QObject(parent)
{
    device = dev; /* Номер платы */
    speed = CAN_SPEED; /* Скорость */
    mData = mData_in;


}

int Can::init(int can_mode)
{
#ifdef ELCUS_200PCI
    if ((device < 0) || (speed < 0))
    {
        qDebug("Device not correct");
        return EXIT_FAILURE;
    }

    if (CAN_SPEED_1000 != speed && CAN_SPEED_800 != speed &&
        CAN_SPEED_500 != speed && CAN_SPEED_250 != speed &&
        CAN_SPEED_125 != speed && CAN_SPEED_50 != speed &&
        CAN_SPEED_20 != speed && CAN_SPEED_10 != speed)
    {
        qDebug() << "Speed error";
        return -1;
    }


    sprintf(name, "/dev/can200_%d", device);
    hCan = open(name, O_RDWR | O_NONBLOCK);
    if (-1 == hCan)
    {
        qDebug() << "Error open /dev/can200_" << device << "d";
        return EXIT_FAILURE;
    }
    qDebug() << "Device open";
    int result = CAN200_GetConfig(hCan, &conf);
    if (EOK != result)
    {
        printf("Error GetConfig (%d, %s).\n", result, strerror(errno));
        close(hCan);
        return EXIT_FAILURE;
    }
    printf("Board: %s\n", conf.szName);
    printf("Serial num.: %s\n", conf.szSN);
    printf("Base port: %Xh\n", conf.wPorts);
    printf("IRQ: %.2d\n\n", conf.wIRQ);

    result = CAN200_SetWorkMode(hCan, can_mode);
    if (EOK != result)
    {
        printf("Error CAN200_SetWorkMode (%d, %s).\n", result, strerror(errno));
        close(hCan);
        return EXIT_FAILURE;
    }

    if(can_mode == PeliCAN)
    {
        result = CAN200_P_SetFilter(hCan, 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
    }
    if(can_mode == BasicCAN)
    {
        result = CAN200_B_SetFilter(hCan, 0xff, 0xff);
    }
    if (EOK != result)
    {
        printf("Error P_SetInputFilter (%d, %s).\n", result, strerror(errno));
        close(hCan);
        return EXIT_FAILURE;
    }

    result = CAN200_SetCANSpeed(hCan, speed);
    if (EOK != result)
    {
        printf("Error SetCANSpeed (%d, %s).\n", result, strerror(errno));
        close(hCan);
        return EXIT_FAILURE;
    }

    result = CAN200_SetDriverMode(hCan);
    if (EOK != result)
    {
        printf("Error SetDriverMode (%d, %s).\n", result, strerror(errno));
        close(hCan);
        return EXIT_FAILURE;
    }
#endif

#ifdef DRPC_100

    PORT_STRUCT setPort;
    setPort.mode     = 0;       //  0 : 11-bit ;  1 : 29-bit CAN network
    setPort.accCode  = 0x0; //	Only ID = accCode can pass the filter
    setPort.accMask  = 0xffffffff;   //  Don't care bit
    setPort.baudrate = 8;       //	0: 10kBps; 1: 20kBps; 2: 50kBps; 3: 100kBps; 4: 125kBps; 5: 250kBps; 6:500kBps; 7: 800kBps; 8:1MBps


    if ((hCan = CanOpenDriver(0, 0)) == -1)
    {
         printf("\nOpen Port 0 Failed!!");
         return -1;
    }

    CanConfigPort(hCan, &setPort);


    //rcvPatterns[1] = 0;
#endif

 //   qDebug() << "INIT OK";
    return 0;
}

int Can::Read(CAN_data& data)
{
#ifdef ELCUS_200PCI
    fd_set rfd; /* For select */
    struct timeval tv; /* Time out */

    FD_ZERO(&rfd);
    FD_SET(hCan, &rfd);
    tv.tv_sec = 0;
    tv.tv_usec = 10000; /* 100 ms */
    int result = select(hCan + 1, &rfd, NULL, NULL, &tv);
    if (result == -1)
    {
        printf("Read Error select (%s)\n", strerror(errno));
        //close(hCan);
        return -1;
    }
    else if (result == 0) return 1; // timeout


    if (FD_ISSET(hCan, &rfd))
    {
        result = read(hCan, &data, sizeof(CAN_data));
        if (result == -1)
        {
            printf("Error read (%s)\n", strerror(errno));
            close(hCan);   // TUT NADO PODUMAT'
            return -1;
        }
        else if (result == 0)  return 2; // error

        if ((data.type!=BasicCAN ) && (data.type!= PeliCAN))
        {
            /* error frame */
            printf("Error frame receive\n");
            printf("mode = %Xh\n", data.data[0]);
            printf("status = %Xh\n", data.data[1]);
            printf("error code = %Xh\n", data.data[2]);
            printf("lost capture = %Xh\n", data.data[3]);
            return 2; // error;
        }
        return 0;
    }
    return 2;
#endif
    //==================================
#ifdef DRPC_100
    CAN_PACKET data_100;

    if(CanRcvMsg(hCan, &data_100) == -1)
    {
        return -1;
    }
    data.id = data_100.CAN_ID;
    memcpy(data.data, data_100.data, 8);
    return 0;

#endif

}

int Can::Read_DRPC(CAN_PACKET &data)
{
    if(CanRcvMsg(hCan, &data) == -1)
    {
        return -1;
    }
    return 0;
}

int Can::Write(const CAN_data& data)
{
#ifdef ELCUS_200PCI
    fd_set wfd; /* For select */
    struct timeval tv; /* Time out */

    FD_ZERO(&wfd);
    FD_SET(hCan, &wfd);
    tv.tv_sec = 0;
    tv.tv_usec = 100000; /* 100 ms */
    int result = select(hCan + 1, NULL, &wfd, NULL, &tv);
    if (result == -1)
    {
        printf("Error select (%s)\n", strerror(errno));
        // Закрываем плату
        close(hCan);
        return -1;
    }
    else if (result == 0) return 1; // timeout

    if (FD_ISSET(hCan, &wfd))
    {
        /* Вызов CAN200_GetTransmitStatus приведен для примера */
        /* В данной точке программы очередь выдачи пуста */
        int status = 0;
        int len = -1;
        result = CAN200_GetTransmitStatus(hCan, &status, &len);
        if (result != EOK)
        {
            printf("Error SetDriverMode (%d, %s).\n", result, strerror(errno));
            // Закрываем плату
            close(hCan);
            return -1;
        }
        if ((status != CAN_TRANSMIT_COMPLETE) && (status != CAN_TRANSMIT_WAIT))
        {
            printf("Error transmit, status = %d\n", status);
            // Закрываем плату
            close(hCan);
            return -1;
        }
        // Выдача
        result = write(hCan, &data, sizeof(CAN_data));
        if (result == -1)
        {
            printf("Error write (%s)\n", strerror(errno));
            // Закрываем плату
            close(hCan);
            return -1;
        }
        else
        {
            return result;
        }
        return 0;
    }
    return 2;
#endif

#ifdef DRPC_100

    CAN_PACKET data_100;
    data_100.CAN_ID = data.id;
    data_100.rtr = data.rtr;
    data_100.len = 8;
    memcpy(data_100.data, data.data, 8);

    if(CanSendMsg(hCan, &data_100) == -1)
    {
        return -1;
    }
    return 0;

#endif
}

int Can::Write_DRPC(CAN_PACKET &data)
{
    CanSendMsg(hCan, &data);
    return 0;
}


int Can::runRecvCicle()
{
/*
    int time_error_counter = 0;
    int can_frame_order_error = 0;
    int not_full_c = 0;
    int counter_r = 0;
    bool find_new_frame = false;

    int can_circle_error = 0;
    int select_time_out_c = 0;*/


    int time_for_OK = 0;
    CAN_data data;
    data.type = BasicCAN;
    data.dlc = 0;
    data.rtr = 0;
    data.id = 0x60;
    for (i = 0; i < 8; i++)
    {
        data.data[i] = 0;
    }

quint8 miss_counter = 0;

    while(true)
    {

        timespec start_point;
        clock_gettime(CLOCK_MONOTONIC, &start_point);

        //CanCloseDriver(hCan);
        //  init(BasicCAN);


        Write(data);
        CAN_data frame;
        int frame_counter = 0;
        int frame_recv_time_nsec = 0;

	//mutex->lock();
        while(frame_counter < mData->frameMap.size())
        {

            const int res = Read(frame);
#ifdef DRPC_100
          /*  timespec miss_point, miss_pointe;

            clock_gettime(CLOCK_MONOTONIC, &miss_point);
*/
      //  CanClearRxBuffer(hCan);
      //  CanClearTxBuffer(hCan);
  //          clock_gettime(CLOCK_MONOTONIC, &miss_pointe);
    //        qDebug() << "Start 2: " << miss_pointe.tv_sec - miss_point.tv_sec << "\n" << miss_pointe.tv_nsec << miss_pointe.tv_nsec;
#endif
#ifndef DRPC_100
            if(res == 1) //timeout
            {
#endif
                //qDebug() << "READ Timeout";
                //emit logReady(QString("Registred not valid frame: %1");
                timespec in_circle_point;
                clock_gettime(CLOCK_MONOTONIC, &in_circle_point);

                if( (in_circle_point.tv_sec - start_point.tv_sec) == 0 )
                    frame_recv_time_nsec = in_circle_point.tv_nsec - start_point.tv_nsec;
                else
                if(in_circle_point.tv_nsec < start_point.tv_nsec)
                {
                    frame_recv_time_nsec = in_circle_point.tv_nsec + NSEC_INSEC - start_point.tv_nsec;
                }
                else
                {
                    //SOVSEM ZHOPA
                }
                if(frame_recv_time_nsec > MAX_FRAME_RECV_TIME)
                {
                   // mutex->unlock();
                    usleep(0);
                    break;
                }
#ifndef DRPC_100
            }
#endif
            if (res != 0)
            {
                emit logReady(QString("Read error: %1").arg(res));
                //mutex->unlock();
                continue;
            }



            frame_counter ++;
            mutex->lock();
        
            if(mData->mData.contains(frame.id))
            {
                if(mData->mData[frame.id].filled == true)
                {
                    //emit logReady(QString("Povtor: %1").arg(getMVVUnit(frame.id)));
                    frame_counter--;
                    mutex->unlock();
                    continue;
                }
                else
                {
            	    
                    if(mData->mData[frame.id].type != DI_TYPE)
                    {
                        float value = 0;
                        memcpy((void *)&value, frame.data, 4);
                        mData->mData[frame.id].data = value;
               //     if (frame.id == getAVCanFrameId(1, 0))
               //     {
               //         qDebug() << "TEMP: " << mData->mData[frame.id].data;
               //     }
                        mData->mData[frame.id].status = frame.data[4];
                    }
                    /*int mvv_id = (int)(frame.id & 31);
                    int ch_id = (int)((frame.id >> 5) & 31);
                    if(ch_id > 7)
                    qDebug() << mvv_id << "    " << ch_id;
                    if (mvv_id == 15)
                	qDebug() << "MVV "<< mvv_id;*/
                    else
                    {	
                    //qDebug() << "DI priletelo:";
                        for(i = 0; i < 32; i++)
                        {
                            int value = 0;
                            memcpy((void *)&value, frame.data, 4);
                    	    mData->mData[frame.id].bit[i] = ((value >> i) & 1);
                    //	    qDebug() << ((value >> i) & 1);
                        }
			

                    }
                    mData->mData[frame.id].filled = true;

                }
                

            }
            else
            {
#ifndef DRPC_100
                emit logReady(QString("Registred not valid frame: %1").arg(getMVVUnit(frame.id)));
#endif
		//qDebug() << "Registred not valid frame: " << getMVVUnit(frame.id);
                frame_counter--;
                mutex->unlock();
                usleep(1000);
                continue;
            }
            sleep(0);
            mutex->unlock();

        }
#ifdef DRPC_100
        CanClearRxBuffer(hCan);
        CanClearTxBuffer(hCan);
#endif


      //  qDebug() << "recv time: " << frame_recv_time_nsec;

        if ( (frame_counter == mData->frameMap.size()) || (frame_recv_time_nsec > MAX_FRAME_RECV_TIME) )
        {
            mutex->lock();
     //       qDebug() << "in pocessing cicle";
            int fill_count = 0;
            for(int i = 0; i < mData->frameMap.size(); i++)
            {
        	/*if(i==40)
        	{
        	    qDebug() << "CANthread " <<  mData->mData[mData->frameMap[i]].data;
        	}*/
        	
        	
                if(mData->mData[mData->frameMap[i]].filled == true)
                {
                    fill_count++;
                    mData->mData[mData->frameMap[i]].filled = false;
                    mData->mData[mData->frameMap[i]].error_count = 0;
                }
                else
                {
                    mData->mData[mData->frameMap[i]].error_count++;
                }
            }

            for(int i = 0; i < mData->frameMap.size(); i++) //only for debug
            {
                if(mData->mData[mData->frameMap[i]].error_count != 0)
                {
                  //  CanCloseDriver(hCan);
                  // init(BasicCAN);
                    emit this->logReady("Missed frame: " + getMVVUnit(mData->mData[mData->frameMap[i]].can_frame_id));
                }
            }
            mutex->unlock();

            if(fill_count != mData->frameMap.size())
            {
                emit this->logReady(QString("%1 Readed frames != numof_channels\trecv time:%2").arg(fill_count).arg(frame_recv_time_nsec));
                miss_counter++;
                if(miss_counter == 2)
                {
                    timespec miss_point, miss_pointe;

                    clock_gettime(CLOCK_MONOTONIC, &miss_point);
                    CanCloseDriver(hCan);
                    init(BasicCAN);
                    clock_gettime(CLOCK_MONOTONIC, &miss_pointe);
                    qDebug() << "Start: " << miss_pointe.tv_sec - miss_point.tv_sec << "\n" << miss_pointe.tv_nsec << miss_pointe.tv_nsec;
                    miss_counter = 0;
                }
            }
            else
            {
                timespec end_point;
                int circle_time;
                clock_gettime(CLOCK_MONOTONIC, &end_point);

                if( (end_point.tv_sec - start_point.tv_sec) == 0 )
                    circle_time = end_point.tv_nsec - start_point.tv_nsec;
                else
                if(end_point.tv_nsec < start_point.tv_nsec)
                {
                    circle_time = end_point.tv_nsec + NSEC_INSEC - start_point.tv_nsec;
                }
                else
                {
                    //SOVSEM ZHOPA
                }

                int time_for_sleep = CAN_ONE_CICLE_TIME - circle_time;
                timespec sleep_tv;
                sleep_tv.tv_sec = end_point.tv_sec;
                sleep_tv.tv_nsec = end_point.tv_nsec + time_for_sleep;
                clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &sleep_tv, NULL);

            //    qDebug() << "Circle time: " << circle_time << "\n\tTime_for_sleep: " << time_for_sleep;

                if(circle_time > 55000000)
                {
//!                    emit logReady(QString("time ERROR: %2").arg(circle_time));
                }
                time_for_OK++;
                if(time_for_OK >= 6000)
                {
                    emit this->logReady("All OK!");
                    time_for_OK = 0;
                }
            }
        }
        else
        {
             emit logReady(QString("error frame_counter != mData->frameMap.size() %1").arg(frame_counter));
        }
    
    //mutex->unlock();
    }
    return 0;





}

void Can::txCicle()
{
    //int time_for_OK = 0;
    CAN_data data;
    data.type = BasicCAN;
    data.dlc = 4;
    data.rtr = 0;



    mutex->lock();
    for(int i = 0; i < mData->frameMap.size(); i++)
    {
        data.id = (int)((1 << 10) | (i & 1023));
        memset(data.data, '\0', 8);
        float tmp_data = mData->dataToSend[mData->frameMap[i]].data;
        memcpy( ( void*) data.data, (void*)&tmp_data, sizeof(float));
        if(Write(data) < 0)
        {
    	    
    	    init(BasicCAN);
    	    break;
        }
    }
    mutex->unlock();
}

