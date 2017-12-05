#ifndef API_FUNC_H
#define API_FUNC_H

#include <QString>
#include <QMutex>



#define NSEC_INSEC (1000000000)
#define MAX_FRAME_RECV_TIME (45000000)
#define CAN_ONE_CICLE_TIME  (90000000)

#define UNKNOW_ERROR     (1)
#define MVV_ERROR        (2)
#define CH_DISABLED      (3)
#define CH_OVERLOAD      (4)
#define LINE_ERROR       (5)
#define MIN_DIAP_ERROR   (6)
#define MAX_DIAP_ERROR   (7)
#define DELT_ERROR       (8)
#define CJ_ERROR         (9)
#define GROUP_ERROR      (10)

#define QVIT_COMMAND (127)
#define DISABLE_CH_COMMAND (128)


// SENSOR TYPES
#define TYPE_K  (0)
#define TYPE_B  (1)
#define TYPE_P  (2)
#define TYPE_dP (3)
#define TYPE_S  (4)
#define PURE    (5)
#define TYPE_TR (6)
#define DI_TYPE  (7)

#define DELT (100)

// MODBUS COMMANDS
#define KVITIROVAT (99)

#define K_TOC_LOW_SIZE (10)
#define K_TOC_TOP_SIZE (7)

//double K_toC_low[K_TOC_LOW_SIZE]={0, 2.508355e1, 7.860106e-2, -2.503131e-1, 8.315270e-2, -1.228034e-2,
 //                       9.804036e-4, -4.413030e-5, 1.057734e-6, -1.052755e-8};
//double K_toC_top[K_TOC_TOP_SIZE]={-1.318058e2, 4.830222e1, -1.646031, 5.464731e-2, -9.650715e-4, 8.802193e-6, -3.110810e-8};


#define K_TOMV_TOP_SIZE (10)
#define K_tomV_C0 (1.185976e-1)
#define K_tomV_C1 (-1.183432e-4)

//double K_tomV_top[K_TOMV_TOP_SIZE] = {-1.7600413686e-2, 3.8921204975e-2, 1.8558770032e-5, -9.9457592874e-8, 3.1840945719e-10,
                                     // -5.6072844889e-13, 5.6075059059e-16, -3.2020720003e-19, 9.7151147152e-23, -1.2104721275e-26};

#define B_TOC_LOW_SIZE (9) // 250 - 700C, 0.291 - 2.431 mV
#define B_TOC_TOP_SIZE (9) // 700 - 1820C, 2.431 - 13.820 mV

//double B_toC_low[B_TOC_LOW_SIZE]={98.423321, 6.9971500e2, -8.4765304e2, 1.0052644e3, -8.3345952e2, 4.5508542e2,
  //                               -1.5523037e2, 29.886750, 2.4742860};
//double B_toC_top[B_TOC_TOP_SIZE]={2.1315071e2, 2.8510504e2, -52.742887, 9.9160804, -1.2965303,
//                                 1.1195870e-1, -6.0625199e-3, 1.8661696e-4, -2.4878585e-6};


#define B_TOMV_LOW_SIZE (7) // 250 - 630.615C
#define B_TOMV_TOP_SIZE (9) // 700 - 1820C,

//double B_tomV_low[B_TOMV_LOW_SIZE]={0, -2.4650818346e-4, 5.9040421171e-6, -1.3257931636e-9, 1.5668291901e-12,
 //                                  -1.6944529240e-15, 6.2990347094e-19};
//double B_tomV_top[B_TOMV_TOP_SIZE]={-3.8938168621, 2.8571747470e-2, -8.4885104785e-5, 1.5785280164e-7,
    //                                -1.6835344864e-10, 1.1109794013e-13, -4.4515431033e-17, 9.8975640821e-21, -9.3791330289e-25};





#define E_CONST (2,718281828459)

QString getMVVUnit(unsigned frame_id);
int getMVVChannelID(int frame_id);
int getMVVCanID(int frame_id);
int getAVCanFrameId(int MVV_can_id, int ch_number);
int getDICanFrameId(int MVV_can_id);
bool isLineSignal(QString type);
//int getTimeForSleep // !!! TO DO




#endif // API_FUNC_H
