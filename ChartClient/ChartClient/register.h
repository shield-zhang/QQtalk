#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "tcpsocket.h"
namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    static Register* gethand(){
        if(reg == nullptr)
            reg = new Register();
        return reg;
    }
    ~Register();

private slots:

    void on_regButton_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_4_textEdited(const QString &arg1);

private:
    Ui::Register *ui;
    explicit Register(QWidget *parent = nullptr);
    static Register* reg;
};

#endif // REGISTER_H
