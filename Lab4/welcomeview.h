#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>

namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
    void goDepartmentView();
    void goDoctorView();
    void goPatientView();
private:
    Ui::WelcomeView *ui;
};

#endif // WELCOMEVIEW_H
