#include "listform.h"
#include "ui_listform.h"
#include<QDateTime>
ListForm* ListForm::listform=nullptr;
int ListForm::group_i=0;
int ListForm::count_i=0;
int number=0;
ListForm::ListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListForm)
{
    ui->setupUi(this);

  /*   TCPsocket* sockethand = TCPsocket::gethand();
   connect(sockethand,SIGNAL(sig_listform_open()),this,SLOT(slot_init()));
   sockethand->get_friend_id(sockethand->getMy_id());
    QList<QString> friend_list=sockethand->getFriends_id();//获取好友列表id
    qDebug()<<"好友列表中共有这么多个人："<<friend_list.count();
    if(friend_list.count() > 0){
        for(int i = 0;i < friend_list.count();i++){
          sockethand->send_id_to_get_name(friend_list.at(i));//获得好友的昵称
          ui->listWidget->addItem("昵称:"+sockethand->getMy_name()+"  id:"+friend_list.at(i));
           }
       }
    sockethand->send_id_to_get_name(sockethand->getMy_id());//sockethand里获得自己的昵称
    ui->label_4->setText(sockethand->getMy_id());
    ui->lineEdit_3->setText(sockethand->getMy_name());*/

}

ListForm::~ListForm()
{
    delete ui;
}

void ListForm::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QColor color_white;
    color_white.setRgb(255,255,255);
    item->setBackgroundColor(color_white);

    qDebug()<<"即将进行弹窗";
   QList<QString> temp=item->text().split(" ");//此时item里传来的是朋友的id

    for (int i=0;i<p2plist.count();i++) {
        if(p2plist[i]->getP2p_friend_id()==temp.at(0))
        {
             qDebug()<<"进入了if的内部";
            p2plist[i]->show();
        }
    }
}

void ListForm::on_pushButton_clicked()
{
        TCPsocket* sockethand = TCPsocket::gethand();
        sockethand->send_friend_request(sockethand->getMy_id(),ui->lineEdit->text());

}

void ListForm::on_pushButton_2_clicked()
{ ui->listWidget->setStyleSheet("");
    ui->listWidget_2->setStyleSheet("");
    ui->GroupWidget->setStyleSheet("");
    ui->lineEdit_2->setStyleSheet("");
   QPixmap pixmap(ic.NextImage());
   int with = ui->label_7->width();
       int height = ui->label_7->height();
       QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
       //QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 按比例缩放
       ui->label_7->setPixmap(fitpixmap);
       number++;


}

void ListForm::on_pushButton_3_clicked()
{
    TCPsocket* sockethand = TCPsocket::gethand();
   sockethand->set_name(sockethand->getMy_id(),ui->lineEdit_3->text());
}

void ListForm::init(){
    TCPsocket* sockethand = TCPsocket::gethand();
        sockethand->get_friend_id(sockethand->getMy_id());//向服务器发送自己ID，得到好友id列表
        sockethand->send_id_to_get_friends_name(sockethand->getMy_id());//向服务器发送自己ID，得到好友昵称列表
        QList<QString> friend_list=sockethand->getFriends_id();//获取好友列表id
        QList<QString> friend_list_name=sockethand->get_friends_name();//获取好友昵称列表
        //QList<QString> friend_name_list;
        qDebug()<<"好友列表中共有这么多个人："<<friend_list.count();
        if(friend_list.count() > 0){
            for(int i = 0;i < friend_list.count();i++){
                qDebug()<<"第i个好友的ID:"<<friend_list.at(i);
             // sockethand->send_id_to_get_name(friend_list.at(i));//获得好友的昵称
              //friend_name_list=sockethand->get_friends_name();
              //ui->listWidget->addItem("昵称:"+sockethand->getMy_name()+"  id:"+friend_list.at(i));
              ui->listWidget->addItem(friend_list.at(i)+" "+friend_list_name.at(i));
              this->friend_off_color(friend_list.at(i));
              //qDebug()<<"昵称:"<<sockethand->getMy_name()<<"  id:"<<friend_list.at(i);
              //好友界面 初始化
              P2PForm* p2pform=p2pforms+count_i++;
              p2pform->setP2p_friend_id(friend_list.at(i));//将好友id与p2p聊天页面绑定
              p2pform->setLableText(1,friend_list.at(i)+" "+friend_list_name.at(i));
              p2plist.append(p2pform);

         }
        }
        //群组界面  初始化
        sockethand->send_id_to_get_groups_id(sockethand->getMy_id());//给服务器发ID,得到群组列表
        QList<QString> group_id = sockethand->get_groups_id();//获取群组列表
        qDebug()<<"group_id的个数："<<group_id.count();
         for(int i=0;i<group_id.count();i++){
             ui->GroupWidget->addItem(group_id.at(i));
             GroupForm* groupform = groupforms+group_i++;
             groupform->setGro_id(group_id.at(i));//设置为当前界面的群组名称
             groupform->setLableText(1,group_id.at(i));//将界面设置成当前群组的ID
              grouplist.append(groupform);
         }



        sockethand->send_id_to_get_name(sockethand->getMy_id());//sockethand里获得自己的昵称
        ui->label_4->setText(sockethand->getMy_id());
        ui->lineEdit_3->setText(sockethand->getMy_name());
        sockethand->send_id_to_get_friends_name(sockethand->getMy_id());//获得好友昵称列表
}

