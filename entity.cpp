#include "entity.h"

std::string Frame::getOneStdLine() const{
    std::string resLine;
    // --- header ---
    resLine +=
            std::to_string(GET_CUR_TIME_MICRO) + "\t" +
            std::to_string(channelNum) + "\t" +
            std::to_string(deviceIndex) + "\n";
    // --- body ---
    for (size_t i = 0; i < payload.size(); i++)
        resLine += std::to_string(payload[i]) + "\t";
    resLine += "\n";
    return resLine;
}

QString Frame::getOneQstrLine() const{
    QString resLine;
    // --- header ---
    resLine +=
            QString::number(GET_CUR_TIME_MICRO) + "\t" +
            QString::number(channelNum) + "\t" +
            QString::number(deviceIndex) + "\n";
    // --- body ---
    for (size_t i = 0; i < payload.size(); i++)
        resLine += QString::number(payload[i]) + "\t";
    resLine += "\n";
    return resLine;
}

QJsonObject EXTRACONFIG::toJsonObject(){
    QJsonObject extraConfig;

    QJsonArray arrLeverPos;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        arrLeverPos.push_back(m_nLeverPos[i]);

    extraConfig[keyDeviceIndex] = m_nDeviceIndex;
    extraConfig[keyDeviceNum]   = m_nDeviceNum;
    extraConfig[keyYTFormat]    = m_nYTFormat;
    extraConfig[keyTriggerMode] = m_nTriggerMode;
    extraConfig[keyTriggerSweep] = m_nTriggerSweep;
    extraConfig[keyLeverPos]    = arrLeverPos;

    return extraConfig;
}

void EXTRACONFIG::fromJson(const QJsonObject &obj){
    if(obj.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Bad json input (EXTRACONFIG)");
        return;
    }
    // ---
    // TODO: void EXTRACONFIG::fromJson(const QJsonObject &obj)
}

QJsonObject _HT_CONTROL_DATA::toJsonObject(){
    QJsonObject jCntrlData;
    jCntrlData[keyCHSet]        = nCHSet;
    jCntrlData[keyTimeDIV]      = nTimeDIV;
    jCntrlData[keyTrigSource]   = nTriggerSource;
    jCntrlData[keyHTriggerPos]  = nHTriggerPos;
    jCntrlData[keyVTriggerPos]  = nVTriggerPos;
    jCntrlData[keyTriggerSlope] = nTriggerSlope;
    jCntrlData[keyBufferLen]    = QString::number(nBufferLen);
    jCntrlData[keyReadDataLen]  = QString::number(nReadDataLen);
    jCntrlData[keyAlreadyReadLen] = QString::number(nAlreadyReadLen);
    jCntrlData[keyALT]          = nALT;
    jCntrlData[keyETSOpen]      = nETSOpen;
    jCntrlData[keyDriverCode]   = nDriverCode;
    jCntrlData[keyLastAddress]  = QString::number(nLastAddress);
    jCntrlData[keyFPGAVersion]  = nFPGAVersion;
    return jCntrlData;
}

void _HT_CONTROL_DATA::fromJson(const QJsonObject &obj){
    if(obj.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Bad json input (CONTROL_DATA)");
        return;
    }
    // ---
    // TODO: void _HT_CONTROL_DATA::fromJson(const QJsonObject &obj)
}

QJsonObject _HT_RELAY_CONTROL::toJsonObject(){
    QJsonObject jRelCntrl;

    QJsonArray arrCHEnable;
    QJsonArray arrCHVoltDIV;
    QJsonArray arrCHCoupling;
    QJsonArray arrCHMult;
    QJsonArray arrCHBWLimit;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        arrCHEnable.push_back(bCHEnable[i]);
        arrCHVoltDIV.push_back(nCHVoltDIV[i]);
        arrCHCoupling.push_back(nCHCoupling[i]);
        arrCHMult.push_back(nCHMult[i]);
        arrCHBWLimit.push_back(bCHBWLimit[i]);
    }

    jRelCntrl[keyCHEnable]      = arrCHEnable;
    jRelCntrl[keyCHVoltDIV]     = arrCHVoltDIV;
    jRelCntrl[keyCHCoupling]    = arrCHCoupling;
    jRelCntrl[keyCHMult]        = arrCHMult;
    jRelCntrl[keyCHBWLimit]     = arrCHBWLimit;

    jRelCntrl[keyTrigSource]    = nTrigSource;
    jRelCntrl[keyTrigFilt]      = bTrigFilt;
    jRelCntrl[keyALT]           = nALT;

    return jRelCntrl;
}

void _HT_RELAY_CONTROL::fromJson(const QJsonObject &obj){
    if(obj.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Bad json input (RELAY_CONTROL)");
        return;
    }
    // ---
    // TODO: void _HT_RELAY_CONTROL::fromJson(const QJsonObject &obj)
}
