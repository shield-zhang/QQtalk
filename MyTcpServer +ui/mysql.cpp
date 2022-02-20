#include "mysql.h"
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/qsqlquery.h>
#include <QDebug>
MySql* MySql::sqlhand=nullptr;

MySql::MySql(QObject *parent) : QObject(parent)
{

}

QList<QString> MySql::get_groupid_byuser(QString user_id){
    QString insert = "select group_id from group_table where user_id =:ID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":ID",user_id);
    //qDebug()<<insert;

    QList<QString> group_id_list;
    if(!sqlquery->exec()){                 //查询失败
       qDebug()<<"get_group_id failed!";
        return group_id_list;
    }
    else{  //查询成功
        qDebug()<<"get_group_id succeessed!";
        while(sqlquery->next())
        {
            qDebug()<<sqlquery->value(0).toString();
            group_id_list.append(sqlquery->value(0).toString());
            //qDebug()<<;
        }

    return group_id_list;
    }
}

bool MySql::delete_group(QString user_id,QString group_id){
    QString insert = "DELETE from group_table where group_id=:GID and user_id=:UID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":UID",user_id);
    sqlquery->bindValue(":GID",group_id);

    if(sqlquery->exec()){
        qDebug()<<"delete group OK";
        return true;
    }
    else{
    qDebug()<<"delete group error ;";
    return false;
    }
}
bool MySql::delete_friend(QString user_id,QString friend_id){
    QString insert = "DELETE from friend_table where user_id=:UID and friend_id=:FID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":UID",user_id);
    sqlquery->bindValue(":FID",friend_id);

    if(sqlquery->exec()){
        qDebug()<<"delete friend OK";
        //return true;
    }
    else{
    qDebug()<<"delete friend error ;";
    //return false;
    }
    insert = "DELETE from friend_table where user_id=:UID and friend_id=:FID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":UID",friend_id);
    sqlquery->bindValue(":FID",user_id);
    if(sqlquery->exec()){
        qDebug()<<"delete friend OK";
        return true;
    }
    else{
    qDebug()<<"delete friend error ;";
    return false;
    }
}


QString MySql::get_id_byport(QString port){
    QString insert="select ID from user_table where port =:PORT";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":PORT",port);
    sqlquery->exec();
    if(!sqlquery->next()){                 //数据库中不存在该id
        qDebug()<<"get_id_byport error!";
        return NULL;
    }
    else{
        qDebug()<<"get_id_byport success!";
        return sqlquery->value(0).toString();
    }

}