void ListForm::on_pushButton_4_clicked()//刷新好友列表
{
        QString str;
        TCPsocket* sockethand = TCPsocket::gethand();
        sockethand->get_friend_id(sockethand->getMy_id());//向服务器发送自己ID，得到好友id列表
        sockethand->send_id_to_get_friends_name(sockethand->getMy_id());//向服务器发送自己ID，得到好友昵称列表
        QList<QString> friend_list=sockethand->getFriends_id();//获取好友id列表
        QList<QString> friend_list_name=sockethand->get_friends_name();//获取好友昵称列表
        qDebug()<<"好友列表中共有这么多个人："<<friend_list.count();
        count_i=0;//——————————————————————————————————————————————————————————————————————————————————————需要更新
        p2plist.clear();
            ui->listWidget->clear();
            if(friend_list.count() > 0){
                for(int i = 0;i < friend_list.count();i++){
                    qDebug()<<"第i个好友的ID:"<<friend_list.at(i);
                 // sockethand->send_id_to_get_name(friend_list.at(i));//获得好友的昵称
                  //friend_name_list=sockethand->get_friends_name();
                  //ui->listWidget->addItem("昵称:"+sockethand->getMy_name()+"  id:"+friend_list.at(i));
                    ui->listWidget->addItem(friend_list.at(i)+" "+friend_list_name.at(i));
                    this->friend_off_color(friend_list.at(i));
                //刷新好友界面
                  P2PForm* p2pform=p2pforms+count_i++;
                  p2pform->setP2p_friend_id(friend_list.at(i));
                  p2pform->setLableText(1,friend_list.at(i)+" "+friend_list_name.at(i));
                  p2pform->getUi()->listWidget->clear();
                  p2plist.append(p2pform);

                   }
                str="register#3#"+sockethand->getMy_id();
                sockethand->send_info(str);
               }
        /*else if(friend_list.count() < ui->listWidget->count()){
            int i=0;
            int j=0;

            for( i=0;i<ui->listWidget->count();i++){
                for(j=0;j<friend_list.count();j++){
                    if(ui->listWidget->item(i)->text()==friend_list.at(j)) break;
                }
                if(j==friend_list.count()){
                    ui->listWidget->takeItem(i);
                    break;
                }
            }
        }*/

        //ui->lineEdit_3->setText(sockethand->getMy_name());
}

void ListForm::friend_red(QString sender_id){
    QColor color_red;
    color_red.setRed(255);
    qDebug()<<"insetcolor";
    for(int i=0;i<ui->listWidget->count();i++){
        qDebug()<<"itemtext:"+ui->listWidget->item(i)->text();
        qDebug()<<"sender_id:"+sender_id;
        QList<QString> temp=ui->listWidget->item(i)->text().split(" ");
        if( temp.at(0)==sender_id){
            qDebug()<<"find item"<<i;
            ui->listWidget->item(i)->setBackgroundColor(color_red);
            break;
        }
    }
this->friend_on_color(sender_id);
}
void ListForm::group_red(QString sender_id){
    QColor color_red;
    color_red.setRed(255);
    qDebug()<<"insetcolor";
    for(int i=0;i<ui->GroupWidget->count();i++){
        qDebug()<<"itemtext:"+ui->GroupWidget->item(i)->text();
        qDebug()<<"sender_id:"+sender_id;
        if(ui->GroupWidget->item(i)->text()==sender_id){
            qDebug()<<"find item"<<i;
            ui->GroupWidget->item(i)->setBackgroundColor(color_red);
            break;
        }
    }

}
void ListForm::friend_on_color(QString friend_id){
    QColor color_green;
    color_green.setRgb(0,128,0);
    qDebug()<<"in friend on setcolor";
    for(int i=0;i<ui->listWidget->count();i++){
//        qDebug()<<"itemtext:"+ui->listWidget->item(i)->text();
//        qDebug()<<"sender_id:"+sender_id;
      QList<QString> temp=ui->listWidget->item(i)->text().split(" ");
        if(temp.at(0)==friend_id){
          //  qDebug()<<"find item"+i;
            ui->listWidget->item(i)->setTextColor(color_green);
            break;
        }
    }

}
void ListForm::friend_off_color(QString friend_id){
    QColor color_grey;
    color_grey.setRgb(184,184,184);
    qDebug()<<"in friend off setcolor";
    for(int i=0;i<ui->listWidget->count();i++){
//        qDebug()<<"itemtext:"+ui->listWidget->item(i)->text();
//        qDebug()<<"sender_id:"+sender_id;
         QList<QString> temp=ui->listWidget->item(i)->text().split(" ");
        if(temp.at(0)==friend_id){
          //  qDebug()<<"find item"+i;
            ui->listWidget->item(i)->setTextColor(color_grey);
            break;
        }
    }
}

