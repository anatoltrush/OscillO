#ifndef ENTITY_H
#define ENTITY_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

#define HANTEK_NUM      (2)
#define MAX_CH_NUM      (4)
#define ESTIM_NUM       (3)
#define IND_TO_NUM      (1)
#define DATA_SIZE_STEP  (1024)

#define GET_CUR_TIME_MILLI (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
#define GET_CUR_TIME_MICRO (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())

struct Frame{
    uint8_t deviceIndex = 0;
    uint8_t channelNum  = 0;
    double timeStamp    = 0.0;
    std::vector<uint16_t> payload;

    // --- methods ---
    static uint8_t getHeaderSize(){return 3;} // deviceIndex, channelNum, timeStamp
    std::string getOneLine(){
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
};

// --- extra ---
const QString keyDeviceIndex    = "deviceIndex";
const QString keyDeviceNum      = "deviceNum";
const QString keyYTFormat       = "yTFormat";
const QString keyTriggerMode    = "triggerMode";
const QString keyTriggerSweep   = "triggerSweep";
const QString keyLeverPos       = "leverPos";

// --- control data ---
const QString keyCHSet          = "cHSet";
const QString keyTimeDIV        = "timeDIV";
const QString keyHTriggerPos    = "hTriggerPos";
const QString keyVTriggerPos    = "vTriggerPos";
const QString keyTriggerSlope   = "triggerSlope";
const QString keyBufferLen      = "bufferLen";
const QString keyReadDataLen    = "readDataLen";
const QString keyAlreadyReadLen = "alreadyReadLen";
const QString keyALT            = "aLT";
const QString keyETSOpen        = "eTSOpen";
const QString keyDriverCode     = "driverCode";
const QString keyLastAddress    = "lastAddress";
const QString keyFPGAVersion    = "fPGAVersion";

// --- relay control ---
const QString keyCHEnable       = "cHEnable";
const QString keyCHVoltDIV      = "cHVoltDIV";
const QString keyCHCoupling     = "cHCoupling";
const QString keyCHMult         = "cHMultiplier";
const QString keyCHBWLimit      = "cHBWLimit";

const QString keyTrigSource     = "trigSource";
const QString keyTrigFilt       = "trigFilt";

struct EXTRACONFIG{
    ushort m_nDeviceIndex   = 255;
    ushort m_nDeviceNum     = 0;
    ushort m_nYTFormat      = 0;
    ushort m_nTriggerMode   = 0;
    ushort m_nTriggerSweep  = 0;
    ushort m_nLeverPos[MAX_CH_NUM] = {0};

    // ---
    QJsonObject toJsonObject(){
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
};

typedef struct _HT_CONTROL_DATA{
    ushort nCHSet           = 0;
    ushort nTimeDIV         = 0;
    ushort nTriggerSource   = 0;
    ushort nHTriggerPos     = 0;
    ushort nVTriggerPos     = 0;
    ushort nTriggerSlope    = 0;
    ulong nBufferLen        = 0;
    ulong nReadDataLen      = 0;
    ulong nAlreadyReadLen   = 0;
    ushort nALT             = 0;
    ushort nETSOpen         = 0;
    ushort nDriverCode      = 0;
    ulong nLastAddress      = 0;
    ushort nFPGAVersion     = 0;

    // ---
    QJsonObject toJsonObject(){
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
} CONTROLDATA, *PCONTROLDATA;

typedef struct _HT_RELAY_CONTROL{
    bool bCHEnable[MAX_CH_NUM]      = {0, 0, 0, 0};
    ushort nCHVoltDIV[MAX_CH_NUM]   = {0};
    ushort nCHCoupling[MAX_CH_NUM]  = {0};
    ushort nCHMult[MAX_CH_NUM]      = {0};
    bool bCHBWLimit[MAX_CH_NUM]     = {false};
    ushort nTrigSource              = 0;
    bool bTrigFilt                  = false;
    ushort nALT                     = 0;

    // ---
    QJsonObject toJsonObject(){
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
} RELAYCONTROL, *PRELAYCONTROL;

#endif // ENTITY_H
