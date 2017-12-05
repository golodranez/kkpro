#include "mainwindow.h"
#include <math.h>
//-----------------vse tut----------------------------
double K_toC_low[K_TOC_LOW_SIZE]={0, 2.508355e1, 7.860106e-2, -2.503131e-1, 8.315270e-2, -1.228034e-2,
                        9.804036e-4, -4.413030e-5, 1.057734e-6, -1.052755e-8};
double K_toC_top[K_TOC_TOP_SIZE]={-1.318058e2, 4.830222e1, -1.646031, 5.464731e-2, -9.650715e-4, 8.802193e-6, -3.110810e-8};

double K_tomV_top[K_TOMV_TOP_SIZE] = {-1.7600413686e-2, 3.8921204975e-2, 1.8558770032e-5, -9.9457592874e-8, 3.1840945719e-10,
                                      -5.6072844889e-13, 5.6075059059e-16, -3.2020720003e-19, 9.7151147152e-23, -1.2104721275e-26};

double B_toC_low[B_TOC_LOW_SIZE]={98.423321, 6.9971500e2, -8.4765304e2, 1.0052644e3, -8.3345952e2, 4.5508542e2,
                                -1.5523037e2, 29.886750, -2.4742860};
double B_toC_top[B_TOC_TOP_SIZE]={2.1315071e2, 2.8510504e2, -52.742887, 9.9160804, -1.2965303,
                                1.1195870e-1, -6.0625199e-3, 1.8661696e-4, -2.4878585e-6};

double B_tomV_low[B_TOMV_LOW_SIZE]={0, -2.4650818346e-4, 5.9040421171e-6, -1.3257931636e-9, 1.5668291901e-12,
                                   -1.6944529240e-15, 6.2990347094e-19};
double B_tomV_top[B_TOMV_TOP_SIZE]={-3.8938168621, 2.8571747470e-2, -8.4885104785e-5, 1.5785280164e-7,
                                    -1.6835344864e-10, 1.1109794013e-13, -4.4515431033e-17, 9.8975640821e-21, -9.3791330289e-25};

MainClass::MainClass(QObject *parent)
    : QObject(parent)
{

    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_RR);

    if(sched_setscheduler(0, SCHED_RR, &param) == -1)
    {
        perror("sched_setscheduler failed");
        exit(-1);
    }



    confHandler = new XmlConf(fullConfig, fullCalibration);
    confHandler->setConfFile("/opt/sntermo/conf.xml");
    confHandler->setCalibrationFile("/opt/sntermo/calibrations.xml");
    confHandler->parseConfig(&measurData);
    //qDebug() << "Conf size" << fullConfig.length();


    for(int i = 0; i < measurData.frameMap.size(); i++)
    {
        //qDebug() << "Sensor name:" << measurData.dataToSend[measurData.frameMap[i]].sensor_name << "\n";
        mDataforModBus.insert(measurData.frameMap[i], measurData.dataToSend[measurData.frameMap[i]]);
    }
//    InputsRegs.resize(measurData.frameMap.size() * 3 + 1);

    HoldingsRegs.resize(2000);
    for(int i = 0; i < 2000; i++)
    {
	HoldingsRegs[i].reg[0] = 0;
	HoldingsRegs[i].reg[1] = 0;
    }
