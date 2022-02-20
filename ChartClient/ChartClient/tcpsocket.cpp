#include "tcpsocket.h"
#include<QMessageBox>
#include "login.h"
#include "register.h"
#include<listform.h>
#include <addnewfriendform.h>
#include <QDateTime>
#include <QDir>
#include <QBuffer>
#include "qfiledialog.h"
TCPsocket* TCPsocket::socket_hand=nullptr;
TCPsocket::TCPsocket(QObject *parent) : QObject(parent)
{
}

void TCPsocket::connect_server(){
    tcpsocket.connectToHost("10.194.142.2",7788);
   // connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(slot_readdata()));//
    connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT( slot_register()));//
    connect(&tcpsocket,SIGNAL(sig_add_new_friend()),this,SLOT(slot_addnewfriend()));//
}

void TCPsocket::send_group_new_message(QString my_id,QString Gro_id,QString mes)//————————————————————————————新加——————————————————————————
{ QString str = "groupmsg#4#"+Gro_id+"#"+my_id+"#"+mes+"#"+my_name;
    tcpsocket.write(str.toUtf8());
    //tcpsocket.waitForReadyRead();
}

void TCPsocket::out_group(QString usr_id,QString group_id_){///__________________________________________________
    QString str = "groupmsg#7#"+usr_id+"#"+group_id_;
    tcpsocket.write(str.toUtf8());
}


void TCPsocket::send_id_to_get_groups_id(QString ID){//————————————————————————————新加——————————————————————————
    QString temp = "groupmsg#6#"+ID;
    tcpsocket.write(temp.toUtf8());
    tcpsocket.waitForReadyRead();
}
void TCPsocket::set_groups_id(QList<QString> value){//-----------------------------------------新添加---------------
    qDebug()<<"在set_groups_id函数中！";
    groups_id = value;

}
QList<QString> TCPsocket::get_groups_id(){ //-----------------------------------------新添加---------------
    qDebug()<<"在get_groups_id中"<<" "<<groups_id.count();

    return groups_id;
}

/*_________________________________________________________________________________________________________________________________________*/


void TCPsocket::send_picture(QString sender_id,QString receiver_id){
    //TCPsocket *sockethand = TCPsocket::gethand();
    ///发送初始头像---------------------------------------------------
    QDir usr_file_dir;
    QStringList imglist;
   // QString sourse_file_path = "E://xuexiao//dasanshang//jisuanjishixi//img";//源文件路径
    //usr_file_dir.setPath(sourse_file_path);
    qDebug()<<"获得img文件夹成功";
    imglist << "*.jpg";//向字符串列表中添加图片类型
    usr_file_dir.setNameFilters(imglist);//获得文件夹下对应图片类型的名字
    //QString img_name = sourse_file_path + "//" + img;//文件夹下第一张图片名字
    QString img_name;
    img_name = QFileDialog::getOpenFileName(NULL, tr("open file"), " ",  tr("Allfile(*.*);;mp3file(*.cpp)"));
    qDebug()<<img_name;


    QImage img_touxiang(img_name);//读取头像
    //QString img = "img#";
    QPixmap pixmap = QPixmap::fromImage(img_touxiang);
    QByteArray touxiang;
    QBuffer buf(&touxiang);
    pixmap.save(&buf,"jpg",100);
    qDebug()<<"头像的size"<<touxiang.size();
    QString send_picture = "send_picture#"+QString::number(touxiang.size())+"#"+sender_id+"#"+receiver_id;//send_picture#size#发送者ID#接收者ID
    tcpsocket.write(send_picture.toUtf8());
    tcpsocket.waitForReadyRead(2000);
    tcpsocket.write(touxiang);//发送图片
    tcpsocket.waitForReadyRead();
    qDebug()<<"发送图片";
    //QString fileName = QCoreApplication::applicationDirPath();//获取程序运行当前目录
   // QString usr_file_path = fileName + "//" + word_list.at(2);//获取登录用户的文件夹路径
   // QDir usr_file;

}

