#ifndef ADDNEWFRIENDFORM_H
#define ADDNEWFRIENDFORM_H

#include <QWidget>
#include<tcpsocket.h>
namespace Ui {
class AddNewFriendForm;
}

class AddNewFriendForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewFriendForm(QWidget *parent = nullptr);
    ~AddNewFriendForm();
    void setLableText(int id,QString txt);//此处id指的是聊天界面上table的序号，而非用户id
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddNewFriendForm *ui;
};

#endif // ADDNEWFRIENDFORM_H