/*    HoldingsRegs.resize(2);
    HoldingsRegs[0].reg.resize(2);
    HoldingsRegs[1].reg.resize(2);
    HoldingsRegs[0].reg[0] = 0;
    HoldingsRegs[0].reg[1] = 0;
    HoldingsRegs[1].reg[0] = 0;
    HoldingsRegs[1].reg[1] = 0;*/

    can1 = new Can(0, &measurData);

    if(can1->init(BasicCAN) != 0)
    {
        //exit -1;
    }
    can1->setMutex(&dataMutex);

    log = new Log("/opt/sntermo/can.log");
    connect(can1, SIGNAL(logReady(QString)), log, SLOT(writeLog(QString)));

    frameLog = new FrameLog;
    connect(can1, SIGNAL(frameLogReady(QString)), frameLog, SLOT(writeLog(QString)));

    canThread1 = new CanThread(can1);     //11111
    canThread1->start(QThread::TimeCriticalPriority);

    mbServer = new ModBusServer(InputsRegs, HoldingsRegs);
    mbServer->setMutex(&sendMutex);
    mbThread = new QThread;
    mbServer->moveToThread(mbThread);
    mbThread->start();

    usleep(130000);

    QTimer *tim = new QTimer;
    connect(tim, SIGNAL(timeout()), this, SLOT(proceedData()));
    connect(tim, SIGNAL(timeout()), this, SLOT(checkCommand()));
    tim->start(20);

    if(measurData.LVS_mode == true)
    {
        can = new Can(1, &measurData);
        can->init(BasicCAN);
	can->setMutex(&dataMutex);

	QThread *canTxThread = new QThread;
	can->moveToThread(canTxThread);
	canTxThread->start();

	QTimer *canTxTimer = new QTimer;
	connect(canTxTimer, SIGNAL(timeout()), can, SLOT(txCicle()));
	canTxTimer->start(100);
    }

    confServer = new QTcpServer;
    if(confServer->listen(QHostAddress::Any, 23456) == 0)
    {
        qDebug() << "listen error" << confServer->errorString();
    }
    else
    {
                    qDebug() << "listen" ;
    }
    connect(confServer, SIGNAL(newConnection()), this, SLOT(confConnection()));






}

MainClass::~MainClass()
{
    
}

void MainClass::proceedData()
{
    static bool first_run = true;

    dataMutex.lock();
    static int first_count = 0;
    first_count++;
    for(int i = 0; i < measurData.frameMap.size(); i++)
    {
      
        measurData.dataToSend[measurData.frameMap[i]].data = measurData.mData[measurData.frameMap[i]].data;
        measurData.dataToSend[measurData.frameMap[i]].bit = measurData.mData[measurData.frameMap[i]].bit;
        measurData.dataToSend[measurData.frameMap[i]].bit_amount = measurData.mData[measurData.frameMap[i]].bit_amount;
        //measurData.dataToSend[measurData.frameMap[i]].mb_status = measurData.mData[measurData.frameMap[i]].mb_status;
        measurData.dataToSend[measurData.frameMap[i]].status = measurData.mData[measurData.frameMap[i]].status;
        measurData.dataToSend[measurData.frameMap[i]].error_count = measurData.mData[measurData.frameMap[i]].error_count;
	

        if(first_run)
        {
            if(isnan(measurData.dataToSend[measurData.frameMap[i]].data) != FP_NAN)
            {
                measurData.dataToSend[measurData.frameMap[i]].data = 0;
                measurData.dataToSend[measurData.frameMap[i]].prev_data =0;
            }
            else
            {	if(i==12)
        	qDebug() << "not NAN " << i << " " << measurData.dataToSend[measurData.frameMap[i]].data;
                measurData.dataToSend[measurData.frameMap[i]].prev_data = measurData.dataToSend[measurData.frameMap[i]].data;   
            }
        }

        switch (measurData.dataToSend[measurData.frameMap[i]].type)
        {
            case TYPE_K:
                calcTempK(i);
                break;

            case TYPE_B:
                calcTempB(i);
                break;

            case TYPE_P:
            case TYPE_dP:
                calcPressure(i);
                break;

            case TYPE_S:
                calcOffset(i);
                break;

            case TYPE_TR:
                calcTR(i);
                break;

            default:
                break;
        }

	



        // ERROR PROCCED


///	if( (measurData.dataToSend[measurData.frameMap[i]].type == TYPE_B) && (measurData.dataToSend[measurData.frameMap[i]].data < -100) )
//	{
//	    measurData.dataToSend[measurData.frameMap[i]].data = -100;
//	}

	proceedStatus(i);
        measurData.dataToSend[measurData.frameMap[i]].prev_data = measurData.dataToSend[measurData.frameMap[i]].data;
	
	if(first_run)
	{
	    measurData.dataToSend[measurData.frameMap[i]].mb_status = 0;
	}
        
        
        
    }
    proceedGroup();
    dataMutex.unlock();

    

   sendMutex.lock();

    


    for(int i = 0; i < measurData.frameMap.size() - 1; i++) // minus 1 потому что двери отдельно
    {
    //qDebug() << "Data[" << i << "]: " << measurData.dataToSend[measurData.frameMap[i]].data;
        ConvertRegisters::floatToInvFlModbus(measurData.dataToSend[measurData.frameMap[i]].data, i*3, HoldingsRegs);
        ConvertRegisters::shortToModbus((short int)(measurData.dataToSend[measurData.frameMap[i]].mb_status), i*3+2, HoldingsRegs);
    }
//    ConvertRegisters::shortToModbus((short int)measurData.dataToSend[measurData.frameMap[measurData.frameMap.size()-1]].bit.at(0)
//            , measurData.frameMap.size()*3-3, HoldingsRegs);    // door 1
//    ConvertRegisters::shortToModbus((short int)measurData.dataToSend[measurData.frameMap[measurData.frameMap.size()-1]].bit.at(1)
//            , measurData.frameMap.size()*3-2, HoldingsRegs);    // door 2
   // ConvertRegisters::shortToModbus((short int)(mvv_errors), measurData.frameMap.size()*3+1, HoldingsRegs);


    if(first_count == 5)
        first_run = false;

   sendMutex.unlock();



}

