#include "addnewfriendform.h"
#include "ui_addnewfriendform.h"

AddNewFriendForm::AddNewFriendForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewFriendForm)
{
    ui->setupUi(this);
}

AddNewFriendForm::~AddNewFriendForm()
{
    delete ui;
}

void AddNewFriendForm::on_pushButton_clicked()
{
     TCPsocket* sockethand = TCPsocket::gethand();
     sockethand->if_friend(sockethand->getNew_friend_id(),sockethand->getMy_id(),true);
     this->close();
}

void AddNewFriendForm::on_pushButton_2_clicked()
{
    TCPsocket* sockethand = TCPsocket::gethand();
    sockethand->if_friend(sockethand->getNew_friend_id(),sockethand->getMy_id(),false);
    this->close();
}
void AddNewFriendForm::setLableText(int id, QString txt)
{
    if(id==1){
        ui->label->setText(txt);
    }
}
