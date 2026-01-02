#ifndef READERDIALOG_H
#define READERDIALOG_H

#include <QDialog>
#include <QVector>
#include <QVariantMap>

namespace Ui {
class ReaderDialog;
}

class ReaderModel;  // 前向声明

class ReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderDialog(QWidget *parent = nullptr);
    ~ReaderDialog();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_refreshButton_clicked();
    void on_closeButton_clicked();

    void on_searchButton_clicked();
    void on_resetButton_clicked();

private:
    Ui::ReaderDialog *ui;
    ReaderModel *m_readerModel;

    void setupConnections();
    void setupTableView();
    void updateStatus();
};

#endif // READERDIALOG_H
