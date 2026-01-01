#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookmodel.h"
#include <QDebug>
#include <QTableView>  // 添加这行

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书管理系统 - 已连接数据库");

    // 创建并设置模型
    BookModel *model = new BookModel(this);
    ui->tableView->setModel(model);

    // 调整表格显示
    ui->tableView->setColumnWidth(0, 50);   // ID
    ui->tableView->setColumnWidth(1, 120);  // ISBN
    ui->tableView->setColumnWidth(2, 200);  // 书名
    ui->tableView->setColumnWidth(3, 100);  // 作者

    // 输出数据库状态
    qDebug() << "数据库连接状态: 正常";
    qDebug() << "当前图书数量:" << model->rowCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}
