#ifndef BORROWDIALOG_H
#define BORROWDIALOG_H

#include <QDialog>

namespace Ui {
class BorrowDialog;
}
class BorrowModel;

class BorrowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowDialog(QWidget *parent = nullptr);
    ~BorrowDialog();

private slots:
    void on_statusFilterChanged(int index);
    void on_searchButton_clicked();
    void on_resetButton_clicked();
    void on_refreshButton_clicked();
    void on_borrowBookButton_clicked();
    void on_returnBookButton_clicked();
    void onSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::BorrowDialog *ui;
    BorrowModel *m_borrowModel;
    void setupConnections();
};

#endif // BORROWDIALOG_H
