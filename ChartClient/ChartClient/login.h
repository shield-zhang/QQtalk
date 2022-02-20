#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "tcpsocket.h"
#include <QHostAddress>
#include "qhostinfo.h"
#include <QSettings>
#include<QWidget>
struct logins{
    QString id;
    QString PWD;
    QString rem;
};

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
     void closegif();
    static Login* gethand(){
        if(log == nullptr)
            log = new Login();
        return log;
    }
    ~Login();
  //  QString		AutoLogin;
    QString		username;//id
    QString		passwd;
    QString		RemeberPasswd;
    bool		remeberPasswd;
    bool		autologin;
    void		savecfg();
    void		loadcfg();
    void        LoginInit();
    int flag;
    QList<logins> login_list;
private slots:


    void on_loginButton_clicked();

    void on_registerButton_clicked();



 //   void on_checkBox_clicked();


    void on_lineEdit_2_textChanged(const QString &arg1);



    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::Login *ui;
    explicit Login(QWidget *parent = nullptr);
    static Login* log;




};

#endif // LOGIN_H
