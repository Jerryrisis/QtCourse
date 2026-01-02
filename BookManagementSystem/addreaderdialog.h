#ifndef ADDREADERDIALOG_H
#define ADDREADERDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class AddReaderDialog;
}

class AddReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddReaderDialog(QWidget *parent = nullptr);
    ~AddReaderDialog();

    QVariantMap getReaderData() const;
    void setReaderData(const QVariantMap &readerData);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddReaderDialog *ui;
};

#endif // ADDREADERDIALOG_H