void TCPsocket::send_id_picture(QString id, QPixmap pix)
{
    ListForm* listform=ListForm::gethand();
    for(int i=0;i<listform->p2plist.count();i++){
        if(id==listform->p2plist[i]->getP2p_friend_id()){
            listform->p2plist[i]->set_image(pix);
        }
    }
}

/*_________________________________________________________________________________________________________________________*/


void TCPsocket::send_message(QString acceptor_id,QString sender_id,QString mes){
    //发送聊天消息, 接受者id#发送者id#消息
    QString temp="msg#"+acceptor_id+"#"+sender_id+"#"+mes+"#"+my_name;
    tcpsocket.write(temp.toUtf8());
}

void TCPsocket::send_id_to_get_name(QString id){
    //发送id
    qDebug()<<"这里发送id";
    QString temp="friendset_basic#1#"+id;
    tcpsocket.write(temp.toUtf8());
    tcpsocket.waitForReadyRead();//这行新添加的-----------------------------------
}

void TCPsocket::set_friends_name(QList<QString> value)
{
    friends_name = value;
}

void TCPsocket::send_id_to_get_friends_name(QString ID){
    QString temp = "friendset_basic#8#"+ID;
    tcpsocket.write(temp.toUtf8());
    tcpsocket.waitForReadyRead();
}

QString TCPsocket::getMy_name() const
{
    return my_name;
}

void TCPsocket::setMy_name(const QString &value)
{
    my_name = value;
}

QString TCPsocket::getMy_id() const
{
    return my_id;
}

void TCPsocket::setMy_id(const QString &value)
{
    my_id = value;
}

QString TCPsocket::getMy_words() const
{
    return my_words;
}

void TCPsocket::setMy_words(const QString &value)
{
    my_words = value;
}

QList<QString> TCPsocket::getFriends_id() const
{
    return friends_id;
}

void TCPsocket::setFriends_id(QList<QString> value)
{
    friends_id = value;
}

QString TCPsocket::getInfo_id() const
{
    return info_id;
}

void TCPsocket::setInfo_id(const QString &value)
{
    info_id = value;
}

QString TCPsocket::getNew_friend_id() const
{
    return new_friend_id;
}

void TCPsocket::setNew_friend_id(const QString &value)
{
    new_friend_id = value;
}
void TCPsocket::if_friend(QString my_id, QString friend_id,bool flag)
{
    QString temp;
    if(flag)
    temp="friendset_basic#4#"+my_id+"#"+friend_id+"#同意";
    else{
         temp="friendset_basic#4#"+my_id+"#"+friend_id+"#不同意";
    }
    tcpsocket.write(temp.toUtf8());
}

void TCPsocket::set_name(QString my_id, QString name)
{
    QString temp="friendset_basic#5#"+name+"#"+my_id;
    tcpsocket.write(temp.toUtf8());
}


void TCPsocket::send_friend_request(QString my_id, QString friend_id){
    QString temp="friendset_basic#3#"+my_id+"#"+friend_id;
    tcpsocket.write(temp.toUtf8());
}

void TCPsocket::slot_addnewfriend()
{

}
void TCPsocket::slot_readdata(){

    QString str= tcpsocket.readAll();

    QList<QString>strlist =str.split('#');
}
void TCPsocket::send_info(QString mes){

      tcpsocket.write(mes.toUtf8());
      connect(&tcpsocket,SIGNAL(readyRead()),this,SLOT(slot_register()));//这行谨慎删除！！！！-----------------
}

void TCPsocket::send_group_message(QString my_id, QString mes)
{
    QString str = "groupmsg#3#"+my_id+"#"+mes+"#"+my_name;
    tcpsocket.write(str.toUtf8());
}

void TCPsocket::delete_friend(QString usr_id, QString friend_id){
    QString str = "friendset_basic#7#"+usr_id+"#"+friend_id;
    tcpsocket.write(str.toUtf8());
}

void TCPsocket::get_friend_id(QString id){
    qDebug()<<"qqqqqqqqqqqqqqqqqqqqqq";
    QString str = "friendset_basic#2#"+id;
    tcpsocket.write(str.toUtf8());
    tcpsocket.waitForReadyRead();//这行新添加-----------------------------------------------------
}




