#include "mainwindow.h"
#include <QApplication>
#include "databasemanager.h"  // 添加这行

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 初始化数据库
    if (!DatabaseManager::instance().initializeDatabase()) {
        qDebug() << "Failed to initialize database!";
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
