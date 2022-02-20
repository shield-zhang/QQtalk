#include "groupform.h"
#include "ui_groupform.h"
#include "tcpsocket.h"
#include<listform.h>
#include<QDateTime>

GroupForm::GroupForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupForm)
{
    ui->setupUi(this);
}

GroupForm::~GroupForm()
{
    delete ui;
}

void GroupForm::on_pushButton_clicked()
{
    TCPsocket* sockethand = TCPsocket::gethand();
    sockethand->send_group_new_message(sockethand->getMy_id(),this->getGro_id(),ui->lineEdit->text());
    //发送消息，发送方，内容
   //时间类
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->listWidget->addItem(current_date+"  昵称:"+sockethand->getMy_name()+"  id:"+sockethand->getMy_id());//发送时间
//
//         QColor color_blue;
//         color_blue.setRgb(0, 85, 255);
//         int index=ui->listWidget->count();
//         ui->listWidget->item(index-1)->setTextColor(color_blue);


    ui->listWidget->addItem(ui->lineEdit->text());//发送内容
    ui->listWidget->addItem(" ");//空格
    ui->lineEdit->clear();//发送消息后清除lineEdit
}
QString GroupForm::getGro_id() const
{
    return Gro_id;
}

void GroupForm::setLableText(int id,QString txt){
    ui->label->setText(txt);
}

void GroupForm::setGro_id(const QString &value)
{
    Gro_id = value;
}


Ui::GroupForm *GroupForm::getUi() const
{
    return ui;
}

void GroupForm::setUi(Ui::GroupForm *value)
{
    ui = value;
}

void GroupForm::on_pushButton_2_clicked()
{
    TCPsocket* sockethand = TCPsocket::gethand();
    QString group_id = ui->label->text();//这里获取好友ID。在显示昵称之后需要分割操作
    QString usr_id = sockethand->getMy_id();
    sockethand->out_group(usr_id,group_id);
}