void TCPsocket::slot_register(){
    QByteArray temp;
    if(this->flag0==1){
        temp=tcpsocket.readAll();
        buffer+=temp;
        qDebug()<<"now the buffer size:"<<buffer.size();
       if(buffer.size()==this->size){

           this->flag0=2;//图片读取完毕，进行图片设置
          // tcpsocket.write("ok");//向服务器发送ok,告知服务器接收端接收图片完毕
           qDebug()<<"flag0 turn to 2";
       }
    }
    else{
        temp=tcpsocket.readAll();
        //buffer=temp;
    }
    qDebug()<<"slot_register";

    //qDebug()<<"bytesize:       "<<buffer.size();
    QString str=QString(temp);

//QString str = tcpsocket.readAll();
qDebug()<<str<<"  yesssQQQQQ!!!!!";
QList<QString>strlist =str.split('#');


   // qDebug()<<strlist.at(0);

if(strlist.at(0)=="register"){
    qDebug()<<"inregister";
    ListForm* listform = ListForm::gethand();
    switch (strlist.at(1).toInt()) {
        case 1:  qDebug()<<"incase1";
                    QMessageBox::information(NULL,strlist.at(2),strlist.at(2),QMessageBox::Ok);
                if(strlist.at(2)=="注册成功"){//注册成功时要干的事
                    Register* reg=Register::gethand();
                    reg->close();
                 }
                 break;
        case 2:qDebug()<<"incase2";
            QMessageBox::information(NULL,strlist.at(2),strlist.at(2),QMessageBox::Ok);
            if(strlist.at(2)=="登录成功"){//登录成功时要干的事

                listform->show();

                for(int i=3;i<strlist.count();i++){
                    listform->friend_on_color(strlist.at(i));
                }
                /*ListForm* listform = new ListForm();
                listform->show();
                extern ListForm* listform;
                listform = new ListForm();
                //listform->init();
                listform->show();*/
                //emit sig_listform_open();
                qDebug()<<"hhhhhhhhhhhhhhhhhhhhhhh";
             Login* log=Login::gethand();
             log->flag=1;
            log->close();

            }
                break;
    case 3:qDebug()<<"in case3";
        for(int i=2;i<strlist.count();i++){
            listform->friend_on_color(strlist.at(i));
        }
    }
}
   else  if(strlist.at(0)=="friendset_basic"){
            qDebug()<<"infriendset_basic";
            switch (strlist.at(1).toInt()){
            case 1: qDebug()<<"incase 1";
                    if(strlist.at(2)=="nonamefound"){ //未查到昵称

                    }
                    else{ //查昵称成功    N_name为strlist.at(2)  （QString类型）
                        if(strlist.at(3)!=socket_hand->getMy_id())
                        socket_hand->friends_name.append(strlist.at(2) );
                        else{
                            socket_hand->setMy_name(strlist.at(2));
                            qDebug()<<"我叫 "<<strlist.at(2);
                            //socket_hand->getMy_name();
                        }
                    }
                    break;
            case 2:qDebug()<<"incase 2";

                    //得到好友列表  好友id为 strlist.at(i)  i=2~(strlist.count()-1)   共strlist.count()-2个
                    if(1){
                        QList<QString> temp=strlist;
                        temp.removeFirst();
                        temp.removeFirst();
                        qDebug()<<temp.count();
                        socket_hand->setFriends_id(temp);
                        qDebug()<<"————————————————————————————————————————————————";
                    }

                    break;
            case 3:qDebug()<<"incase 3";
                    if(strlist.at(2)=="好友已存在"){ //所查id已经是当前用户好友
                        QMessageBox::information(NULL,"好友已经存在","好友已经存在",QMessageBox::Ok);
                    }
                    else if(strlist.at(2)=="您与该用户还不是好友"){//所查id存在，但不是好友
                        QMessageBox::information(NULL,"已向该用户发送好友请求","已向该用户发送好友请求",QMessageBox::Ok);
                    }
                    else if(strlist.at(2)=="该用户不存在"){//所查id不存在
                       QMessageBox::information(NULL,"该用户不存在","该用户不存在",QMessageBox::Ok);
                    }
                    else if(strlist.at(2)=="好友请求已发送"){//所查id不存在
                        QMessageBox::information(NULL,"好友请求已发送","好友请求已发送",QMessageBox::Ok);
                     }
                    break;
            case 4:qDebug()<<"incase 4";
                    if(strlist.at(2)=="好友添加成功"){//好友添加成功   可用 QMessageBox::information（）调用弹窗显示
                         QMessageBox::information(NULL,"好友添加成功","好友添加成功",QMessageBox::Ok);
                    }
                    else{//好友添加失败
                        QMessageBox::information(NULL,"好友添加失败","对方拒绝了你的请求",QMessageBox::Ok);
                    }
                    break;
            case 5:qDebug()<<"incase 5";
                    if(strlist.at(2)=="设置昵称成功"){    //设置昵称成功
                         QMessageBox::information(NULL,"设置昵称成功","设置昵称成功",QMessageBox::Ok);
                    }
                    else{//设置昵称失败（一般不会）
                         QMessageBox::information(NULL,"昵称炸了","设置失败",QMessageBox::Ok);
                    }
                    break;
           case 6:qDebug()<<"incase 6";
            {
                socket_hand->new_friend_id=strlist.at(2);
                AddNewFriendForm* add_new_friend_form = new AddNewFriendForm();
                add_new_friend_form->setLableText(1,"用户"+strlist.at(2)+"申请添加您为好友");
                add_new_friend_form->show();
            }
                break;

            case 7:
                if(strlist.at(2)=="删除好友成功"){
                     QMessageBox::information(NULL,"删除好友成功","删除好友成功",QMessageBox::Ok);

                }
                else{
                     QMessageBox::information(NULL,"删除好友失败","删除好友失败",QMessageBox::Ok);
                }
                break;
            case 8:
                qDebug()<<"in case8";
                QList<QString> temp = strlist;
                temp.removeFirst();
                temp.removeFirst();
                socket_hand->set_friends_name(temp);
                break;
          }

    }
    else if(strlist.at(0)=="msg"){
        //弹窗显示消息，最low测试
    ListForm *listui=ListForm::gethand();

    listui->friend_red(strlist.at(2));
//        QMessageBox::information(NULL,strlist.at(2)+"information:",strlist.at(3),QMessageBox::Ok);

    //    QMessageBox::information(NULL,strlist.at(2)+"发来了消息",strlist.at(3),QMessageBox::Ok);
        ListForm *listform = ListForm::gethand();
         qDebug()<<"msg：p2plist.count："<<listform->p2plist.count();
        for (int i=0;i<listform->p2plist.count();i++) {
            if(listform->p2plist.at(i)->getP2p_friend_id()==strlist.at(2))
            {
               qDebug()<<"进入接收if内部"<<listform->p2plist.count();
               QDateTime current_date_time =QDateTime::currentDateTime();
               QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
               listform->p2plist.at(i)->getUi()->listWidget->addItem(current_date+"   昵称:"+strlist.at(4)+"   id:"+strlist.at(2));


//               //新添加内容如下↓
//               QColor color_green;
//               color_green.setRgb(0,128,0);
//               int index=listform->p2plist.at(i)->getUi()->listWidget->count();
//               listform->p2plist.at(i)->getUi()->listWidget->item(index-1)->setTextColor(color_green);


               listform->p2plist.at(i)->getUi()->listWidget->addItem(strlist.at(3));
               listform->p2plist.at(i)->getUi()->listWidget->addItem(" ");

            }
        }
    }
    else if(strlist.at(0)=="online"){
    qDebug()<<"in online";
    ListForm* listform = ListForm::gethand();
    switch (strlist.at(1).toInt()) {
        case 1:
         qDebug()<<"in case1";

          listform->friend_on_color(strlist.at(2));
          break;

       case 2:

            qDebug()<<"in case 2";

            listform->friend_off_color(strlist.at(2));
            break;


    }
    }
    else if(strlist.at(0)=="groupmsg"){//groupmsg#sender_id#mes

    ListForm *listform = ListForm::gethand();

    QList<QString>temp;
    QDateTime current_date_time =QDateTime::currentDateTime();
           QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    switch (strlist.at(1).toInt()) {
    case 3:
    { listform->getUi()->listWidget_2->addItem(current_date+"  昵称:"+strlist.at(4)+"  id:"+strlist.at(2));

     //新添加内容如下↓
//     QColor color_green;
//     color_green.setRgb(0,128,0);
//     int index=listform->getUi()->listWidget_2->count();
//     listform->getUi()->listWidget_2->item(index-1)->setTextColor(color_green);


     listform->getUi()->listWidget_2->addItem(strlist.at(3));
     listform->getUi()->listWidget_2->addItem(" ");}

        break;
    case 4:
       {//只需对case4的昵称做一个考证
        qDebug()<<"in case 4";
       // ListForm *listui=ListForm::gethand();
        listform->group_red(strlist.at(2));  //该行代码及其危险！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
       // QMessageBox::information(NULL,strlist.at(2)+"发来了消息",strlist.at(3),QMessageBox::Ok);
        //  ListForm *listform = ListForm::gethand();
           for (int i=0;i<listform->grouplist.count();i++){
               if(listform->grouplist.at(i)->getGro_id()==strlist.at(2))
               {
                   QDateTime current_date_time =QDateTime::currentDateTime();
                   QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                   listform->grouplist.at(i)->getUi()->listWidget->addItem(current_date+"  昵称:"+strlist.at(5)+"  id:"+strlist.at(3));
                   //新添加内容如下↓
//                   QColor color_green;
//                   color_green.setRgb(0,128,0);
//                   int index=listform->grouplist.at(i)->getUi()->listWidget->count();
//                   listform->grouplist.at(i)->getUi()->listWidget->item(index-1)->setTextColor(color_green);

                   listform->grouplist.at(i)->getUi()->listWidget->addItem(strlist.at(4));
                   listform->grouplist.at(i)->getUi()->listWidget->addItem(" ");
               }
           }
           }
            break;
    case 5:
           if(strlist.at(2)=="群组编辑成功"){
               QMessageBox::information(NULL,strlist.at(2)+"群组编辑成功","群组编辑成功",QMessageBox::Ok);
           }
              break;

    case 6:
        {
        qDebug()<<"in case6";
         temp = strlist;
        temp.removeFirst();
        temp.removeFirst();
        temp.removeFirst();
        qDebug()<<"准备进入temp";
        for(int i=0;i<temp.count();i++){
            qDebug()<<"temp的第"<<i<<"个值："<<temp.at(i);
        }
        socket_hand->set_groups_id(temp);
        }
        break;
    case 7:
        qDebug()<<"in case7";
        if(strlist.at(2)=="退群成功"){
             QMessageBox::information(NULL,"退群成功","退群成功",QMessageBox::Ok);
        }
        else{
             QMessageBox::information(NULL,"退群失败","退群失败",QMessageBox::Ok);
        }
        break;
    //qDebug()<<QString::fromStdString((databyte.toStdString()));
    }
}
else if(strlist.at(0)=="send_picture"){
this->size=strlist.at(1).toInt(); //获取图片size
//获取发送者id
this->FriendId=strlist.at(2);
qDebug()<<"goal size:"<<this->size;
qDebug()<<"get the sender id:"<<this->FriendId;
qDebug()<<"in send_picture";
       // tcpsocket.write("ok");
        tcpsocket.waitForBytesWritten(); //????
        this->flag0=1;//进入读图片模式
        qDebug()<<"flag0 turn to 1";
}
else if((str.count()>0)&&(this->flag0==2)){

qDebug()<<"in get_picture";
// flag++;
//qDebug()<<flag;
//  ListForm *listform = ListForm::gethand();

//buffer.toHex()
QPixmap pix;

    bool ret = pix.loadFromData(buffer,"jpg");

    //show
    if(ret){
        qDebug()<<"load png done!";
       // pix.scaled(10,10,Qt::KeepAspectRatioByExpanding);//设置接收图片的大小
        pix=pix.scaledToWidth(200);

        this->send_id_picture(this->FriendId,pix);
    }
    this->flag0=0;
    qDebug()<<"flag0 turn to 0";





      // tcpsocket.write("ok");
          // ->lb_picture->setPixmap(pix.scaled(250,200));
}


}
