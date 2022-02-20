#ifndef LISTFORM_H
#define LISTFORM_H

#include <QWidget>
#include <QMainWindow>
#include <QListWidgetItem>
#include<p2pform.h>
#include<ui_p2pform.h>
#include<QDebug>
#include<QList>
#include<tcpsocket.h>
#include "ui_listform.h"
#include "groupform.h"
#include <ui_groupform.h>
#include<imagechange.h>
//好友列表
extern int number;
namespace Ui {
class ListForm;
}

class ListForm : public QWidget
{
    Q_OBJECT

public:
    static ListForm* gethand(){
        if(listform==nullptr){
            listform = new ListForm();
        }
        return listform;
    }
    void init();
    void friend_red(QString sender_id);
    void friend_on_color(QString friend_id);
    void friend_off_color(QString friend_id);   
    void group_red(QString sender_id);
    Images_Change ic;
    ~ListForm();



    static int count_i;//初始化值0
    P2PForm p2pforms[100];
   QList<P2PForm*> p2plist;//作为储存p2p页面的一个列表


    QList<QString> groups_id;
   static int group_i;//群组个数
    GroupForm groupforms[100];//群组实例
    QList<GroupForm*> grouplist;//群组列表
   Ui::ListForm *getUi() const;
   void setUi(Ui::ListForm *value);

private slots:
   void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();



    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_GroupWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_2_clicked();

private:
    Ui::ListForm *ui;
    explicit ListForm(QWidget *parent = nullptr);
    static ListForm* listform;
};

#endif // LISTFORM_H
