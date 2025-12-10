#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void on_btSignIn_clicked();

signals:
    void loginsuccess();
    void loginfail();

private:
    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