void MySql::open_database(){//创建数据库连接并打开四个数据库
    if(QSqlDatabase::contains("qt_sql_defualt_connection")){//检查该连接是否存在
        database = QSqlDatabase::database("qt_sql_defualt_connection");//若存在连接则返回该连接并用database接收
    }
     else{
        //不存在该连接则创建连接
        database = QSqlDatabase::addDatabase("QSQLITE");
       // database = QSqlDatabase::addDatabase("QMYSQL");
        database.setDatabaseName("MyDataBase.db");
        database.setUserName("root");
        database.setPassword("12345678");
    }
    if(database.open()){
        QStringList tablelist = database.tables();
        sqlquery = new QSqlQuery(database);  //建立相应数据库的QSqlQuery对象

        QString tablename="friend_table";   //打开或创建 friend_table
        if(!tablelist.contains("friend_table")){
           QString createtable = "CREATE TABLE "+tablename;
           createtable+=" (user_id varchar(10),friend_id varchar(10),primary key(user_id,friend_id),foreign key(user_id) references user_table(ID),";
           createtable+="foreign key(friend_id) references user_table(ID))";
           qDebug()<<createtable;
           if(sqlquery->prepare(createtable)){
               if(sqlquery->exec()){
                   qDebug()<<"Create "+tablename+" OK";
               }

           }
           else{
               qDebug()<<"create "+tablename+" command error ;";
           }
        }
        else{
            qDebug()<<tablename+" already exist ;";
        }
        tablename="user_table";  //打开或创建user_table
        if(!tablelist.contains(tablename)){
           QString createtable = "CREATE TABLE "+tablename+" (N_name varchar(30),ID varchar(10) primary key,password varchar(30),IP varchar(30),port varchar(30))";

           if(sqlquery->prepare(createtable)){
               if(sqlquery->exec()){
                   qDebug()<<"Create "+tablename+" OK";
               }

           }
           else{
               qDebug()<<"create "+tablename+" command error ;";
           }
       }
        else{
            qDebug()<<tablename+" already exist ;";
        }
        tablename="signature_table";  //打开或创建signature_table
        if(!tablelist.contains(tablename)){
           QString createtable = "CREATE TABLE "+tablename+" (sign_text varchar(30),user_id varchar(10) primary key,foreign key(user_id) references user_table(ID))";

           if(sqlquery->prepare(createtable)){
               if(sqlquery->exec()){
                   qDebug()<<"Create "+tablename+" OK";
               }

           }
           else{
               qDebug()<<"create "+tablename+" command error ;";
           }
       }
        else{
            qDebug()<<tablename+" already exist ;";
        }
            tablename="group_table";   //打开或创建 friend_table
        if(!tablelist.contains("group_table")){
           QString createtable = "CREATE TABLE "+tablename;

           createtable+=" (group_id varchar(10),user_id varchar(10),primary key(group_id,user_id),foreign key(user_id) references user_table(ID))";
           //createtable+="foreign key(friend_id) references user_table(ID))";
         //  qDebug()<<createtable;
           sqlquery->prepare(createtable);

           if(sqlquery->exec()){
                   qDebug()<<"Create "+tablename+" OK";

           }


           else{
               qDebug()<<"create "+tablename+" command error ;";

           }
        }
        else{
            qDebug()<<tablename+" already exist ;";

        }
    }

    else{
        qDebug()<<"database open faild ;";
    }
}
void MySql::create_table(){
}

