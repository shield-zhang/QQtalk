#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include "mainwindow.h"
#include "register.h"
#include <QLabel>
#include <QMovie>
#include <QTime>
#include "listform.h"


Login* Login::log = nullptr;
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    QMovie *movie = new QMovie(":/fox.gif");
    qDebug()<<"movie";
    QSize si(ui->label->width(),ui->label->height());
        movie->setScaledSize(si);
    ui->label->setMovie(movie);
    movie->start();

    QMovie *movie1 = new QMovie(":/jump.gif");
    qDebug()<<"movie";
    QSize si1(ui->label_2->width(),ui->label_2->height());
        movie1->setScaledSize(si1);
    ui->label_2->setMovie(movie1);
    movie1->start();

}
void Login::closegif(){
    int msec=2000;
        QTime _Timer = QTime::currentTime().addMSecs(msec);

        while( QTime::currentTime() < _Timer )

        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    qDebug()<<"close";
    ui->label->close();
}
Login::~Login()
{
    delete ui;
}


/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/


void Login::on_loginButton_clicked()
{qDebug()<<"on_loginButton_clicked()";

    savecfg();
    QString ID;//ID
    TCPsocket* sockethand = TCPsocket::gethand();

      QString PWD;//PassWord
        ID = ui->lineEdit->text();  //获取账号
        PWD = ui->lineEdit_2->text();//获取密码
        sockethand->setMy_id(ID);
        ListForm *listform = ListForm::gethand();
        listform->init();//这里初始化----------------------------------------------------------------

          if(ID == ""){
              QMessageBox::warning(this,"账号不能为空","账号不能为空");   //账号为空
              }
        else  if(PWD == ""){
           QMessageBox::warning(this,"密码不能为空","密码不能为空");  //密码为空
          }
        else{ //调用数据库比对
              TCPsocket* sockethand = TCPsocket::gethand();
                        QString str="register#2#"+ID+"#"+PWD;
                        sockethand->send_info(str);


            //           this->close();                            //关闭登录界面

//          QSqlQuery query;
//          query.prepare("select ID,password from user_table where ID =:ID and password =:PWD");
//          query.bindValue(":ID",ID);
//          query.bindValue(":password",PWD);
//          query.exec();
//          if(!query.next()){                 //数据库中不存在该账号和密码
//              QMessageBox::warning(this,"","警告！账号或密码输入错误！");
//          }
//          else{
//              QMessageBox::information(this,"登录成功","登录成功",QMessageBox::Ok);//存在该账号
//              MainWindow *m=new MainWindow();
//              m->show();                               //显示聊天界面
//             this->close();                            //关闭登录界面

//          }
         }

}

/*——————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/


void Login::on_registerButton_clicked()
{
    Register* reg=Register::gethand();
    reg->show();
}






void Login::savecfg()
{
       if(ui->checkBox->isChecked())
           {  //qDebug()<<"check true";
               remeberPasswd= true;
           }
           else
           {
               remeberPasswd= false;
           }

    qDebug()<<"savecfg";
    QSettings cfg("fzconfig.ini",QSettings::IniFormat);
    username= ui->lineEdit->text();
    passwd= ui->lineEdit_2->text();
int flag=1;
   int size = cfg.beginReadArray("logins");
   qDebug()<<size;
   cfg.endArray();
    cfg.beginWriteArray("logins");
                                                       //gai guo
    for (int i = 0; i < size; ++i) {
    cfg.setArrayIndex(i);
    if(username == cfg.value("id").toString() ){
     flag=0;
     cfg.setValue("PWD",passwd);
     cfg.setValue("rem",remeberPasswd);
    }
   }
    if(flag==1){
        cfg.setArrayIndex(size);
        cfg.setValue("id",username);
        cfg.setValue("PWD",passwd);
        cfg.setValue("rem",remeberPasswd);
    }

    cfg.endArray();
   size =cfg.beginReadArray("logins");
   qDebug()<<size;
   cfg.endArray();



    cfg.sync();
}

void Login::loadcfg()
{        qDebug()<<"loadcfg";
    QSettings  cfg("fzconfig.ini",QSettings::IniFormat);

    //在这儿使用了宏定义，也可以将LOAD_CFG_VARIANT用cfg.value代替
//    username= LOAD_CFG_VARIANT(username).toString();
//    passwd= LOAD_CFG_VARIANT(passwd).toString();
    //  RemeberPasswd= LOAD_CFG_VARIANT(remeberPasswd).toString();
                     //将全部数据存入login_list

    int size = cfg.beginReadArray("logins");
    for (int i = 0; i < size; ++i) {
    cfg.setArrayIndex(i);
    logins login;
    login.id=cfg.value("id").toString();
    login.PWD=cfg.value("PWD").toString();
    login.rem=cfg.value("rem").toString();
    qDebug()<<"cfg.vakue"<<cfg.value("id").toString();

    ui->comboBox->addItem(login.id);
    login_list.append(login);}
   //cfg.endArray();


                     //默认为最早一次输入的数据
  // cfg.setArrayIndex(size);
   username =cfg.value("id").toString();
   passwd =cfg.value("PWD").toString();
   RemeberPasswd =cfg.value("rem").toString();
         qDebug()<<username;
         qDebug()<<passwd;
              qDebug()<<RemeberPasswd;

  //  AutoLogin= LOAD_CFG_VARIANT(autologin).toString();

ui->lineEdit->setText(username);
    if(RemeberPasswd== "true")
    {//qDebug()<<"load true";

        ui->lineEdit_2->setText(passwd);

        ui->checkBox->setChecked(true);
    }
    else{
         ui->lineEdit_2->setText(NULL);
         ui->checkBox->setChecked(false);
    }
//	if(AutoLogin== "true")
//	{
//		ui->Auto_Log->setChecked(true);
//	}
}

void Login::LoginInit()
 {

    //暗注释
     ui->lineEdit->setPlaceholderText(QStringLiteral("请输入用户名"));
     ui->lineEdit->setMaxLength(8);
     ui->lineEdit_2->setPlaceholderText(QStringLiteral("请输入密码"));
     ui->lineEdit_2->setMaxLength(8);
    flag=0;


}

void Login::on_lineEdit_2_textChanged(const QString &arg1)
{
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}

void Login::on_comboBox_currentTextChanged(const QString &arg1)
{   qDebug()<<"on_comboBox_currentTextChanged";
    QSettings  cfg("fzconfig.ini",QSettings::IniFormat);
      username= ui->comboBox->currentText();
       //cfg.beginReadArray("logins");
       int size = cfg.beginReadArray("logins");
       for (int i = 0; i < size; ++i) {
       cfg.setArrayIndex(i);
       if(username == cfg.value("id").toString() ){
          ui->lineEdit->setText(cfg.value("id").toString());
          ui->lineEdit_2->setText(NULL);
          if( cfg.value("rem").toString() =="true"){
           ui->lineEdit_2->setText(cfg.value("PWD").toString());
           ui->checkBox->setChecked(true);}
       }
      }
      cfg.endArray();

}
