#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString LocalIP = getLocalIP();
//    ui->ServerIPLine->setText(LocalIP);

    MySocket=new QTcpSocket();
    MyServer=new QTcpServer();
    SpeedTimer=new QTimer();
    SpeedTimer->start(100);

    NormalLackGroup=new QButtonGroup();
    NormalLackGroup->addButton(ui->NormalSendRadio);
    NormalLackGroup->addButton(ui->LackSendRadio);
    ui->NormalSendRadio->setChecked(true);
    ServerClientGroup=new QButtonGroup();
    ServerClientGroup->addButton(ui->ServerRadio);
    ServerClientGroup->addButton(ui->ClientRadio);
    ui->ServerRadio->setChecked(true);

    // 信号槽函数，连接信号和这种功能
    connect(ui->ConnectButton, SIGNAL(clicked()),this,SLOT(socketConnect()));
    connect(ui->SendButton, SIGNAL(clicked()),this,SLOT(socketSendFile()));
    connect(ui->PauseButton, SIGNAL(clicked()),this,SLOT(pauseCall()));
    connect(MySocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    connect(MySocket, SIGNAL(readyRead()),this,SLOT(socketRecFile()));
    connect(ui->ClearButton, SIGNAL(clicked()), this, SLOT(windowClear()));
    connect(ui->ResendButton, SIGNAL(clicked()), this, SLOT(reSend()));
    connect(SpeedTimer, SIGNAL(timeout()), this, SLOT(speedShow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 网络连接
void MainWindow::socketConnect()
{
    if(ServerClientGroup->checkedButton()==ui->ServerRadio) {
        if(ifConnected==0){ // server模式没有连接过,此时要连接
            ui->ConnectButton->setText("正在连接");
            ui->ClientIPLine->setDisabled(true);
            ui->ClientRadio->setDisabled(true);
            ui->ServerRadio->setDisabled(true);
            ui->PortBox->setDisabled(true);

            // 作为服务器端连接
            connect(MyServer, &QTcpServer::newConnection, this, &MainWindow::serverNewConnect);
            if(!MyServer->listen(QHostAddress::Any, ui->PortBox->value())){   // 开始监听连接
                qDebug() << "ifconnect set to be 0" << endl;
                ifConnected=0;
                ui->ConnectButton->setText("点击连接");
                ui->ClientIPLine->setEnabled(true);
                ui->ClientRadio->setEnabled(true);
                ui->ServerRadio->setEnabled(true);
                ui->PortBox->setEnabled(true);
                ui->SendButton->setDisabled(true);

                qDebug()<<MyServer->errorString();    //错误信息
            }
        }
        else
            ServerSocket->disconnectFromHost();
    }
    else {   // 客户端模式
        if(MySocket->state()==QAbstractSocket::UnconnectedState)
            MySocket->connectToHost(ui->ClientIPLine->text(), ui->PortBox->value());
        else if(MySocket->state()==QAbstractSocket::ConnectedState)
            MySocket->disconnectFromHost();
    }
}

// 断开连接
void MainWindow::socketDisconnect()
{
    RSModel=0; // 重置收发模式
    ui->ConnectButton->setText("点击连接");
    ui->ClientIPLine->setEnabled(true);
    ui->ClientRadio->setEnabled(true);
    ui->ServerRadio->setEnabled(true);
    ui->PortBox->setEnabled(true);
    ui->SendButton->setDisabled(true);
    MyServer->disconnect();
    MyServer->close();
    ifConnected=0;  // 在监听中，准备重新连接
}

// 服务器端产生了新的连接
void MainWindow::serverNewConnect()
{
    ui->ConnectButton->setText("点击断开");
    ui->ClientIPLine->setDisabled(true);
    ui->ClientRadio->setDisabled(true);
    ui->ServerRadio->setDisabled(true);
    ui->PortBox->setDisabled(true);
    ui->SendButton->setEnabled(true);

    ServerSocket=MyServer->nextPendingConnection();
    connect(ServerSocket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnect);
    connect(ServerSocket, &QTcpSocket::readyRead, this, &MainWindow::socketRecFile);
    ifConnected=1; // 连接上了
}

// 发送具体的帧
void MainWindow::socketSendFrame()
{
    QTcpSocket *nowSocket;
    if(ServerClientGroup->checkedButton()==ui->ServerRadio)
        nowSocket=ServerSocket;
    else
        nowSocket=MySocket;

    if(nowSocket->state() == QAbstractSocket::ConnectedState){  // 可以发送
        for(int i=0;i<ConfirmVec.size();i++){
            if(ConfirmVec[i]==0 && SendUnACKCount < ui->WindowSIzeBox->value()){ // i=0代表未发送
                ui->MessageText->append("开始发送第"+QString::number(i)+"帧！");
                if(i!=LackFrameNo){ // 指定缺失帧
                    nowSocket->write(ByteVec[i],ByteVec[i].length()); // 发送未发送满的窗口数量
                    nowSocket->waitForBytesWritten();
                    SpeedCount+=ByteVec[i].length();
                }
                else{
                    ui->MessageText->append("缺失一次！");
                    LackFrameNo=100; // 只阻挠发送一次
                }
                ui->MessageText->append("第"+QString::number(i)+"帧数据发送完成！");
                SendUnACKCount++; // 已发送但未验证的数量加1
                QTimer *MyTimer=new QTimer;
                MyTimer->start(MaxWaitTime);
                TimerVec.push_back(MyTimer); // 只判断第一个是否超时
                ConfirmVec[i]=1; // 对引用赋值，所以是改变了原来的
            }
        }
        // 将当前正在等待的验证定时器连接到响应函数
        if(TimerVec.size())
            connect(TimerVec[0], SIGNAL(timeout()), this, SLOT(ackTimeOut()));
    }
}

// 只在发送模式下调用
// 发送按钮按下后调用一次，之后在接收功能中判定后自动调用上一个
void MainWindow::socketSendFile()
{
    ByteVec.clear();
    TimerVec.clear();
    ConfirmVec.clear();

    RSModel=1; // 调用时就设置成发送模式
    SendFile=new QFile("send.txt");
    if (!SendFile->open(QFile::ReadOnly))     //不能带QFile::Text，因为文本格式是不会去读0x0D的，导致读取到的数据会将0x0D去除掉
        qDebug()<<"Open file err";

    if(NormalLackGroup->checkedButton()==ui->LackSendRadio)
        LackFrameNo=ui->LackFrameLine->text().toInt();
    qDebug() << LackFrameNo << endl;

    for (int i=0;i<FrameCount ;i++ ) {
        QByteArray LineData=SendFile->readLine();
        LineData.insert(0, 0xFF);
        LineData.insert(1, 0xD8);
        LineData.insert(2, char(i));
        LineData.append(0xFF);
        LineData.append(0xD9);
        ByteVec.push_back(LineData);
        ConfirmVec.push_back(0); // 全都未发送
    }
    socketSendFrame();
    SendFile->close();
}


// 接受发送来的数据
void MainWindow::socketRecFile()
{
    QTcpSocket *nowSocket;
    if(ServerClientGroup->checkedButton()==ui->ServerRadio)
        nowSocket=ServerSocket;
    else
        nowSocket=MySocket;

    // 接收模式，需要发送验证位
    if(RSModel==0){
        if(nowSocket->state() == QAbstractSocket::ConnectedState) {
            QByteArray buffer = nowSocket->readAll();
            SpeedCount+=buffer.size();
            // 数据
            for (QByteArray::size_type i = 0; i < buffer.size(); i++) {
                switch (RecStatus) {
                case 0: // 未验证头
                    if (0xFF == (unsigned char)(buffer[i]))
                        RecStatus++;
                    RecByte.clear();
                    break;
                case 1: // 已验证一个头
                    if (0xD8 == (unsigned char)(buffer[i]))
                        RecStatus++;
                    else
                        RecStatus = 0;
                    break;
                case 2:
                    RecByte.append(buffer[i]);
                    if (0xFF == (unsigned char)(buffer[i]))
                        RecStatus++;
                    break;
                case 3:
                    RecByte.append(buffer[i]);
                    if (0xD9 == (unsigned char)(buffer[i])){ // 一帧接受完成，需要把数据尾和无用标识符去掉
                        uchar FrameNo = uchar(RecByte[0]);
                        if(FrameNo==NowWaitFrameNo){
                            RecByte.remove(0,1);
                            qDebug() << RecByte << endl;
                            RecByte.remove(12,2);
                            qDebug() << RecByte << endl;
                            RecByteVec.push_back(RecByte); // 压入缓存
                            ui->MessageText->append("第"+QString::number(NowWaitFrameNo)+"帧数据接收完成！");
                            QByteArray ackSend;
                            ackSend.append(NowWaitFrameNo);
                            nowSocket->write(ackSend,1); // 发送验证位
                            nowSocket->waitForBytesWritten();
                            ui->MessageText->append("第"+QString::number(NowWaitFrameNo)+"验证位发送成功！");
                            ++NowWaitFrameNo;
                        }
                        else if(FrameNo==0xFF){
                            RecByteVec.clear();
                            NowWaitFrameNo=0;
                            ui->MessageText->append("准备接受重发数据");
                        }
                        if(NowWaitFrameNo==10){ // 所有数据接收完成
                            NowWaitFrameNo=0; // 重置等待位
                            // 将接收数据写入内存
                            QByteArray AllByte;
                            AllByte=RecByteVec[0]+RecByteVec[1]+RecByteVec[2]+RecByteVec[3]+RecByteVec[4]+RecByteVec[5]+RecByteVec[6]+RecByteVec[7]+
                                    RecByteVec[8]+RecByteVec[9];
                            QDateTime current_time=QDateTime::currentDateTime();
                            QString thistime=current_time.toString("yyyyMMddhhmmss");
                            QFile *myFile=new QFile(thistime + ".txt");
                            myFile->open(QIODevice::WriteOnly);
                            int writeBit = myFile->write(AllByte);
                            ui->MessageText->append("接收所有数据都已保存！长度为" + QString::number(writeBit));
                            myFile->close();
                        }
                        RecStatus = 0;
                    }
                    else
                        RecStatus = 2;
                    break;
                default:
                    break;
                }
            }
        }
    }
    else{ // 发送模式，需要接受验证位
        if(nowSocket->state() == QAbstractSocket::ConnectedState){
            QByteArray buffer = nowSocket->readAll();
            SpeedCount+=buffer.size();
            // 有可能同时接受到多个帧的验证位
            for(int i=0;i<buffer.size();i++){
                uchar FrameNo = uchar(buffer[i]);
                int frameno=int(FrameNo);
                if(NowWaitFrameNo==frameno){
                    ConfirmVec[frameno]=2;
                    ui->MessageText->append("第"+QString::number(frameno)+"验证位接收成功！");
                    TimerVec[0]->stop();
                    disconnect(TimerVec[0], SIGNAL(timeout()), this, SLOT(ackTimeOut()));
                    auto pos=TimerVec.begin();
                    TimerVec.erase(pos);
                    --SendUnACKCount;
                    ++NowWaitFrameNo;
                    if(NowWaitFrameNo==10)
                        NowWaitFrameNo=0;
                }

            }
            // 全部验证完后再进行发送
            if(!IfPause)
                socketSendFrame();
        }
    }
}

void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState) {
    case QAbstractSocket::UnconnectedState:
        ui->ConnectButton->setText("点击连接");
        ui->ClientIPLine->setEnabled(true);
        ui->ClientRadio->setEnabled(true);
        ui->ServerRadio->setEnabled(true);
        ui->PortBox->setEnabled(true);
        ui->SendButton->setDisabled(true);
        break;
    case QAbstractSocket::ConnectingState:
        ui->ConnectButton->setText("正在连接");
        ui->ClientIPLine->setDisabled(true);
        ui->ClientRadio->setDisabled(true);
        ui->ServerRadio->setDisabled(true);
        ui->PortBox->setDisabled(true);
        ui->SendButton->setDisabled(true);
        break;
    case QAbstractSocket::ConnectedState:
        ui->ConnectButton->setText("点击断开");
        ui->ClientIPLine->setDisabled(true);
        ui->ClientRadio->setDisabled(true);
        ui->ServerRadio->setDisabled(true);
        ui->PortBox->setDisabled(true);
        ui->SendButton->setEnabled(true);
        break;
    default:
        break;
    }
}

// 获取本机所有IP地址
QString MainWindow::getLocalIP()
{
    QString hostName=QHostInfo::localHostName();//本地主机名
    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QString localIP="";
    QList<QHostAddress> addList=hostInfo.addresses();//

    if (!addList.isEmpty()){
        for (int i=0;i<addList.count();i++) {
            QHostAddress aHost=addList.at(i);
            if (aHost.toIPv4Address()) {
                localIP=aHost.toString();
                ui->ServerIPLine->addItem(localIP);
            }
        }
    }
    return localIP;
}

// 接受验证位超时，重新发送当前帧
void MainWindow::ackTimeOut()
{
    // 等待验证为超时，按照当前等待验证的位置重新发送窗口大小
    SendUnACKCount=0; // 将发送未验证的设为0
    TimerVec[0]->stop();
    disconnect(TimerVec[0], SIGNAL(timeout()), this, SLOT(ackTimeOut()));
    TimerVec.clear();
    for(int i=0;i<ConfirmVec.size();i++){
        if(ConfirmVec[i]==1 ){ // i=0代表未发送
            ConfirmVec[i]=0;
        }
    }
    ui->MessageText->append("从第"+QString::number(NowWaitFrameNo)+"帧开始重新发送！");
    socketSendFrame();
}

// 暂停功能
void MainWindow::pauseCall()
{
    IfPause=!IfPause;
    if(!IfPause)
        socketSendFrame();
}

// 清除当前窗口显示内容
void MainWindow::windowClear()
{
    ui->MessageText->clear();
}

// 重新发送所有数据
// 要先向对方接收端发送重新发送的指令
void MainWindow::reSend()
{
    QByteArray LineData;
    LineData.append(0xFF);
    LineData.append(1, 0xD8);
    LineData.append(2, 0xFF); // 重新发送标志位
    LineData.append(0xFF);
    LineData.append(0xD9);

    QTcpSocket *nowSocket;
    if(ServerClientGroup->checkedButton()==ui->ServerRadio)
        nowSocket=ServerSocket;
    else
        nowSocket=MySocket;

    nowSocket->write(LineData,LineData.length()); // 发送未发送满的窗口数量
    nowSocket->waitForBytesWritten();

    ui->MessageText->append("重新发送！");
    if(TimerVec.size()){
        TimerVec[0]->stop();
        disconnect(TimerVec[0], SIGNAL(timeout()), this, SLOT(ackTimeOut()));
    }
    SendUnACKCount=0;
    NowWaitFrameNo=0;
    socketSendFile();
}

// 显示当前实时速度
void MainWindow::speedShow()
{
    float bs=SpeedCount/1024.0*10.0;
    ui->SpeedLabel->setText(QString::number(bs) + " KB/S");
    SpeedCount=0;
}