bool MySql::insert_sign_data(QString user_id,QString sign_text){
    QString insert = "INSERT INTO signature_table(user_id,sign_text) VALUES(?,?)";
       sqlquery->prepare(insert);
    sqlquery->addBindValue(user_id);
    sqlquery->addBindValue(sign_text);
       if(sqlquery->exec()){
            qDebug()<<"Insert signature_table OK";
            return true;
        }
        else{
        qDebug()<<"Insert signature_table command error ;";
        return false;
        }
}
bool MySql::update_sign_data(QString user_id,QString sign_text){
    QString insert = "UPDATE signature_table SET sign_text=:signtext where user_id=:id";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":signtext",sign_text);
    sqlquery->bindValue(":id",user_id);
    qDebug()<<insert;
    if(sqlquery->exec()){
        qDebug()<<"update signature_table with sign_text OK";
        return true;
    }
    else{
    qDebug()<<"update signature_table with sign_text error ;";
    return false;
    }
}
QString MySql::get_sign_data(QString user_id){
    QString insert = "select sign_text from signature_table where user_id =:ID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":ID",user_id);
    //qDebug()<<insert;

    QString str;
    if(!sqlquery->exec()){                 //查询失败
       qDebug()<<"get_sign_text failed!";
        return NULL;
    }
    else{  //查询成功
        qDebug()<<"get_sign_text succeessed!";
        while(sqlquery->next())
        {
            //qDebug()<<sqlquery->value(0).toString();
            str=sqlquery->value(0).toString();
            //qDebug()<<;
        }

    return str;
    }
}
bool MySql::insert_group_data(QString group_id,QString user_id){
    QString insert = "INSERT INTO group_table(group_id,user_id) VALUES(?,?)";
       sqlquery->prepare(insert);
    sqlquery->addBindValue(group_id);
    sqlquery->addBindValue(user_id);
       if(sqlquery->exec()){
            qDebug()<<"Insert group_table OK";
            return true;
        }
        else{
        qDebug()<<"Insert group_table command error ;";
        return false;
        }

}
QList<QString> MySql::get_groupmember_id(QString group_id){
    QString insert = "select user_id from group_table where group_id =:ID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":ID",group_id);
    qDebug()<<insert;

    QList<QString> user_id_list;
    if(!sqlquery->exec()){                 //查询失败
       qDebug()<<"get_groupmember_id failed!";
        return user_id_list;
    }
    else{  //查询成功
        qDebug()<<"get_groupmember_id succeessed!";
        while(sqlquery->next())
        {
            qDebug()<<sqlquery->value(0).toString();
            user_id_list.append(sqlquery->value(0).toString());
            //qDebug()<<;
        }

    return user_id_list;
    }
}
bool MySql::friend_exist(QString user_id,QString friend_id){
    QString insert = "select * from friend_table where user_id =:UID and friend_id =:FID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":UID",user_id);
    sqlquery->bindValue(":FID",friend_id);
    //qDebug()<<insert;

    //QString myname;
    if(!sqlquery->exec()){                 //查询失败
       qDebug()<<"friend_exist_excute failed!";
        return false;
        }
    else{  //查询成功

        qDebug()<<"friend exist 函数已执行";
        return sqlquery->first();


        }
}
bool MySql::insert_friend_data(QString user_id,QString friend_id){
   // qDebug()<<"Begin Insert";
 QString insert = "INSERT INTO friend_table(user_id,friend_id) VALUES(?,?)";
    sqlquery->prepare(insert);
 sqlquery->addBindValue(user_id);
 sqlquery->addBindValue(friend_id);
    if(sqlquery->exec()){
         qDebug()<<"Insert friend_table OK";

     }
     else{
     qDebug()<<"Insert friend_table command error ;";

     }
      insert = "INSERT INTO friend_table(user_id,friend_id) VALUES(?,?)";
        sqlquery->prepare(insert);

     sqlquery->addBindValue(friend_id);
     sqlquery->addBindValue(user_id);
     if(sqlquery->exec()){
         qDebug()<<"Insert friend_table OK";
         return true;
     }
     else{
     qDebug()<<"Insert friend_table command error ;";
        return false;
     }
}
QList<QString> MySql::get_friend_id(QString ID){

     QString insert = "select friend_id from friend_table where user_id =:ID";
     sqlquery->prepare(insert);
     sqlquery->bindValue(":ID",ID);
     //qDebug()<<insert;

     QList<QString> friend_id_list;
     if(!sqlquery->exec()){                 //查询失败
        qDebug()<<"get_friend_id failed!";
         return friend_id_list;
     }
     else{  //查询成功
         qDebug()<<"get_friend_id succeessed!";
         while(sqlquery->next())
         {
             qDebug()<<sqlquery->value(0).toString();
             friend_id_list.append(sqlquery->value(0).toString());
             //qDebug()<<;
         }

     return friend_id_list;
     }
}
QString MySql::get_name_byid(QString ID){
QString insert = "select N_name from user_table where ID =:ID";
sqlquery->prepare(insert);
sqlquery->bindValue(":ID",ID);
//qDebug()<<insert;

//QString myname;
if(!sqlquery->exec()){                 //查询失败
   qDebug()<<"get_name_byid failed!";
    return NULL;
    }
else{  //查询成功
    sqlquery->first();
    qDebug()<<"get_name_byid succeessed!";
    return sqlquery->value(0).toString();
    }
}
bool MySql::id_exist(QString ID){
    QString insert="select * from user_table where ID =:ID";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":ID",ID);
    sqlquery->exec();
    if(!sqlquery->next()){                 //数据库中不存在该id
        qDebug()<<"id_not_exist!";
        return false;
    }
    else{
        qDebug()<<"id_exist!";
        return true;
    }
}
bool MySql::insert_user_data(QString N_name,QString ID,QString password){
 QString insert = "INSERT INTO user_table VALUES(?,?,?,?,?)";
 sqlquery->prepare(insert);
 sqlquery->addBindValue(N_name);
 sqlquery->addBindValue(ID);
 sqlquery->addBindValue(password);
 sqlquery->addBindValue("zero");
 sqlquery->addBindValue("zero");




     if(sqlquery->exec()){
         qDebug()<<"Insert user_table OK";
         return true;
     }


 else{
     qDebug()<<"Insert user_table command error ;";
     return false;
 }
}
bool MySql::update_user_ipport(QString ID,QString IP,QString port){
        QString insert = "UPDATE user_table SET IP=:ip, port=:PORT where ID=:id";
        sqlquery->prepare(insert);
        sqlquery->bindValue(":ip",IP);
        sqlquery->bindValue(":PORT",port);
        sqlquery->bindValue(":id",ID);
       // qDebug()<<insert;
        if(sqlquery->exec()){
            qDebug()<<"update user_table with ipport OK";
            return true;
        }
        else{
        qDebug()<<"update user_table with ipport error ;";
        return false;
        }

}
bool MySql::update_user_nname(QString N_name,QString ID){
    QString insert = "UPDATE user_table SET N_name=:nname where ID=:id";
    sqlquery->prepare(insert);
    sqlquery->bindValue(":nname",N_name);
    sqlquery->bindValue(":id",ID);

    if(sqlquery->exec()){
        qDebug()<<"update user_table with N_name OK";
        return true;
    }
    else{
    qDebug()<<"update user_table with N_name error ;";
    return false;
    }
}
bool MySql::select_user_data(QString ID,QString password){

    QSqlQuery query;
    query.prepare("select ID,password from user_table where ID =:ID and password =:PWD");
   //   query.prepare("select * from user_table where ID =1234 and password =1234");
//    //query.bindValue(":N_name",N_name);
    query.bindValue(":ID",ID);
    query.bindValue(":PWD",password);
   // query.prepare("select * from user_table");
    query.exec();
    if(!query.next()){                 //数据库中不存在该账号和密码
      //  QMessageBox::warning(this,"","警告！账号或密码输入错误！");
        return false;
    }
    else{
     /*   QMessageBox::information(this,"登录成功","登录成功",QMessageBox::Ok);//存在该账号
        MainWindow *m=new MainWindow();
        m->show();                               //显示聊天界面
       this->close();  */                          //关闭登录界面
    return true;
    }
//    if(sqlquery->prepare(str_select)){
//        if(sqlquery->exec()){
//            while(sqlquery->next()){
//                qDebug()<<sqlquery->value(0).toString();
//                qDebug()<<sqlquery->value(1).toString();
//            }
//            qDebug()<<"Select userstable OK";
//        }

//    }
//    else{
//        qDebug()<<"Select table command error ;";
//    }
}
QString MySql::get_port(QString ID){
    QSqlQuery query;
    query.prepare("select port from user_table where ID =:ID");

    query.bindValue(":ID",ID);

   // query.prepare("select * from user_table");
    query.exec();
    if(!query.next()){                 //查询失败
        qDebug()<<"get_port defeat";
      return "error";

    }
    else{
        qDebug()<<"get_port success";
    return query.value(0).toString();
    }
}
bool MySql::set_ipport_zero(QString port){
    QString insert = "UPDATE user_table SET IP=:ip, port=:PORT where port=:Port";//不能原语句中直接=值
 //   "UPDATE user_table SET IP=:ip, port=:PORT where ID=:id"
    sqlquery->prepare(insert);
    sqlquery->bindValue(":ip","zero");
    sqlquery->bindValue(":PORT","zero");
    sqlquery->bindValue(":Port",port);
   // qDebug()<<insert;
    if(sqlquery->exec()){
        qDebug()<<"set ipport zero OK";
        return true;
    }


else{
    qDebug()<<"set ipport zero error ;";
    return false;
}
}
void MySql::drop_table(QString tablename){
    tablename = "userstable";
    QString str_drop = "DROP TABLE "+tablename;
    if(sqlquery->prepare(str_drop)){
        if(sqlquery->exec()){

            qDebug()<<"Drop userstable OK";
        }

    }
    else{
        qDebug()<<"Drop table command error ;";
    }
}

QList<QString> MySql::get_friend_name(QString user_id){
    QList<QString> friend_id_list=this->get_friend_id(user_id);
    QList<QString> friend_name_list;
    for(int i=0;i<friend_id_list.count();i++){
        friend_name_list.append(this->get_name_byid(friend_id_list.at(i)));
        qDebug()<<"name:"+friend_name_list.at(i);
    }
    return friend_name_list;

}

