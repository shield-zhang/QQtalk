#ifndef MYSQL_H
#define MYSQL_H

#include <QObject>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/qsqlquery.h>
#include <QDebug>
class MySql : public QObject
{
    Q_OBJECT
public:
    static MySql* gethand(){
        if(sqlhand==nullptr)
            sqlhand= new MySql();
        return sqlhand;
    }
    void open_database(); //打开或创建数据库
    void create_table();    //打开或创建数据表
    bool delete_friend(QString,QString);
    QString get_id_byport(QString);
    bool insert_sign_data(QString user_id,QString sign_text);
    bool update_sign_data(QString user_id,QString sign_text);
    QString get_sign_data(QString user_id);
    bool insert_group_data(QString group_id,QString user_id);
    QList<QString> get_groupmember_id(QString group_id);
    bool friend_exist(QString user_id,QString friend_id);
    bool insert_friend_data(QString user_id,QString friend_id);
    QList<QString> get_friend_id(QString ID);//根据id获取好友id
    QString get_name_byid(QString ID); //根据id获取昵称
    bool id_exist(QString ID);
    bool insert_user_data(QString N_name,QString ID,QString password);
   // bool insert_user_data_ip(QString ID,QString IP, QString port);
    bool update_user_ipport(QString ID,QString IP,QString port);
    bool update_user_nname(QString N_name,QString ID);
    bool select_user_data(QString ID,QString password);
    QList<QString> get_friend_name(QString user_id);
    void select_data();
    QString get_port(QString ID);
    bool set_ipport_zero(QString port);
    void drop_table(QString tablename);
    bool delete_group(QString user_id,QString group_id);
    QList<QString> get_groupid_byuser(QString user_id);


    QString sendport;
    QString goalport;
    int flag0=0;
    int size;

private:
    explicit MySql(QObject *parent = nullptr);
    static MySql* sqlhand;
    QSqlDatabase database; //建立数据库对象
    QSqlQuery* sqlquery; //创建该对象用于数据库操作
    QStringList tablelist;//存储数据库所包含数据表列表
signals:


//    QString  get_my_name(long long my_id);//获取自己名字        get_name_byid
//        QList<long long> get_friend_id(long long my_id);//根据自己的id，获取所有好友的id  get_friend_id
//        QString get_friend_name(long long friend_id);//根据好友id获取好友昵称   get_name_byid 与1是同一个
//        bool find_friend(long long id);//对当前id进行查询，是否存在:若存在则返回true，否则返回false.  //好友是否存在？ id_exist
//        void  add_friend_ok(long long friend_id);//同意添加好友，添加好友到好友关系表  //inset_friend
//        void  set_name(QString my_name,long long my_id);//设置自己昵称              update_user_nname
//        void set_words(QString my_words,long long my_id);//设置自己个性签名**

//        bool create_group(long long group_id);//创建一个群聊** (这里数据库建一张空的群聊表，group_id和成员的id为主键，);创建成功true，失败false
//        void add_group_member(long long  friend_id,long long group_id);//添加成员id到群聊表group_id**
//        QList<long long> get_group_ids(long long my_id);//根据自己的id，获取所有加入群聊成员的id ** //所有群聊一张表？
//        QString get_friend_picture(long long friend_id);//获取好友头像**
//        QString get_friend_words(long long friend_id);//获取好友个性签名**
};

#endif // MYSQL_H
