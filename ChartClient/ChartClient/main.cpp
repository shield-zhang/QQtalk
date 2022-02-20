#include "mainwindow.h"

#include <QApplication>
#include "login.h"
#include "listform.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*QFile qss("stylesheet.qss");
       if( qss.open(QFile::ReadOnly))
       {
           qDebug("open success");
           QString style = QLatin1String(qss.readAll());
           a.setStyleSheet(style);
           qss.close();
       }
       else
       {
          qDebug("Open failed");
 } 可用以改变样式 */
    TCPsocket* sockethand = TCPsocket::gethand();
    sockethand->connect_server();
    Login* log=Login::gethand();
    log->LoginInit();

    log->loadcfg();
    log->show();
    log->closegif();

    ListForm* listform = ListForm::gethand();

    //listform->show();
    //QObject::connect(sockethand,SIGNAL(sig_listform_open()),&listform,SLOT(slot_init()));
   /* if(log->flag == 1){
        ListForm* listform=new ListForm();
        listform->init();
        listform->show();
    }*/


    return a.exec();
}