void MainClass::checkCommand()
{
    unsigned short int command = ConvertRegisters::toShort(HoldingsRegs, 1000);

	if(command == 0xFFFF)
	{
		kvitCommand();
		ConvertRegisters::shortToModbus(0, 1000, HoldingsRegs);
		return;
	}
    if(command == 999)
    {
        pressureDiffToZero();
        ConvertRegisters::shortToModbus(0, 1000, HoldingsRegs);
        return;
    }
	
	for(int i = 1001; i < measurData.frameMap.size()+1001; i++)
	{
		command = ConvertRegisters::toShort(HoldingsRegs, i);
		if(command == 0xFFFF)
		{
			kvitChCommand(i-1001);
			ConvertRegisters::shortToModbus(0, i, HoldingsRegs);
		}
		else
		if(command == 0x7FFF)
		{
			disableChCommand(i-1001);
			ConvertRegisters::shortToModbus(0, i, HoldingsRegs);
		}			
	}

}

void MainClass::kvitCommand()
{
    dataMutex.lock();
    for(int i = 0; i < measurData.frameMap.size(); i++)
    {
        measurData.dataToSend[measurData.frameMap[i]].mb_status = 0;
    }
    dataMutex.unlock();
}

void MainClass::kvitChCommand(int channel)
{
    dataMutex.lock();
    measurData.dataToSend[measurData.frameMap[channel]].mb_status = 0;
    dataMutex.unlock();
}

void MainClass::disableChCommand(int channel)
{
    measurData.dataToSend[measurData.frameMap[channel]].mb_status = CH_DISABLED;
}

void MainClass::confConnection()
{
    confSocket = confServer->nextPendingConnection();
    QByteArray tmp;
    tmp.append(fullConfig);
    int i = confSocket->write(tmp);
    confSocket->flush();
    qDebug() << "Conf len: " << fullConfig.length() <<  "   write: " << i;
}

void MainClass::calcPressure(int i)
{
    float P_mv = measurData.dataToSend[measurData.frameMap[i]].data;
    measurData.dataToSend[measurData.frameMap[i]].data =  measurData.dataToSend[measurData.frameMap[i]].a_coeff * P_mv +
                                                             measurData.dataToSend[measurData.frameMap[i]].b_coeff;
}

void MainClass::calcTempK(int i)
{

}

