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
#define MAX_VERT_AXIS   (std::numeric_limits<uint8_t>::max())
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
    std::string getOneStdLine() const;
    QString getOneQstrLine() const;
};

struct EXTRACONFIG{
private:
    const QString keyDeviceIndex    = "deviceIndex";
    const QString keyDeviceNum      = "deviceNum";
    const QString keyYTFormat       = "yTFormat";
    const QString keyTriggerMode    = "triggerMode";
    const QString keyTriggerSweep   = "triggerSweep";
    const QString keyLeverPos       = "leverPos";

    const QString keyPWCond         = "PWCondition";
    const QString keyPulseWidVal    = "pulseWValue";
    const QString keyPulseWidUn     = "pulseWUnit";

public:
    ushort m_nDeviceIndex   = 255;
    ushort m_nDeviceNum     = 0;
    ushort m_nYTFormat      = 0;
    ushort m_nTriggerMode   = 0;
    ushort m_nTriggerSweep  = 0;
    ushort m_nLeverPos[MAX_CH_NUM] = {0, 0, 0, 0};
    ushort pwCondInd        = 0;
    ushort pulseWidUnit     = 0;
    int pulseWidVal       = 0;
    // ---
    QJsonObject toJsonObject();
    void fromJson(const QJsonObject& obj);
};

typedef struct _HT_CONTROL_DATA{
private:
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

    const QString keyTrigSource     = "trigSource";

public:
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
    QJsonObject toJsonObject();
    void fromJson(const QJsonObject& obj);

} CONTROLDATA, *PCONTROLDATA;

typedef struct _HT_RELAY_CONTROL{
private:
    const QString keyCHEnable       = "cHEnable";
    const QString keyCHVoltDIV      = "cHVoltDIV";
    const QString keyCHCoupling     = "cHCoupling";
    const QString keyCHMult         = "cHMultiplier";
    const QString keyCHBWLimit      = "cHBWLimit";
    const QString keyTrigFilt       = "trigFilt";

    const QString keyALT            = "aLT";
    const QString keyTrigSource     = "trigSource";

public:
    bool bCHEnable[MAX_CH_NUM]      = {0, 0, 0, 0};
    ushort nCHVoltDIV[MAX_CH_NUM]   = {0, 0, 0, 0};
    ushort nCHCoupling[MAX_CH_NUM]  = {0, 0, 0, 0};
    ushort nCHMult[MAX_CH_NUM]      = {0, 0, 0, 0};
    bool bCHBWLimit[MAX_CH_NUM]     = {0, 0, 0, 0};
    ushort nTrigSource              = 0;
    bool bTrigFilt                  = false;
    ushort nALT                     = 0;

    // ---
    QJsonObject toJsonObject();
    void fromJson(const QJsonObject& obj);

    // ---
    bool operator == (const _HT_RELAY_CONTROL& rlc);
    _HT_RELAY_CONTROL& operator = (const _HT_RELAY_CONTROL& rlc);

} RELAYCONTROL, *PRELAYCONTROL;

#endif // ENTITY_H
