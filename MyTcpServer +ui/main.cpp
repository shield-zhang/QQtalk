#include "mytcpserver.h"

#include <QApplication>
#include "mysql.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySql *sqlhand = MySql::gethand();
    sqlhand->open_database();
    qDebug()<<sqlhand->delete_group("1001","11100");
    MyTcpServer w;
    w.show();
    return a.exec();
}
