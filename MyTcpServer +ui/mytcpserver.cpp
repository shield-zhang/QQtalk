#include "mytcpserver.h"
#include "ui_mytcpserver.h"
#include "mysql.h"
#include <QBuffer>
#include <QDir>

MyTcpServer::MyTcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyTcpServer)
{
    ui->setupUi(this);
    tcp_server = new QTcpServer();
    hostInfo.setHostName(hostInfo.localHostName());//将hostinfo的name置为本机的名称
    hostInfo.lookupHost(hostInfo.localHostName(),this,SLOT(slot_get_ip_address(QHostInfo)));//利用本机名称查找ip地址

}

void MyTcpServer::slot_get_ip_address(QHostInfo info){
    ipv4_Address = info.addresses().first();//IPV4地址
    //ui->plainTextEdit->appendPlainText(ipv4_Address.toString());
    //qDebug()<<ipv4_Address.toString();
    ui->ip_lineEdit->setText(ipv4_Address.toString());
}

MyTcpServer::~MyTcpServer()
{
    delete ui;
}


void MyTcpServer::on_pushButton_clicked()
{
    ui->plainTextEdit->appendPlainText("opening server");
    //qDebug()<<"opening server";
    QString address = ipv4_Address.toString();
    QString port = ui->port_lineEdit->text();
    open_server(address, port);
}

void MyTcpServer::open_server(QString ip, QString port){//开启服务器
    ui->plainTextEdit->appendPlainText("服务器ip = " + ip + " 服务器端口 = " + port);
    //qDebug()<<"IP = "<<ip<<" port = "<<port.toUInt();
    if(tcp_server->listen(ipv4_Address,port.toInt())){      //开始监听
        ui->plainTextEdit->appendPlainText("开始监听：");
        connect(tcp_server,SIGNAL(newConnection()),this,SLOT(slot_new_connection()));//监听成功后，当有新客户端进行连接，则运行slot

        ui->pushButton->setText("Start listening");
    }
}

void MyTcpServer::slot_new_connection(){
    tcp_socket = tcp_server->nextPendingConnection(); //将新连接的客户端赋值给tcp_socket
    connect(tcp_socket,SIGNAL(readyRead()),this,SLOT(slot_get_data()));
    connect(tcp_socket,SIGNAL(disconnected()),this,SLOT(slot_disconnection()));
    int i;
    for(i = 0;i < clientList.count(); i++){
        if(tcp_socket == clientList.at(i)) break;
    }
    if(i == clientList.count()){//此连接的客户端之前未连接过
        ui->plainTextEdit->appendPlainText("一个客户端正在连接：");
        ui->listWidget->addItem("客户端IP： "+tcp_socket->peerAddress().toString()+"客户端端口： "+QString::number(tcp_socket->peerPort()));

        //qDebug()<<"a new client is connecting";
        clientList.append(tcp_socket);
        //此处连接数据库，将新连接的客户端的ip地址与端口保存
        //MySql* mysql = MySql::gethand();
        //
       // qDebug()<<"there are "<<clientList.count()<<" clients connecting";
        ui->plainTextEdit->appendPlainText("连接成功！");
    }
}

