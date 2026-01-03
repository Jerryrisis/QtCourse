#ifndef BORROWBOOKDIALOG_H
#define BORROWBOOKDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class BorrowBookDialog;
}

class BorrowBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrowBookDialog(QWidget *parent = nullptr);
    ~BorrowBookDialog();

    // 获取借阅信息
    int getSelectedReaderId() const;
    int getSelectedBookId() const;
    QDate getDueDate() const;

private slots:
    void onReaderSelectionChanged(int index);
    void onBookSelectionChanged(int index);
    void onRefreshReadersClicked();
    void onBorrowButtonClicked();

private:
    Ui::BorrowBookDialog *ui;
    QMap<int, QVariantMap> m_readers;  // 存储读者数据（ID -> 数据）
    QMap<int, QVariantMap> m_books;    // 存储图书数据（ID -> 数据）

    void loadReaders();
    void loadAvailableBooks();
    void updateReaderInfo(int readerId);
    void updateBookInfo(int bookId);
    void validateForm();
};

#endif // BORROWBOOKDIALOG_H
