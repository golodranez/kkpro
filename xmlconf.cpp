#include "xmlconf.h"
#include "api_func.h"
#include <QDebug>
#include <unistd.h>

XmlConf::XmlConf(QString &fullConfig_in, QString &fullcalibration_in, QObject *parent) :
    QObject(parent), fullConfig(fullConfig_in), fullcalibration(fullcalibration_in)
{
    confDoc = new QDomDocument;
    calibrationDoc = new QDomDocument;

}

int XmlConf::setConfFile(QString confFile_in)
{
    confFile = new QFile(confFile_in);
    if (!confFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
            emit signalLog("Couldn't open conf file");
            qDebug() << "Couldn't open conf file";
            return -1;
    }

    fullConfig = confFile->readAll();
    confFile->close();

    if (!confFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
            emit signalLog("Couldn't open conf file");
            qDebug() << "Couldn't open conf file";
            return -1;
    }


    if (!confDoc->setContent(confFile))
    {
        emit signalLog("Conf file is bad XML");
        qDebug() << "bad XML";
        return -1;
    }
    return 0;
}

int XmlConf::setCalibrationFile(QString confFile_in)
{
    calibrationFile = new QFile(confFile_in);
    if (!calibrationFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
            emit signalLog("Couldn't open conf file");
            qDebug() << "Couldn't open conf file";
            return -1;
    }

    fullcalibration = confFile->readAll();
    calibrationFile->close();

    if (!calibrationFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
            emit signalLog("Couldn't open conf file");
            qDebug() << "Couldn't open conf file";
            return -1;
    }


    if (!calibrationDoc->setContent(calibrationFile))
    {
        emit signalLog("Conf file is bad XML");
        qDebug() << "bad XML";
        return -1;
    }
    return 0;
}

int XmlConf::parseConfig(MeasurData *measureData_in)
{

    QDomElement docElem = confDoc->documentElement();
    QDomElement calibElem = calibrationDoc->documentElement();

    QDomElement conf = docElem.firstChildElement("K_type");
    int k_min = conf.attribute("over_min").toLong();
    int k_max = conf.attribute("over_max").toLong();
    
    conf = docElem.firstChildElement("B_type");
    int b_min = conf.attribute("over_min").toLong();
    int b_max = conf.attribute("over_max").toLong();

    conf = docElem.firstChildElement("LVS");
    measureData_in->LVS_mode = conf.attribute("mode").toLong();
    
    int delt_k = conf.attribute("t_K").toLong();
    int delt_b = conf.attribute("t_B").toLong();

    conf = docElem.firstChildElement("GROUPS");
    measureData_in->group_amount = conf.attribute("amount").toLong();
    measureData_in->groupSIG.resize(measureData_in->group_amount);
    measureData_in->groupMap.resize(measureData_in->group_amount);
    measureData_in->groupMean.resize(measureData_in->group_amount);

    for(int i = 0; i < measureData_in->group_amount; i++)
    {
        measureData_in->groupSIG[i] = conf.attribute("sig" + QString::number(i + 1)).toFloat();
    }


    QDomElement element = docElem.firstChildElement("mvv");

    while(!element.isNull())
    {

        int mvv_can_id = element.attribute("id").toLong();
    //    qDebug() << "Type: " << element.attribute("type");


        QDomElement chElement = element.firstChildElement("channel");
        while(!chElement.isNull())
        {
            data_t md;
            int channel_number = chElement.attribute("numb").toLong();

            QString type = chElement.attribute("type");
            QString comp_sensor;
            if(type == "K") // rewrite it in CASE style
            {
                md.type = TYPE_K;
                md.over_min = k_min;
                md.over_max = k_max;
                
                conf = docElem.firstChildElement("DELT");
                md.step_delta = conf.attribute("t_K").toFloat();
                //md.step_delta = delt_k;
                //qDebug() << delt_k;
            }
            if(type == "B")
            {
                comp_sensor = chElement.attribute("comp_sensor");
                md.type = TYPE_B;
                md.over_min = b_min;
                md.over_max = b_max;
                md.comp_name = comp_sensor;
                md.step_delta = delt_k;
            }



            if(type == "P")
            {
                md.type = TYPE_P;
                QString s_type = chElement.attribute("sub_type");
                conf = docElem.firstChildElement(s_type);
                md.over_min = conf.attribute("over_min").toFloat();
                md.over_max = conf.attribute("over_max").toFloat();
                conf = docElem.firstChildElement("DELT");
                md.step_delta = conf.attribute(s_type).toFloat();

            }
            if(type == "dP")
            {
                md.type = TYPE_dP;
                QString s_type = chElement.attribute("sub_type");
                conf = docElem.firstChildElement(s_type);
                md.over_min = conf.attribute("over_min").toFloat();
                md.over_max = conf.attribute("over_max").toFloat();
                conf = docElem.firstChildElement("DELT");
                md.step_delta = conf.attribute(s_type).toFloat();
            }
            if(type == "TR")
            {
                md.type = TYPE_TR;
                conf = docElem.firstChildElement(type);
                md.over_min = conf.attribute("over_min").toFloat();
                md.over_max = conf.attribute("over_max").toFloat();
                conf = docElem.firstChildElement("DELT");
                md.step_delta = conf.attribute(type).toFloat();
            }
            if(type == "DI")
            {
            qDebug() << " DI conf";
                md.type = DI_TYPE;
                md.bit_amount = conf.attribute("bit_amount").toLong();
            }
            if(type == "PURE")
            {
                md.type = PURE;
            }

            md.sensor_name = chElement.attribute("sensor_name");

            if(isLineSignal(type))
            {
                QDomElement calib = calibElem.firstChildElement(md.sensor_name);
                md.a_coeff = calib.attribute("a").toFloat();
                md.b_coeff = calib.attribute("b").toFloat();
            }

            if(type != "DI")
            {
                md.min_range = chElement.attribute("min_range").toFloat();
                md.max_range = chElement.attribute("max_range").toFloat();
                md.can_frame_id = getAVCanFrameId(mvv_can_id, channel_number);
            }
            if(type == "DI")
            {
        	md.can_frame_id = getDICanFrameId(mvv_can_id);
            }
            

            int group = chElement.attribute("group").toLong();
            if(group > 0)
            {
                measureData_in->groupMap[group-1].push_back(md.can_frame_id);
                md.group = group;
            }






            md.filled = false;
            md.error_count = 0;
            md.prev_data = 0;
            md.mmv_num = mvv_can_id;
            md.mb_status = 0;
            md.status = 0;

            measureData_in->frameMap.push_back(md.can_frame_id);
            measureData_in->mData.insert(md.can_frame_id, md);
            measureData_in->dataToSend.insert(md.can_frame_id, md);
            measureData_in->buff.insert(md.can_frame_id, md);
            measureData_in->sensorNameMap.insert(md.sensor_name, md.can_frame_id);

            chElement = chElement.nextSiblingElement("channel");
        }

        element = element.nextSiblingElement("mvv");
    }


    return 0;
}