void MainClass::calcTempB(int i)
{
    float B_mv = measurData.dataToSend[measurData.frameMap[i]].data;
    float K_t = measurData.dataToSend[measurData.sensorNameMap[measurData.dataToSend[measurData.frameMap[i]].comp_name]].data;
    int cj_status = measurData.dataToSend[measurData.sensorNameMap[measurData.dataToSend[measurData.frameMap[i]].comp_name]].mb_status;
    if(B_mv == -115)
    {
        //measurData.dataToSend[measurData.frameMap[i]].mb_status = LINE_ERROR;
    }
    else
    {
        double B_cj_mv = 0; // Cold junction temp

        if(cj_status == 0)
        {
            if(K_t < 630.615)
            {
                for(int bc = 0; bc < B_TOMV_LOW_SIZE; bc++)
                {
                    B_cj_mv += B_tomV_low[bc] * pow(K_t, bc);
                }
            }
            else
            {
                for(int bc = 0; bc < B_TOMV_TOP_SIZE; bc++)
                {
                    B_cj_mv += B_tomV_top[bc] * pow(K_t, bc);
                }
            }
        }
        else if(measurData.dataToSend[measurData.frameMap[i]].mb_status == 0)
        {
            measurData.dataToSend[measurData.frameMap[i]].mb_status = CJ_ERROR;
        }

        B_mv += B_cj_mv;

        double B_t = 0;
        if(B_mv < 2.431)
        {
            for(int bc = 0; bc < B_TOC_LOW_SIZE; bc++)
            {
                B_t += B_toC_low[bc] * pow(B_mv, bc);
            }
        }
        else
        {
            for(int bc = 0; bc < B_TOC_TOP_SIZE; bc++)
            {
                B_t += B_toC_top[bc] * pow(B_mv, bc);
            }
        }

        measurData.dataToSend[measurData.frameMap[i]].data = B_t;
    }
}

void MainClass::calcTR(int i)
{
    float R = measurData.dataToSend[measurData.frameMap[i]].data;
    measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].a_coeff * R +
                                                             measurData.dataToSend[measurData.frameMap[i]].b_coeff;
}

void MainClass::calcOffset(int i)
{

}

void MainClass::proceedStatus(int i)
{
    mvv_errors = 0;
    int tmp_status = measurData.dataToSend[measurData.frameMap[i]].mb_status;

    if(isnan(measurData.dataToSend[measurData.frameMap[i]].data) != FP_NAN)
    {
        measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].prev_data;
    }


// module error
    if(measurData.dataToSend[measurData.frameMap[i]].error_count != 0)
    {
        measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].prev_data;

        if(measurData.dataToSend[measurData.frameMap[i]].error_count >= 3)
        {
            measurData.dataToSend[measurData.frameMap[i]].mb_status = MVV_ERROR;
            measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].prev_data;
            mvv_errors++;
        }
    }

    if((measurData.dataToSend[measurData.frameMap[i]].mb_status != MVV_ERROR) && (measurData.dataToSend[measurData.frameMap[i]].mb_status != LINE_ERROR))
    {
// one step delta error
        if(fabs(measurData.dataToSend[measurData.frameMap[i]].data - measurData.dataToSend[measurData.frameMap[i]].prev_data) >
                                                                        measurData.dataToSend[measurData.frameMap[i]].step_delta)
        {
    	    if(i == 3) qDebug() << measurData.dataToSend[measurData.frameMap[i]].step_delta;
            measurData.dataToSend[measurData.frameMap[i]].mb_status = DELT_ERROR;
        }

        if((measurData.dataToSend[measurData.frameMap[i]].data < measurData.dataToSend[measurData.frameMap[i]].over_min) ||
         (measurData.dataToSend[measurData.frameMap[i]].data > measurData.dataToSend[measurData.frameMap[i]].over_max) )
        {
            measurData.dataToSend[measurData.frameMap[i]].mb_status = CH_OVERLOAD;
        }

        if(measurData.dataToSend[measurData.frameMap[i]].status == 1)
        {
            measurData.dataToSend[measurData.frameMap[i]].mb_status = LINE_ERROR;
            measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].prev_data;
        }

    if(measurData.dataToSend[measurData.frameMap[i]].mb_status == 0)
    {
    if(measurData.dataToSend[measurData.frameMap[i]].data > measurData.dataToSend[measurData.frameMap[i]].max_range)
        {
    		qDebug() << "Sensor:" << measurData.dataToSend[measurData.frameMap[i]].sensor_name;
    		qDebug() << measurData.dataToSend[measurData.frameMap[i]].max_range;
                measurData.dataToSend[measurData.frameMap[i]].mb_status = MAX_DIAP_ERROR;
        }
        if(measurData.dataToSend[measurData.frameMap[i]].data < measurData.dataToSend[measurData.frameMap[i]].min_range)
        {
            measurData.dataToSend[measurData.frameMap[i]].mb_status = MIN_DIAP_ERROR;
        }
    }

    }
    if(tmp_status == CH_DISABLED)
    {
        measurData.dataToSend[measurData.frameMap[i]].mb_status = CH_DISABLED;
    }
    if( (measurData.dataToSend[measurData.frameMap[i]].mb_status == LINE_ERROR && measurData.dataToSend[measurData.frameMap[i]].status != 0) ||
             (measurData.dataToSend[measurData.frameMap[i]].mb_status == MVV_ERROR && measurData.dataToSend[measurData.frameMap[i]].error_count != 0) )
    {
        measurData.dataToSend[measurData.frameMap[i]].data = measurData.dataToSend[measurData.frameMap[i]].prev_data;
    }
    else
    {
        measurData.dataToSend[measurData.frameMap[i]].prev_data = measurData.dataToSend[measurData.frameMap[i]].data;
    }
}

