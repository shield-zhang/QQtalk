#ifndef GROUPFORM_H
#define GROUPFORM_H

#include <QWidget>
#include<tcpsocket.h>
namespace Ui {
class GroupForm;
}

class GroupForm : public QWidget
{
    Q_OBJECT

public:
    explicit GroupForm(QWidget *parent = nullptr);
    ~GroupForm();

    Ui::GroupForm *getUi() const;
    void setUi(Ui::GroupForm *value);

        QString getGro_id() const;
        void setGro_id(const QString &value);
        void setLableText(int id,QString txt);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GroupForm *ui;
    QString Gro_id="#";//用来标志p2p页面
};

#endif // GROUPFORM_H

//namespace Ui {
//class P2PForm;
//}

//class P2PForm : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit P2PForm(QWidget *parent = nullptr);
//    ~P2PForm();
//    void setLableText(int id,QString txt);//此处id指的是聊天界面上table的序号，而非用户id

//    Ui::P2PForm *getUi() const;
//    void setUi(Ui::P2PForm *value);

//    QString getP2p_friend_id() const;
//    void setP2p_friend_id(const QString &value);

//private slots:
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();

//private:
//    Ui::P2PForm *ui;
//    QString p2p_friend_id="#";//用来标志p2p页面
//};

//#endif // P2PFORM_H
