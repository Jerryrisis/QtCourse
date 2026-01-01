#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BookModel *m_bookModel;

    void setupConnections();
    void updateStatusBar();

private slots:
    // 图书管理
    void onAddBook();
    void onEditBook();
    void onDeleteBook();
    void onRefresh();

    // 搜索功能
    void onSearch();
    void onResetSearch();

    // 读者管理
    void onAddReader();
    void onManageReaders();

    // 借阅管理
    void onBorrowBook();
    void onReturnBook();

    // 文件操作
    void onExportData();

    // 帮助
    void onAbout();

};
#endif // MAINWINDOW_H