void MyTcpServer::slot_get_data(){
    QTcpSocket* sender_socket = (QTcpSocket*) sender();
    //QTcpSocket *tcpnow = (QTcpSocket*) sender();

    QList<QString>word_list;
    MySql* sqlhand = MySql::gethand();//用来调用数据库函数
    QString reciever_account;//接收者账号
    QString reciever_port;//接收者端口
    QString usr_account;
    QString friend_port;
    QString str_2;
    QString receive_port;
    QString sender_port;
    QList<QString> online_friends;//在线的好友列表
    MySql* mysql = MySql::gethand();
            qDebug()<<"getmessege";
            QByteArray databyte= sender_socket->readAll();
            if(mysql->flag0==1){
                qDebug()<<"服务器接收发送者的图片";
                mysql->size-=(databyte.size());
                qDebug()<<"now size left:"<<mysql->size;
                for(int i=0;i<clientList.count();i++){
                    if(mysql->goalport == QString::number(clientList.at(i)->peerPort())){//找到了接收者端口
                        clientList.at(i)->write(databyte);
                        qDebug()<<"服务器向接收者转发图片";
                        break;
                    }
                }
               // 目标port:this->goalport;
                //向目标传输所有消息!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                if(mysql->size==0){
                    //size等于0，说明图片传输完毕，等待接收者回复ok
                    qDebug()<<"图片转发完毕";
                    for(int i=0;i<clientList.count();i++){
                        if(mysql->goalport == QString::number(clientList.at(i)->peerPort())){//找到了接收者端口
                            //clientList.at(i)->waitForReadyRead();//等待接收者回复ok
                            for(int j=0;j<clientList.count();j++){
                                if(mysql->sendport == QString::number(clientList.at(j)->peerPort())){//找到发送者端口
                                  //  clientList.at(j)->write("ok");//给发送者发送ok
                                    break;
                                }

                            }
                            break;
                        }
                    }
                   /* waitfor 接收者  接收者会给个ok
                        给发送者ok*/
                    mysql->flag0=0;
                    qDebug()<<"flag0 turn to 0";//传输图像结束

                }
            }
            else if(mysql->flag0==0){//说明不是图片
                QString string_list=databyte;
                QList<QString> stringqlist;
                qDebug()<<string_list;
                word_list=string_list.split('#');
                QString str;    //存储要发送给客户端的文本

                qDebug()<<word_list.at(0);
                QString peerip,peerport;
                peerip=sender_socket->peerAddress().toString();
                peerport=QString::number(tcp_socket->peerPort());
                qDebug()<<"port:"<<peerport;
                 if(word_list.at(0)=="register"){
                    qDebug()<<"inregister";
                    switch (word_list.at(1).toInt()) {
                        case 1:
                                qDebug()<<"incase1";
                                if(sqlhand->insert_user_data(word_list.at(4),word_list.at(2),word_list.at(3))){
                                    str="register#1#注册成功";
                                    qDebug()<<"注册成功";

                                    qDebug()<<"send_picture 成功";
                                 /*  ///发送初始头像---------------------------------------------------
                                    QDir usr_file_dir;
                                    QStringList imglist;
                                    QString sourse_file_path = "E://xuexiao//dasanshang//jisuanjishixi//img";
                                    usr_file_dir.setPath(sourse_file_path);
                                    qDebug()<<"获得img文件夹成功";
                                    imglist << "*.jpg";//向字符串列表中添加图片类型
                                    usr_file_dir.setNameFilters(imglist);//获得文件夹下对应图片类型的名字
                                    QString img_name = sourse_file_path + "//" + usr_file_dir[0];//文件夹下第一张图片名字
                                    QImage img_touxiang(img_name);//读取头像
                                    //QString img = "img#";
                                    QPixmap pixmap = QPixmap::fromImage(img_touxiang);
                                    QByteArray touxiang;
                                    QBuffer buf(&touxiang);
                                    pixmap.save(&buf,"jpg",100);
                                    qDebug()<<"头像的size"<<touxiang.size();
                                    QString send_picture = "send_picture#"+QString::number(touxiang.size())+"#";
                                    sender_socket->write(send_picture.toUtf8());
                                    sender_socket->waitForReadyRead();
                                    sender_socket->write(touxiang);//发送图片
                                    sender_socket->waitForReadyRead();
                                    qDebug()<<"发送图片";
                                    QString fileName = QCoreApplication::applicationDirPath();//获取程序运行当前目录
                                    QString usr_file_path = fileName + "//" + word_list.at(2);//获取登录用户的文件夹路径
                                    QDir usr_file;
                                   /* usr_file.setPath(usr_file_path);
                                    if(!usr_file.exists(usr_file_path)){
                                        bool ok = usr_file.mkpath(usr_file_path);
                                        if(ok){
                                         /   QFile usr_txt_path(usr_file_path+"//data.txt");
                                            qDebug()<<usr_file_path+"//data.txt";
                                            if(usr_txt_path.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
                                                {
                                                    qDebug()<<"txt文件创建成功";
                                                }
                                            QTextStream stream(&usr_txt_path);/
                                           // qDebug()<<usr_file_path;
                                            qDebug()<<"创建用户文件夹成功！";
                                            if(QFile::copy(sourse_file_path,usr_file_path)){
                                                qDebug()<<"拷贝图片文件夹成功";
                                            }
                                        }
                                    }*/


                                }
                                else{
                                    str="register#1#注册失败";
                                }
                                sender_socket->write((str.toUtf8()));
                                qDebug()<<"send messege!";
                        break;
                        case 2: qDebug()<<"incase2";
                                if(sqlhand->select_user_data(word_list.at(2),word_list.at(3))){//2：账号
                                    str="register#2#登录成功";
                                    sqlhand->update_user_ipport(word_list.at(2),sender_socket->peerAddress().toString(),QString::number(sender_socket->peerPort()));
                                    usr_account = word_list.at(2);
                                    //usr_port = sqlhand->get_port(usr_account);
                                    stringqlist=sqlhand->get_friend_id(word_list.at(2));//得到好友列表
                                    for(int i = 0;i < stringqlist.count();i++){
                                        friend_port = sqlhand->get_port(stringqlist.at(i));//得到好友的端口
                                        if(friend_port != "zero"){//好友在线
                                            str+=("#"+stringqlist.at(i));
                                            //online_friends.append("#")
                                        }
                                        for(int j=0;j<clientList.count();j++){
                                            if(friend_port == QString::number(clientList.at(j)->peerPort())){//判断好友是否在线
                                                QString str = "online#1#" + usr_account;//online#usr_ID
                                                qDebug()<<usr_account<<" 对 "<< stringqlist.at(i)<<" 说：我在线！";
                                                clientList.at(j)->write(str.toUtf8());
                                                break;
                                            }
                                        }
                                    }

                                    /*  sqlhand->update_user_ipport(word_list.at(2),peerip,peerport);
                                    //发送图片------------------------------------------------
                                    QString fileName = QCoreApplication::applicationDirPath();//获取程序运行当前目录
                                    QString usr_file_path = fileName + "//" + word_list.at(2);//获取登录用户的文件夹路径
                                    QDir usr_file_dir;
                                    usr_file_dir.setPath(usr_file_path);
                                    bool exist = usr_file_dir.exists(usr_file_path);
                                    if(!exist){
                                        qDebug()<<"登录后找不到此用户的头像文件夹！";
                                    }
                                    else{
                                        QStringList imglist;
                                        imglist << "*.jpg";//向字符串列表中添加图片类型
                                        usr_file_dir.setNameFilters(imglist);//获得文件夹下对应图片类型的名字
                                        QString img_name = usr_file_path + "//" + usr_file_dir[0];//文件夹下第一张图片名字
                                        QImage img_touxiang(img_name);//读取头像
                                        //QString img = "img#";
                                        QPixmap pixmap = QPixmap::fromImage(img_touxiang);
                                        QByteArray touxiang;
                                        pixmap.save(touxiang,"jpg",50);
                                        sender_socket->write(touxiang);//发送图片
                                        //touxiang.append();
                                    }
                                    //QString usr_file_path = fileName + "//" + word_list.at(1);
                                    //QDir *usr_file = new QDir;*/
                                }
                                else{
                                    str="register#2#登录失败";
                                }
                                sender_socket->write((str.toUtf8()));
                                qDebug()<<"send messege!";
                         break;
                    case 3:qDebug()<<"in case 3";
                        str="register#3";
                        usr_account = word_list.at(2);
                        //usr_port = sqlhand->get_port(usr_account);
                        stringqlist=sqlhand->get_friend_id(word_list.at(2));//得到好友列表
                        for(int i = 0;i < stringqlist.count();i++){
                            friend_port = sqlhand->get_port(stringqlist.at(i));//得到好友的端口
                            if(friend_port != "zero"){//好友在线
                                str+=("#"+stringqlist.at(i));
                                //online_friends.append("#")
                            }

                        }
                        sender_socket->write((str.toUtf8()));
                        qDebug()<<"send messege!";
                    }
                }
                else if(word_list.at(0)=="friendset_basic"){//添加好友------------------------------------------
                                qDebug()<<"in friendset_basic";


                                switch (word_list.at(1).toInt()) {
                                    case 1: qDebug()<<"incase1";//获得昵称
                                            string_list=sqlhand->get_name_byid(word_list.at(2));
                                            if(string_list=="NULL"){
                                                str="friendset_basic#1#nonamefound";
                                            }
                                            else{
                                                str+=("#"+word_list.at(2));
                                                str="friendset_basic#1#"+string_list+"#"+word_list.at(2);
                                            }
                                            sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";
                                            break;
                                    case 2://获得好友ID列表
                                            qDebug()<<"incase2";
                                            stringqlist=sqlhand->get_friend_id(word_list.at(2));
                                            str="friendset_basic#2";
                                            for(int i=0;i<stringqlist.count();i++){
                                                str+=("#"+stringqlist.at(i));
                                            }

                                            sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";
                                            break;
                                    case 3: qDebug()<<"incase3";
                                            if(sqlhand->id_exist(word_list.at(3))){
                                                if(sqlhand->friend_exist(word_list.at(2),word_list.at(3))){//2：发送者 3：好友
                                                    qDebug()<<"hhhhhhhhhhhhhh";
                                                    str="friendset_basic#3#好友已存在";
                                                }
                                                else{
                                                    //向好友发送添加好友请求
                                                    //reciever_account = word_list.at(3);
                                                    reciever_port = mysql->get_port(word_list.at(3));
                                                    qDebug()<<"qqqqqqqqqqqqqqqqqqq";
                                                    int i=0;
                                                    for( i=0;i<clientList.count();i++){
                                                        qDebug()<<"tttttttttttttttttttttt";
                                                        qDebug()<<QString::number(clientList.at(i)->peerPort())<<"  11";
                                                        qDebug()<<reciever_port<<"  22";
                                                        if(QString::number(clientList.at(i)->peerPort())==reciever_port){
                                                            qDebug()<<"好友请求已发送";
                                                            QString str_0 = "friendset_basic#6#"+word_list.at(2);//firendset_basic#6#发送者
                                                            QByteArray str_1 = str_0.toUtf8();//这里将发送者的账号保留了下来共同发送给接受者，用于接受者进行窗口识别。接受者需要再去除掉发送者的账号来显示消息
                                                            clientList.at(i)->write(str_1);
                                                            str="friendset_basic#3#好友请求已发送";
                                                            break;
                                                        }
                                                    }
                                                    if(i==clientList.count()){
                                                        qDebug()<<"好友不在线";
                                                        str="friendset_basic#3#好友不在线";
                                                    }
                                                }
                                            }
                                            else{
                                                str="friendset_basic#3#该用户不存在";
                                            }
                                            sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";
                                            break;
                                    case 4: qDebug()<<"incase4";//好友请求的结果 服务器收到的消息为：firendset_basic#4#usr_id#friend_id#同意/不同意
                                            if(word_list.at(4)=="同意"){
                                                sqlhand->insert_friend_data(word_list.at(2),word_list.at(3));
                                                str_2="friendset_basic#4#好友添加成功";
                                            }
                                            else{
                                                qDebug()<<"好友添加失败，少侠请重新来过";
                                                str_2="friendset_basic#4#好友添加失败";
                                            }
                                            reciever_port = mysql->get_port(word_list.at(2));//reciever_port为usr
                                            for(int i=0;i<clientList.count();i++){
                                                if(QString::number(clientList.at(i)->peerPort())==reciever_port){
                                                    //QString str_0 = "friendset_basic#4#好友添加成功";//firendset_basic#发送者
                                                    QByteArray str = str_2.toUtf8();//这里将发送者的账号保留了下来共同发送给接受者，用于接受者进行窗口识别。接受者需要再去除掉发送者的账号来显示消息
                                                    clientList.at(i)->write(str);
                                                   // qDebug()<<"好友请求已发送";
                                                    break;
                                                }
                                            }
                                            /*sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";*/
                                            break;
                                    case 5: qDebug()<<"incase5";
                                   // bool update_user_nname(QString N_name,QString ID);
                                            if(sqlhand->update_user_nname(word_list.at(2),word_list.at(3))){
                                                str="friendset_basic#5#设置昵称成功";
                                            }
                                            else{
                                                str="friendset_basic#5#设置昵称失败";
                                            }
                                            sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";
                                            break;
                                   case 7:qDebug()<<"in case7";
                                            if(sqlhand->delete_friend(word_list.at(2),word_list.at(3))){
                                                str="friendset_basic#7#删除好友成功";
                                            }
                                            else{
                                                str="friendset_basic#7#删除好友失败";
                                            }
                                            sender_socket->write((str.toUtf8()));
                                            qDebug()<<"send messege!";
                                            break;
                                    case 8:qDebug()<<"in case8";//根据ID获得好友昵称列表
                                            stringqlist=mysql->get_friend_name(word_list.at(2));
                                            str = "friendset_basic#8";
                                            for(int i=0;i<stringqlist.count();i++){
                                                str+=("#"+stringqlist.at(i));
                                            }
                                            sender_socket->write(str.toUtf8());

                                }

                }
                else if(word_list.at(0)=="groupmsg"){
                                qDebug()<<"in group";
                                switch (word_list.at(1).toInt()) {
                                case 1:qDebug()<<"in case1";
                                    if(sqlhand->insert_group_data(word_list.at(2),word_list.at(3))){
                                        str="groupmsg#1#群组创建成功";
                                    }
                                    else{
                                        str="groupmsg#1#群组创建失败";
                                    }
                                    sender_socket->write((str.toUtf8()));
                                    qDebug()<<"send messege!";
                                    break;
                                case 2:qDebug()<<"in case 2";//查询群组成员id
                                    stringqlist=sqlhand->get_groupmember_id(word_list.at(2));
                                    str="groupmsg#2";
                                    for(int i=0;i<stringqlist.count();i++){
                                        str+=("#"+stringqlist.at(i));
                                    }

                                    sender_socket->write((str.toUtf8()));
                                    qDebug()<<"send messege!";
                                    break;
                                case 3:qDebug()<<"in case 3";
                                    // QString receive_port;
                                   //  QString sender_port;
                                     sender_port = sqlhand->get_port(word_list.at(2));//获取发送者端口
                                     //stringqlist=sqlhand->get_groupmember_id(word_list.at(2));//获取群成员
                                     for(int i=0;i<clientList.count();i++){//遍历所有在线人员
                                         receive_port = QString::number(clientList.at(i)->peerPort());//每一个在线人员的端口
                                         if(receive_port!=sender_port){//若成员在线且不是发送者
                                                    qDebug()<<str+"发送的消息就是这个！";
                                                     str=string_list;//群发消息
                                                     clientList.at(i)->write(str.toUtf8());
                                                 }

                                         //str_2="groupmsg#3#群发成功";
                                         sender_socket->write(str_2.toUtf8());

                                     }
                                     break;
                                case 4://群发消息 groupmsg#4#群号#发送者#消息#发送者昵称
                                   // QString receive_port;
                                  //  QString sender_port;
                                    qDebug()<<"in case 4";
                                    sender_port = sqlhand->get_port(word_list.at(3));//获取发送者端口
                                    stringqlist=sqlhand->get_groupmember_id(word_list.at(2));//获取群成员
                                    for(int i=0;i<stringqlist.count();i++){
                                        receive_port = sqlhand->get_port(stringqlist.at(i));//每一个群成员的端口
                                        if(receive_port!="zero" && receive_port!=sender_port){//若成员在线且不是发送者
                                            qDebug()<<"in if";
                                            qDebug()<<"clientlist.cout():"<<clientList.count();
                                            for(int j=0;j<clientList.count();j++){
                                              //  qDebug()<<"receive_port = "<<receive_port<<" client.j="<<
                                                if(QString::number(clientList.at(j)->peerPort()) == receive_port){//找到了在线的群成员
                                                    QString str=string_list;//群发消息
                                                    str+="#";
                                                    qDebug()<<str<<"发送群消息";
                                                    clientList.at(j)->write(str.toUtf8());
                                                }
                                            }
                                        }

                                    }
                                    str_2="groupmsg#4#群发成功";
                                   // sender_socket->write(str_2.toUtf8());
                                    break;
                                 case 5://groupmsg#5#群号#..每个成员
                                qDebug()<<"incase 5";
                                  for(int i=3;i<word_list.count();i++){
                                        sqlhand->insert_group_data(word_list.at(2),word_list.at(i));
                                  }
                                  str="groupmsg#5#群组编辑成功";
                                  sender_socket->write((str.toUtf8()));
                                  qDebug()<<"send messege!";
                                  break;
                                case 6:qDebug()<<"incase 6";//通过id获取群组列表  groupmsg#6#usr_id#..群组列表
                                     stringqlist=sqlhand->get_groupid_byuser(word_list.at(2));
                                     str="groupmsg#6#"+word_list.at(2);
                                     qDebug()<<"stringqlist.count():"<<stringqlist.count();
                                     for(int i=0;i<stringqlist.count();i++){
                                         qDebug()<<"第"<<i<<"个 群组是 ："<<stringqlist.at(i);
                                           str+=("#"+stringqlist.at(i));
                                     }

                                     sender_socket->write((str.toUtf8()));
                                     qDebug()<<"send messege!";
                                     break;
                                case 7:qDebug()<<"in case7";
                                    if(sqlhand->delete_group(word_list.at(2),word_list.at(3))){
                                        str="groupmsg#7#退群成功";
                                    }
                                    else{
                                        str="groupmsg#7#退群失败";
                                    }
                                    sender_socket->write((str.toUtf8()));
                                    qDebug()<<"send messege!";
                                    break;

                                }

                }
                else if(word_list.at(0)=="signature"){
                                qDebug()<<"in signature";
                                switch (word_list.at(1).toInt()) {
                                case 1:qDebug()<<"in case1";//更新签名  signature#1#用户ID#更新内容
                                    if(sqlhand->update_sign_data(word_list.at(2),word_list.at(3))){
                                        str="signature#1#个性签名设置成功";
                                    }
                                    else{
                                        str="signature#1#个性签名设置失败";
                                    }
                                    sender_socket->write((str.toUtf8()));
                                    qDebug()<<"send messege!";
                                    break;
                                case 2:qDebug()<<"in case 2";//读取签名 signature#2#用户ID
                                    string_list=sqlhand->get_sign_data(word_list.at(2));
                                    str="signature#2#"+string_list;//signature#2#签名内容


                                    sender_socket->write((str.toUtf8()));
                                    qDebug()<<"send messege!";
                                    break;

                                }

                }
                else if(word_list.at(0)=="del"){//删除好友 del#发送人#被删掉的人
                    QString sender = word_list.at(1);
                    QString del_man = word_list.at(2);
                    //sqlhand
                }
                else if(word_list.at(0) == "msg"){
                            //QString data = sender_socket->readLine();
                            QList<QString> data_list = word_list;//msg#接收者账号#发送者账号#消息#发送者昵称
                            reciever_account = data_list.at(1);//即接收者的账号
                            qDebug()<<"接收者账号："<<reciever_account;
                            //此处要查询数据库，以判断是否在线---------------------------------

                            reciever_port = mysql->get_port(reciever_account);//根据账号得到端口号

                            for(int i=0;i<clientList.count();i++){
                                if(QString::number(clientList.at(i)->peerPort())==reciever_port){
                                    QString str_0 = "msg#"+data_list.at(1)+"#"+data_list.at(2)+"#"+data_list.at(3)+"#"+data_list.at(4);
                                    QByteArray str = str_0.toUtf8();//这里将发送者的账号保留了下来共同发送给接受者，用于接受者进行窗口识别。接受者需要再去除掉发送者的账号来显示消息
                                    clientList.at(i)->write(str);
                                    qDebug()<<"接收者端口号： "<<clientList.at(i)->peerPort();
                                    break;
                                }
                            }
                }

            else if(word_list.at(0)=="send_picture"){//send_picture#size#发送者ID#接收者ID
                qDebug()<<"in send_picture";
                mysql->size=word_list.at(1).toInt();
                qDebug()<<"size:"<<mysql->size;
                mysql->sendport=mysql->get_port(word_list.at(2));//获取图片发送者端口
                mysql->goalport=mysql->get_port(word_list.at(3));//获取图片接收者端口
                //this->goal_port=.....//获取发送者port
                //this->sender_port//获取接收者port
                mysql->flag0=1;
                qDebug()<<"flag0 turn to 1";//开启传输图片模式

                for(int i=0;i<clientList.count();i++){//向接收者发送size——————————————————————
                    if(QString::number(clientList.at(i)->peerPort()) == mysql->goalport){//找到接收者
                        qDebug()<<"找到了图片接收者";
                        QString str_= "send_picture#"+word_list.at(1)+"#"+word_list.at(2);//send_picture#size#发送者ID
                        clientList.at(i)->write(str_.toUtf8());//发送给接收者
                        qDebug()<<"给接收者发送size";
                        //break;
                       // clientList.at(i)->waitForReadyRead(5000);//等待接收者收到消息
                        qDebug()<<"接收者收到了size";
                      //  sender_socket->write("ok");//告诉图片发送者服务器开始接收图片
                        qDebug()<<"服务器开始接收图片";
                        sender_socket->waitForBytesWritten();//发送者收到了服务器的消息
                        qDebug()<<"发送者开始发图片 ";
                        //开始接收图片
                        /*QDir usr_file_dir;
                        QStringList imglist;
                        QString sourse_file_path = "E://xuexiao//dasanshang//jisuanjishixi//img";
                        usr_file_dir.setPath(sourse_file_path);
                        qDebug()<<"获得img文件夹成功";
                        imglist << "*.jpg";//向字符串列表中添加图片类型
                        usr_file_dir.setNameFilters(imglist);//获得文件夹下对应图片类型的名字
                        QString img_name = sourse_file_path + "//" + usr_file_dir[0];//文件夹下第一张图片名字
                        QImage img_touxiang(img_name);//读取头像
                        //QString img = "img#";
                        QPixmap pixmap = QPixmap::fromImage(img_touxiang);
                        QByteArray touxiang;
                        QBuffer buf(&touxiang);
                        pixmap.save(&buf,"jpg",100);
                        qDebug()<<"头像的size"<<touxiang.size();
                        QString send_picture = "send_picture#"+QString::number(touxiang.size())+"#";
                        //sender_socket->write(send_picture.toUtf8());
                        //sender_socket->waitForReadyRead();
                        clientList.at(i)->write(touxiang);//发送图片
                        clientList.at(i)->waitForReadyRead();//等待接收者接收完图片
                        sender_socket->write("ok");//告诉发送者，接收者已经接收完图片*/
                        break;
                    }
                }

                //qDebug()<<"发送图片";
//                发送者-》服务器  size
//                发送者 waitfor 服务器
//                 服务器接收size
//                 服务器-》接收者 size
//                 服务器 waitfor 接收者readyread
//                  接收者接收size;
//                接收者-》服务器 ok；  xxxxxxxxxx
//                  接收者 waitfor 服务器
//                  服务器-》发送者 ok   xxxxxxxxx
//                     服务器 waitfor 发送者
//                    发送者-》服务器 图片
//                    发送者 waitfor 服务器//
//                   服务器-》接收者 图片
//                      服务器waitfor 接收者
//                        接收者 收完图片 -> 服务器 ok; xxxxxxxxxx
//                服务器-》发送者 ok  xxxxxxxxxxxxx


            }
        }