void ListForm::on_pushButton_5_clicked()//发送广播消息
{
     TCPsocket* sockethand = TCPsocket::gethand();
     sockethand->send_group_message(sockethand->getMy_id(),ui->lineEdit_2->text());

     QDateTime current_date_time =QDateTime::currentDateTime();
     QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
     ui->listWidget_2->addItem(current_date + "  昵称:"+sockethand->getMy_name()+"  id:"+sockethand->getMy_id());//发送时间

////
//     QColor color_blue;
//     color_blue.setRgb(0, 85, 255);
//     int index=ui->listWidget->count();
//     ui->listWidget->item(index-1)->setTextColor(color_blue);


     ui->listWidget_2->addItem(ui->lineEdit_2->text());//发送内容
     ui->listWidget_2->addItem("");//空格
     ui->lineEdit_2->clear();

}

Ui::ListForm *ListForm::getUi() const
{
    return ui;
}

void ListForm::setUi(Ui::ListForm *value)
{
    ui = value;
}
void ListForm::on_pushButton_7_clicked()
{
    QString group_id=ui->lineEdit_4->text();// 群ID lable
    //groups_id.append(groupidinput);//将群号添加在 群号列表中
    QList<QString>group_m_list=ui->lineEdit_5->text().split(",");//成员ID lable 逗号分隔
    QString str="groupmsg#5#"+group_id;
    for (int i=0;i<group_m_list.count();i++) {
        str+="#"+group_m_list.at(i);
    }
    TCPsocket* sockethand = TCPsocket::gethand();
    qDebug()<<"send messege!:"+str;
    sockethand->send_info(str);



}


void ListForm::on_pushButton_6_clicked()// 刷新群ID
{
    group_i=0;
    grouplist.clear();
    ui->GroupWidget->clear();
    TCPsocket* sockethand = TCPsocket::gethand();
    sockethand->send_id_to_get_groups_id(sockethand->getMy_id());//给服务器发ID,得到群组列表
    QList<QString> group_id = sockethand->get_groups_id();//获取群组列表
    for(int i=0;i<group_id.count();i++){//将群组显示进列表
        qDebug()<<"第"<<i<<"个群组是："<<group_id.at(i);
        ui->GroupWidget->addItem(group_id.at(i));
        GroupForm* groupform = groupforms+group_i++;
        groupform->setGro_id(group_id.at(i));//设置为当前界面的好友名称
        groupform->setLableText(1,group_id.at(i));//将界面设置成当前好友的ID
       groupform->getUi()->listWidget->clear();
         grouplist.append(groupform);
    }
    //刷新群组界面


}

void ListForm::on_GroupWidget_itemClicked(QListWidgetItem *item)
{
    QColor color_white;
    color_white.setRgb(255,255,255);
    item->setBackgroundColor(color_white);


    qDebug()<<"即将进行弹窗";
            qDebug()<<"p2plist里面的项目数:"<<grouplist.count();
     //   qDebug()<<grouplist[0]->getGro_id()+"  "<<grouplist[1]->getGro_id();


    QString temp=item->text();//此时item里传来的是群聊的id

    for (int i=0;i<grouplist.count();i++) {
        if(grouplist[i]->getGro_id()==temp)
        {
             qDebug()<<"进入了if的内部";
            grouplist[i]->show();
        }
    }
}


