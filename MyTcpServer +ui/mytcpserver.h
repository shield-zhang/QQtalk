#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostInfo>
QT_BEGIN_NAMESPACE
namespace Ui { class MyTcpServer; }
QT_END_NAMESPACE

class MyTcpServer : public QWidget
{
    Q_OBJECT

public:
    MyTcpServer(QWidget *parent = nullptr);
    void open_server(QString, QString);
    ~MyTcpServer();

private slots:
    void slot_get_ip_address(QHostInfo);
    void on_pushButton_clicked();
    void slot_new_connection();
    void slot_get_data();
    void slot_disconnection();
    //int flag0=0;//判断是否是图片转发模式；
    //int size=0;//判断传输图片是否结束
    //QString sender_port;
    //QString goal_port;
private:
    QHostInfo hostInfo;
    QTcpServer *tcp_server;
    QTcpSocket* tcp_socket;
    QList<QTcpSocket*> clientList;
    QHostAddress ipv4_Address;
    Ui::MyTcpServer *ui;
};
#endif // MYTCPSERVER_H