//            else{//说明传输的是图片
//               // else if(word_list.at(0) == "img"){//"img#账号#图片"
//                    QByteArray jpgArr = databyte;
//                    //jpgArr.append(word_list.at(2));
//                    QImage img;
//                    img.loadFromData(jpgArr,"JPG");//从jpgArr读取数据
//                    QPixmap pixmap;
//                    pixmap.convertFromImage(img);//img转为pixmap
//                    QString fileName = QCoreApplication::applicationDirPath();//获取程序运行当前目录
//                    QString usr_file_path = fileName + "//" + word_list.at(1);
//                    QDir usr_file;
//                    usr_file.setPath(usr_file_path);
//                    //保存图片----------------------------------------------------
//                    bool exist = usr_file.exists(usr_file_path);
//                    if(exist){
//                       /* QFile usr_txt_path(usr_file_path+"//data.txt");
//                        qDebug()<<usr_file_path+"//data.txt";
//                        if(usr_txt_path.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){
//                            qDebug()<<"txt文件打开成功";
//                        }
//                        QTextStream stream(&usr_txt_path);*/
//                        //写入
//                        usr_file.remove(usr_file[0]);
//                        qDebug()<<"保存头像成功！";
//                        pixmap.save(usr_file_path+"//touxiang","JPG");
//                       // usr_txt_path.close();
//                    }
//                    else{
//                        bool ok = usr_file.mkpath(usr_file_path);
//                        if(ok){
//                         /*   QFile usr_txt_path(usr_file_path+"//data.txt");
//                            qDebug()<<usr_file_path+"//data.txt";
//                            if(usr_txt_path.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate))
//                                {
//                                    qDebug()<<"txt文件创建成功";
//                                }
//                            QTextStream stream(&usr_txt_path);*/
//                            qDebug()<<"创建用户文件夹成功！";
//                            pixmap.save(usr_file_path+"//touxiang","JPG");
//                           // usr_txt_path.close();
//                        }
//                    }
//                    //发送图片----------------------------------------


