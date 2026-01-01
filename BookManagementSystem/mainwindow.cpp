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

    // 方案1：直接创建TableView
    QTableView *tableView = new QTableView(this);
    setCentralWidget(tableView);  // 将TableView设置为主窗口的中心部件

    // 方案2：测试BookModel
    BookModel *model = new BookModel(this);
    tableView->setModel(model);

    // 调整列宽
    tableView->setColumnWidth(0, 50);
    tableView->setColumnWidth(1, 150);
    tableView->setColumnWidth(2, 200);

    qDebug() << "测试成功！共有" << model->rowCount() << "本书";
}

MainWindow::~MainWindow()
{
    delete ui;
}
