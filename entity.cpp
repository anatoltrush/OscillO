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

void Frame::clear(){
    this->deviceIndex = 0;
    this->channelNum = 0;
    this->timeStamp = 0.0;
    this->payload.clear();
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

    extraConfig[keyPWCond]      = pwCondInd;
    extraConfig[keyPulseWidUn]  = pulseWidUnit;
    extraConfig[keyPulseWidVal] = pulseWidVal;

    extraConfig[keyLastPLSize]  = lastPLSize;

    return extraConfig;
}

void EXTRACONFIG::fromJson(const QJsonObject &obj){
    if(obj.isEmpty()){
        QMessageBox::critical(nullptr, "Error", "Bad json input (EXTRACONFIG)");
        return;
    }
    // ---
    QJsonArray arrLeverPos = obj[keyLeverPos].toArray();
    for(uint8_t i = 0; i < MAX_CH_NUM; i++)
        m_nLeverPos[i] = arrLeverPos[i].toInt();
    m_nDeviceIndex  = obj[keyDeviceIndex].toInt();
    m_nDeviceNum    = obj[keyDeviceNum].toInt();
    m_nYTFormat     = obj[keyYTFormat].toInt();
    m_nTriggerMode  = obj[keyTriggerMode].toInt();
    m_nTriggerSweep = obj[keyTriggerSweep].toInt();

    pwCondInd       = obj[keyPWCond].toInt();
    pulseWidUnit    = obj[keyPulseWidUn].toInt();
    pulseWidVal     = obj[keyPulseWidVal].toInt();

    lastPLSize      = obj[keyLastPLSize].toInt();
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
    nCHSet          = obj[keyCHSet].toInt();
    nTimeDIV        = obj[keyTimeDIV].toInt();
    nTriggerSource  = obj[keyTrigSource].toInt();
    nHTriggerPos    = obj[keyHTriggerPos].toInt();
    nVTriggerPos    = obj[keyVTriggerPos].toInt();
    nTriggerSlope   = obj[keyTriggerSlope].toInt();
    nBufferLen      = obj[keyBufferLen].toString().toInt();
    nReadDataLen    = obj[keyReadDataLen].toString().toInt();
    nAlreadyReadLen = obj[keyAlreadyReadLen].toString().toInt();
    nALT            = obj[keyALT].toInt();
    nETSOpen        = obj[keyETSOpen].toInt();
    nDriverCode     = obj[keyDriverCode].toInt();
    nLastAddress    = obj[keyLastAddress].toString().toInt();
    nFPGAVersion    = obj[keyFPGAVersion].toInt();
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
    QJsonArray arrCHEnable  = obj[keyCHEnable].toArray();
    QJsonArray arrCHVoltDIV = obj[keyCHVoltDIV].toArray();
    QJsonArray arrCHCoupling = obj[keyCHCoupling].toArray();
    QJsonArray arrCHMult    = obj[keyCHMult].toArray();
    QJsonArray arrCHBWLimit = obj[keyCHBWLimit].toArray();
    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        bCHEnable[i]    = arrCHEnable[i].toBool();
        nCHVoltDIV[i]   = arrCHVoltDIV[i].toInt();
        nCHCoupling[i]  = arrCHCoupling[i].toInt();
        nCHMult[i]      = arrCHMult[i].toInt();
        bCHBWLimit[i]   = arrCHBWLimit[i].toBool();
    }
    nALT = obj[keyALT].toInt();
    nTrigSource = obj[keyTrigSource].toInt();
    bTrigFilt = obj[keyTrigFilt].toBool();
}

bool _HT_RELAY_CONTROL::operator ==(const _HT_RELAY_CONTROL &rlc){
    bool isAlt          = nALT == rlc.nALT;
    bool isTrigFilt     = bTrigFilt == rlc.bTrigFilt;
    bool isTrigSrc      = nTrigSource == rlc.nTrigSource;
    bool isCHEnable     = true;
    bool isCHVoltDIV    = true;
    bool isCHCoupling   = true;
    bool isCHLimit      = true;
    bool isCHMult      = true;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        if(bCHEnable[i] != rlc.bCHEnable[i]) isCHEnable = false;
        if(nCHVoltDIV[i] != rlc.nCHVoltDIV[i]) isCHVoltDIV = false;
        if(nCHCoupling[i] != rlc.nCHCoupling[i]) isCHCoupling = false;
        if(bCHBWLimit[i] != rlc.bCHBWLimit[i]) isCHLimit = false;
        if(nCHMult[i] != rlc.nCHMult[i]) isCHMult = false;
    }
    return isAlt && isTrigFilt && isTrigSrc &&
            isCHEnable && isCHVoltDIV && isCHCoupling && isCHLimit && isCHMult;
}

_HT_RELAY_CONTROL &_HT_RELAY_CONTROL::operator =(const _HT_RELAY_CONTROL &rlc){
    nTrigSource = rlc.nTrigSource;
    bTrigFilt   = rlc.bTrigFilt;
    nALT        = rlc.nALT;
    for(uint8_t i = 0; i < MAX_CH_NUM; i++){
        bCHEnable[i] = rlc.bCHEnable[i];
        nCHVoltDIV[i] = rlc.nCHVoltDIV[i];
        nCHCoupling[i] = rlc.nCHCoupling[i];
        nCHMult[i] = rlc.nCHMult[i];
        bCHBWLimit[i] = rlc.bCHBWLimit[i];
    }
    return *this;
}
