#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
//#include"listform.h"
#include<addnewfriendform.h>
class TCPsocket : public QObject
{
    Q_OBJECT
public:
    static TCPsocket*gethand(){
        if(socket_hand==nullptr){
            socket_hand=new TCPsocket();
        }
        return socket_hand;
    }
    QByteArray buffer;
        int size=0;
        int flag0=0;
        int currentsize=0;
        QString FriendId;

    void out_group(QString usr_id,QString group_id_);
    void connect_server();
    void send_message(QString acceptor_id,QString sender_id,QString message);//发送聊天消息, 接受者id#发送者id#消息
    void send_id_to_get_name(QString id); //向服务器发送id已获得姓名
    void send_friend_request(QString my_id,QString friend_id);//发送好友请求
    void if_friend(QString my_id,QString friend_id,bool flag);
    void set_name(QString my_id,QString name);
    void send_info(QString mes);
    void send_group_message(QString my_id,QString mes);
    void send_picture(QString sender_id,QString receiver_id);
    void send_id_picture(QString id, QPixmap pix);



    QString getMy_name() const;
    void setMy_name(const QString &value);

    QString getMy_id() const;
    void setMy_id(const QString &value);

    QString getMy_words() const;
    void setMy_words(const QString &value);

    QList<QString> get_friends_name(){
        return friends_name;
    }
    void send_id_to_get_friends_name(QString ID);//通过ID获得好友昵称列表
    void set_friends_name(QList<QString> value);
    QList<QString> getFriends_id() const;
    void setFriends_id(QList<QString> value);
    void delete_friend(QString usr_id,QString friend_id);
    QString getInfo_id() const;
    void setInfo_id(const QString &value);

    QString getNew_friend_id() const;
    void setNew_friend_id(const QString &value);
    void get_friend_id(QString id);//发送自己id，获得好友的id列表

    void send_id_to_get_groups_id(QString ID);//-----------------------------------------新添加---------------
    void set_groups_id(QList<QString> value);//-----------------------------------------新添加---------------
    QList<QString> get_groups_id();//-----------------------------------------新添加---------------
    void send_group_new_message(QString my_id,QString Gro_id,QString mes);//————————————————————————————新添加
private:
    QString my_name="liuliuliu";//个人信息
    QString my_id="10086";
    QString my_words="helloword";
    QString new_friend_id;//新朋友的id
    QList<QString> friends_name;
    QList<QString> friends_id;//friend_id;
    QList<QString> groups_id;//群组ID-----------------------------------------------新添加-------------------
    QString info_id;//
    explicit TCPsocket(QObject *parent = nullptr);
    static TCPsocket* socket_hand;
    QTcpSocket tcpsocket;
//
private slots:
    void slot_readdata();
    void slot_register();
     void slot_addnewfriend();
signals:
    void sig_signInOK();
    void sig_add_new_friend();
    void sig_listform_open();

};




#endif // TCPSOCKET_H
