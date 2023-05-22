#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent) : QDialog(parent), ui(new Ui::Player){
    ui->setupUi(this);

    qRegisterMetaType<std::vector<Frame>>();

    thrPlayFile = std::thread(&Player::playLogFile, this);

    connect(ui->pBChooseFile, &QPushButton::clicked, this, &Player::slotChooseFile);
    connect(ui->pBLoadFile, &QPushButton::clicked, this, &Player::slotLoadFile);
    connect(ui->lEInpFile, &QLineEdit::textChanged, this, &Player::slotPathChanged);
}

Player::~Player(){
    if (thrPlayFile.joinable()) thrPlayFile.join();

    delete ui;
}

void Player::slotChooseFile(){
    pathFileLog = QFileDialog::getOpenFileName(this, tr("Choose log file"), "", tr("Log files (*.log)"));
    if(!pathFileLog.isEmpty())
        ui->lEInpFile->setText(pathFileLog);
}

void Player::slotLoadFile(){
    if(ui->lEInpFile->text().isEmpty()){
        QMessageBox::information(this, "Input from log file","Empty input data");
        return;
    }
    // --- --- ---

    QFile file(pathFileLog);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error", "'" + pathFileLog + "'\nFile not found");
        ui->pBarLoad->setValue(0);
        return;
    }
    else{
        std::ifstream fileStream(pathFileLog.toLocal8Bit()); // + cyrillic
        int stringCount = std::count(std::istreambuf_iterator<char>(fileStream),
                                     std::istreambuf_iterator<char>(), '\n');
        std::vector<QString> dataStrings = readStrings(file, stringCount); // ...loading...
        makeFrames(dataStrings);
    }
}

void Player::slotOneBack(){
    (currFrameIndex <= 0) ? currFrameIndex = frames.size() - 1 : currFrameIndex--;
    std::vector<Frame> locVec = {frames[currFrameIndex]};
    emit signFrameMessage(locVec);
    emit signStatePlay(PlSt::Back, currFrameIndex);
}

void Player::slotPause(){
    isPlay = false;
    emit signStatePlay(PlSt::Pause, currFrameIndex);
}

void Player::slotOneForw(){
    (currFrameIndex >= frames.size() - 1) ? currFrameIndex = 0 : currFrameIndex++;
    std::vector<Frame> locVec = {frames[currFrameIndex]};
    emit signFrameMessage(locVec);
    emit signStatePlay(PlSt::Forw, currFrameIndex);    
}

void Player::slotRoughRewind(int val){
    if(val < 0 || val >= (int)frames.size()) return;
    if(!isPlay){
        currFrameIndex = val;
        emit signStatePlay(PlSt::Pause, currFrameIndex);
    }
}

std::vector<QString> Player::readStrings(QFile &file, int linesAmount){
    std::vector<QString> vecStrings;
    ui->pBarLoad->setValue(0);
    uint8_t percent = 0;
    uint8_t tempPer = 0;
    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        vecStrings.push_back(line);
        percent = (vecStrings.size() / (float)linesAmount) * 100;
        if(percent != tempPer && percent % 5 == 0){
            ui->pBarLoad->setValue(percent);
            tempPer = percent;
        }
    }
    return vecStrings;
}

void Player::makeFrames(const std::vector<QString> &strings){
    if(strings.size() % 2 != 0){
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","Bad file data (%2 size != 0)");
        return;
    }
    // --- programm state ---
    QJsonDocument jDoc = QJsonDocument::fromJson(strings.at(0).toUtf8());
    QJsonObject jObj;
    if(!jDoc.isNull()){
        if(jDoc.isObject()){
            jObj = jDoc.object();
            emit signStateMain(jObj);
        }
        else{
            ui->pBLoadFile->setStyleSheet("background-color: red");
            QMessageBox::critical(this, "Error","Json document is not an object");
            return;
        }
    }
    else{
        ui->pBLoadFile->setStyleSheet("background-color: red");
        QMessageBox::critical(this, "Error","Invalid JSON (json is null)");
        return;
    }

    // --- all data ---
    currFrameIndex = 0;
    frames.clear();
    uint8_t gap = 2;
    std::vector<QString> vecTemp(strings.size() - gap);
    std::copy(strings.begin() + gap, strings.end(), vecTemp.begin());
    // ---
    Frame locFrame;
    for (size_t i = 0; i < vecTemp.size(); i++) {
        QList listData = vecTemp[i].split("\t");
        if(i == 0 || i % 2 == 0){
            if(listData.size() != 3){ // wrong data
                ui->pBLoadFile->setStyleSheet("background-color: red");
                QMessageBox::critical(this, "Error","Bad file data (header of data)");
                return;
            }
            locFrame.timeStamp = listData.at(0).toDouble();
            locFrame.channelNum = listData.at(1).toUShort();
            locFrame.deviceIndex = listData.at(2).toUShort();
        }
        else{
            locFrame.payload.clear();
            for (const auto& val : qAsConst(listData))
                if(!val.isEmpty())
                    locFrame.payload.push_back(val.toUInt());
            frames.push_back(locFrame);
        }
    }
    // ---
    ui->pBLoadFile->setStyleSheet("background-color: green");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    emit signStatePlay(PlSt::Start, frames.size());
    this->hide();
}

void Player::playLogFile(){
    while(!isAppStopped){
        std::this_thread::sleep_for(std::chrono::milliseconds(bigLoopMs));
        if(isPlay){
            if(frames.empty()) continue;
            double diffTime = GET_CUR_TIME_MICRO - frames[currFrameIndex].timeStamp;
            while (isPlay) {
                if(isAppStopped) break;
                std::this_thread::sleep_for(std::chrono::microseconds(3));
                if(GET_CUR_TIME_MICRO >= (frames[currFrameIndex].timeStamp + diffTime)){
                    // --- send to displ ---
                    std::vector<Frame> locVec = {frames[currFrameIndex]};
                    emit signFrameMessage(locVec);
                    emit signStatePlay(PlSt::Play, currFrameIndex);
                    // ---
                    currFrameIndex++;
                    if(currFrameIndex >= frames.size()){
                        // --- stop ---
                        currFrameIndex = 0;
                        isPlay = false;
                        emit signStatePlay(PlSt::Pause, currFrameIndex);
                        break;
                    }
                }
            }
        }
    }
}