void MainClass::proceedGroup()
{

        int gr_amount = 0;
        for(int i = 0 ; i < measurData.groupMap.size(); i++)
        {
            for(int j = 0; j < measurData.groupMap[i].size(); j++)
            {
                if(measurData.dataToSend[measurData.groupMap[i][j]].mb_status == 0)
                {
                    measurData.groupMean[i] += measurData.dataToSend[measurData.groupMap[i][j]].data;
                    gr_amount++;
                }
            }
            measurData.groupMean[i] = measurData.groupMean[i] / gr_amount;
            gr_amount = 0;
        }

        for(int i = 0 ; i < measurData.groupMap.size(); i++)
        {
            for(int j = 0; j < measurData.groupMap[i].size(); j++)
            {
                if(fabs(measurData.dataToSend[measurData.groupMap[i][j]].data - measurData.groupMean[i]) > measurData.groupSIG[i])
                {
                    measurData.dataToSend[measurData.groupMap[i][j]].mb_status = GROUP_ERROR;
                }
            }
            measurData.groupMean[i] = 0;
        }

     /*   qDebug() << "gr_mean " << measurData.groupMean[4];
        qDebug() << "0" << measurData.dataToSend[measurData.groupMap[4][0]].data;
        qDebug() << "0" << measurData.dataToSend[measurData.groupMap[4][0]].mb_status;
        qDebug() << "1" << measurData.dataToSend[measurData.groupMap[4][1]].data;
        qDebug() << "0" << measurData.dataToSend[measurData.groupMap[4][0]].mb_status;
        qDebug() << "gr_mean " << measurData.groupMean[5];
        qDebug() << "2" << measurData.dataToSend[measurData.groupMap[5][0]].data;
        qDebug() << "3" << measurData.dataToSend[measurData.groupMap[5][1]].data;*/

}

void MainClass::pressureDiffToZero()
{
    QDomElement docElem = confHandler->confDoc->documentElement();
    QDomElement element = docElem.firstChildElement("mvv");

    while(!element.isNull())
    {
        QDomElement chElement = element.firstChildElement("channel");
        while(!chElement.isNull())
        {
            QString type = chElement.attribute("type");

            if(type == "dP")
            {
                QString s_name = chElement.attribute("sensor_name");
                if (measurData.dataToSend[measurData.sensorNameMap[s_name]].mb_status == 0)
                {
                    float b_coeff = measurData.dataToSend[measurData.sensorNameMap[s_name]].b_coeff;

                    float new_b = b_coeff - measurData.dataToSend[measurData.sensorNameMap[s_name]].data;
                    measurData.dataToSend[measurData.sensorNameMap[s_name]].b_coeff = new_b;
                    chElement.setAttribute("b", QString::number(new_b));
                }
            }
            chElement = chElement.nextSiblingElement("channel");
        }
        element = element.nextSiblingElement("mvv");
    }
    QFile outconf("/opt/sntermo/conf.xml");
    outconf.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outstream(&outconf);
    outstream << confHandler->confDoc->toString();
    outconf.close();

}