//            }



/*    while(sender_socket->canReadLine()){
        QString data = sender_socket->readLine();
        QString reciever_account = data.mid(0,8);//截取前八位，即接收者的账号
        qDebug()<<"接收者账号："<<reciever_account;
        QString reciever_port;//此处要查询数据库，以判断是否在线---------------------------------
        MySql* mysql = MySql::gethand();
        //reciever_port = mysql->get_port(reciever_account);//根据账号得到端口号

        for(int i=0;i<clientList.count();i++){
            if(QString::number(clientList.at(i)->peerPort())==reciever_port){
                QByteArray str = data.mid(8).toUtf8();//这里将发送者的账号保留了下来共同发送给接受者，用于接受者进行窗口识别。接受者需要再去除掉发送者的账号来显示消息
                clientList.at(i)->write(str);
                qDebug()<<"接收者端口号： "<<clientList.at(i)->peerPort();
                break;
            }
        }*/
}

void MyTcpServer::slot_disconnection(){
    QTcpSocket* sender_socket = (QTcpSocket*) sender();//得到发送disconnected信号的客户端
    MySql* mysql = MySql::gethand();
    QString sender_id = mysql->get_id_byport(QString::number(sender_socket->peerPort()));
    //qDebug()<<
    qDebug()<<"掉线者的id："<<sender_id;
    QString friend_port;
    QList<QString> outline_friend = mysql->get_friend_id(sender_id);//通过id获得好友列表
    qDebug()<<"好友列表有这么多人："<<outline_friend.count();
    QString str;
    for(int i=0;i<outline_friend.count();i++){
        friend_port = mysql->get_port(outline_friend.at(i));//通过好友ID查到好友端口
        for(int j=0;j<clientList.count();j++){
            if(friend_port == QString::number(clientList.at(j)->peerPort())){//通过端口查到好友客户端
                str = "online#2#"+sender_id;//outline#掉线者ID
                qDebug()<<sender_id << " 对端口号为 " <<outline_friend.at(i) << " 说：我掉线了！";
                clientList.at(j)->write(str.toUtf8());
            }
        }
    }
    for(int i=0;i<clientList.count();i++){
        if(sender_socket == clientList.at(i)){
            ui->plainTextEdit->appendPlainText("一个客户端断开连接，端口号："+QString::number(sender_socket->peerPort()));
            ui->listWidget->takeItem(i);
            clientList.removeAt(i);//这是链表，删除后长度自动减少
            mysql->set_ipport_zero(QString::number(sender_socket->peerPort()));
            break;
        }

    }



}
