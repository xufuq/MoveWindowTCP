#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QHeaderView>
#include <QDateTime>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QVector>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int RSModel=0; // 默认接收
    // socket通信相关
    QTcpSocket *MySocket, *ServerSocket;
    QTcpServer *MyServer;
    QByteArray socketArray;
    QButtonGroup *ServerClientGroup, *NormalLackGroup;
    QVector<QTimer*> TimerVec; // 不能定义定时器的向量
    QVector<QByteArray> ByteVec;
    QVector<int> ConfirmVec;
    QFile *SendFile;

    QTimer *SpeedTimer;
    int SpeedCount=0;

    int ifConnected=0, hasConnected=0;
    int FrameLength=17, FrameCount=10, MaxWaitTime=10000; // 最大等待时间是10s
    int SendUnACKCount=0; // 发送但未验证的数据个数
    int LackFrameNo=100;
    uchar NowWaitFrameNo=0;
    bool IfPause=false;
    int RecStatus=0; // 当前的接受状态
    QVector<QByteArray> RecByteVec;
    QByteArray RecByte;
    void socketSendFrame();

private slots:
    void socketConnect();
    void socketDisconnect();
    void serverNewConnect();
    void socketSendFile();
    void socketRecFile();
//    void socketSetRecPath();
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    QString getLocalIP();
    void ackTimeOut();
    void pauseCall();
    void windowClear();
    void reSend();
    void speedShow();

};
#endif // MAINWINDOW_H
