#include "register.h"
#include "ui_register.h"
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QMovie>
Register* Register::reg = nullptr;
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    QMovie *movie = new QMovie(":/compute.gif");
    qDebug()<<"movie";
    QSize si(ui->label_6->width(),ui->label_6->height());
        movie->setScaledSize(si);
    ui->label_6->setMovie(movie);
    movie->start();
}

Register::~Register()
{
    delete ui;
}


/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/



void Register::on_regButton_clicked()
{
//    QString mes="10021001nihao";
//    qDebug()<<mes;
//    TCPsocket* sockethand = TCPsocket::gethand();
//    sockethand->send_info(mes);

    QString ID;//ID
    QString PWD;//PassWord
     QString PWD_2;//再次确认password
     QString N_name;
      ID = ui->lineEdit->text();  //获取账号
      PWD = ui->lineEdit_2->text();//获取密码
      PWD_2 = ui->lineEdit_3->text();
      N_name = ui->lineEdit_4->text();
        if(ID == ""){
        QMessageBox::warning(this,"账号不能为空","账号不能为空");   //账号为空
        }
      else  if(PWD == ""){
         QMessageBox::warning(this,"密码不能为空","密码不能为空");  //密码为空
        }
      else if(PWD != PWD_2){
            QMessageBox::warning(this,"请再次确认密码","请再次确认密码"); //再次确认密码
        }
      else {
            QString mes="register#1#"+ID+"#"+PWD+"#"+N_name;
            qDebug()<<mes;
            TCPsocket* sockethand = TCPsocket::gethand();
            sockethand->send_info(mes);


//            QSqlQuery query;
//            query.prepare("select ID,password from user_table where ID =:ID ");
//            query.bindValue(":ID",ID);
//            query.exec();
//            if(!query.next()){                 //数据库中不存在该账号和密码,插入
//                query.prepare("insert into user_table(ID,password)"
//                              "values(:ID,:PWD)");
//                query.bindValue(":ID",ID);
//                query.bindValue(":password",PWD);
//                query.exec();
//                QMessageBox::information(this,"注册成功","注册成功",QMessageBox::Ok);
//            }
//            else{                              //数据库中存在该账号和密码
//                QMessageBox::warning(this,"","警告！账号已存在！");
//            }
        }
}


/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

void Register::on_lineEdit_editingFinished()
{
     QString ID;//ID
       ID = ui->lineEdit->text();  //获取账号
       if(ID.count()<4 ){
           ui->label_7->setText("× 小于4位");
       }
     else  if(ID.count()>12 ){
           ui->label_7->setText("× 大于12位");
       }
       else {
              ui->label_7->setText("√");
       }
}


void Register::on_lineEdit_2_textEdited(const QString &arg1)
{
    ui->progressBar->show();

    int flag0=0,flag1=0,flag2=0;
    QString password=arg1;
    for(int i=0;i<password.count();i++){
        if(password[i]<=57&&password[i]>=48){

        }
        else if(password[i]<=90&&password[i]>=65){
            flag0=1;
        }
        else if(password[i]<=122&&password[i]>=97){
            flag1=1;
        }
        else{
            flag2=1;
        }
    }
    int total=flag0+flag1+flag2;
    if(total==0){
        ui->label_8->setText("弱");
        ui->label_8->setStyleSheet("color:red");
        ui->progressBar->setValue(25);
        ui->progressBar->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:red}");
    }
    else if(total==1){
        ui->label_8->setText("微弱");
        ui->label_8->setStyleSheet("color:rgb(255,128,64)");
        ui->progressBar->setValue(50);
       ui->progressBar->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:rgb(255,128,64)}");
    }
    else if(total==2){
        ui->label_8->setText("中");
        ui->label_8->setStyleSheet("color:rgb(0,128,0)");
        ui->progressBar->setValue(75);
        ui->progressBar->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:rgb(219,219,0)}");

    }
    else if(total==3){
        ui->label_8->setText("高");
        ui->label_8->setStyleSheet("color:rgb(0,200,0)");
        ui->progressBar->setValue(100);
        ui->progressBar->setStyleSheet("QProgressBar{background:white;} QProgressBar::chunk{background:rgb(0,255,0)}");

    }
}

void Register::on_lineEdit_3_textEdited(const QString &arg1)
{
    QString password=arg1;
    if(arg1 !=ui->lineEdit_2->text()){
        ui->label_9->setText("请再次确认密码");
    }
    else{
        ui->label_9->setText("√");
    }
}

void Register::on_lineEdit_4_textEdited(const QString &arg1)
{QString password=arg1;
    if(arg1 ==NULL){
        ui->label_10->setText("昵称不能为空");
    }
    else{
        ui->label_10->setText("√");
    }

}

