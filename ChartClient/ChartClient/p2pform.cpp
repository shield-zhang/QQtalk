#include "p2pform.h"
#include "ui_p2pform.h"
#include<listform.h>
#include<QDateTime>

P2PForm::P2PForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::P2PForm)
{
    ui->setupUi(this);

    if (number%6==1){                              //更改聊天窗口背景出错时将此段剩余全部删去
    QImage Image;
        Image.load(":/1.jpg");
        QPixmap pixmap = QPixmap::fromImage(Image);
    ui->label_2->setPixmap(pixmap);}
    else if (number%6==2){
        QImage Image;
            Image.load(":/2.jpg");
            QPixmap pixmap = QPixmap::fromImage(Image);
        ui->label_2->setPixmap(pixmap);}
    else if (number%6==3){
        QImage Image;
            Image.load(":/3.jpg");
            QPixmap pixmap = QPixmap::fromImage(Image);
        ui->label_2->setPixmap(pixmap);}
    else if (number%6==4){
        QImage Image;
            Image.load(":/4.jpg");
            QPixmap pixmap = QPixmap::fromImage(Image);
        ui->label_2->setPixmap(pixmap);}
    else if (number%6==5){
        QImage Image;
            Image.load(":/5.jpg");
            QPixmap pixmap = QPixmap::fromImage(Image);
        ui->label_2->setPixmap(pixmap);}
    else if (number%6==0){
        QImage Image;
            Image.load(":/6.jpg");
            QPixmap pixmap = QPixmap::fromImage(Image);
        ui->label_2->setPixmap(pixmap);}                //更改聊天窗口背景出错时将此前全部删去
}



P2PForm::~P2PForm()
{
    delete ui;
}
void P2PForm::setLableText(int id,QString txt){//显示出来在跟谁聊天

    if(id==1){
        ui->label->setText(txt);
    }
}

void P2PForm::on_pushButton_clicked()//点击“发送”后发送消息到服务端
{

    TCPsocket* sockethand = TCPsocket::gethand();
    sockethand->send_message(this->getP2p_friend_id(),sockethand->getMy_id(),ui->lineEdit->text());
   //发送聊天消息, 接受者id#发送者id#消息
   //时间类
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    //
//         QColor color_blue;
//         color_blue.setRgb(0, 85, 255);
//         int index=ui->listWidget->count();
//         ui->listWidget->item(index-1)->setTextColor(color_blue);

    ui->listWidget->addItem(current_date+"  昵称:"+sockethand->getMy_name()+"  id:"+sockethand->getMy_id());//发送时间
    ui->listWidget->addItem(ui->lineEdit->text());//发送内容
    ui->listWidget->addItem("");//空格
    ui->lineEdit->clear();//发送消息后清除lineEdit
}

QString P2PForm::getP2p_friend_id() const
{
    return p2p_friend_id;
}

void P2PForm::setP2p_friend_id(const QString &value)
{
    p2p_friend_id = value;
}

Ui::P2PForm *P2PForm::getUi() const
{
    return ui;
}

void P2PForm::setUi(Ui::P2PForm *value)
{
    ui = value;
}

void P2PForm::set_image(QPixmap pix){
    qDebug()<<"set image success!";
    ui->label_3->setPixmap(pix);
}


void P2PForm::on_pushButton_2_clicked()
{
    TCPsocket* sockethand = TCPsocket::gethand();
    QString friend_id = this->getP2p_friend_id();//这里获取好友ID
    QString usr_id = sockethand->getMy_id();
    sockethand->delete_friend(usr_id,friend_id);
}

void P2PForm::on_pushButton_3_clicked()
{   TCPsocket* sockethand = TCPsocket::gethand();
    QString sender_ID = sockethand->getMy_id(); //获取usrID
    QString receiver_id = p2p_friend_id;
    sockethand->send_picture(sender_ID,receiver_id);


}

void P2PForm::on_pushButton_4_clicked()
{
    ui->label_3->clear();
}
