#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookmodel.h"
#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置窗口标题
    setWindowTitle("图书管理系统 - 已连接数据库");

    // 创建并设置模型
    m_bookModel = new BookModel(this);
    ui->tableView->setModel(m_bookModel);

    // 调整表格显示
    ui->tableView->setColumnWidth(0, 50);   // ID
    ui->tableView->setColumnWidth(1, 120);  // ISBN
    ui->tableView->setColumnWidth(2, 200);  // 书名
    ui->tableView->setColumnWidth(3, 100);  // 作者
    ui->tableView->verticalHeader()->setVisible(false);

    // 输出数据库状态
    qDebug() << "数据库连接状态: 正常";
    qDebug() << "当前图书数量:" << m_bookModel->rowCount();

    setupConnections();  // 连接信号和槽
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections()
{
    // 搜索功能
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::onResetSearch);
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

    // 菜单项连接
    connect(ui->actionAddBook, &QAction::triggered, this, &MainWindow::onAddBook);
    connect(ui->actionDeleteBook, &QAction::triggered, this, &MainWindow::onDeleteBook);
    connect(ui->actionEditBook, &QAction::triggered, this, &MainWindow::onEditBook);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::onRefresh);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->actionAddReader, &QAction::triggered, this, &MainWindow::onAddReader);
    connect(ui->actionManageReaders, &QAction::triggered, this, &MainWindow::onManageReaders);
    connect(ui->actionBorrowBook, &QAction::triggered, this, &MainWindow::onBorrowBook);
    connect(ui->actionReturnBook, &QAction::triggered, this, &MainWindow::onReturnBook);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::onSearch()
{
    QString keyword = ui->searchLineEdit->text().trimmed();
    if (!keyword.isEmpty()) {
        m_bookModel->searchBooks(keyword);
        ui->statusbar->showMessage(QString("搜索: %1").arg(keyword), 3000);
    }
    // 注意：这里还需要更新状态栏
    updateStatusBar();
}

void MainWindow::onResetSearch()
{
    ui->searchLineEdit->clear();
    m_bookModel->searchBooks("");  // 清空搜索条件
    ui->statusbar->showMessage("已重置搜索", 2000);
    updateStatusBar();
}



// 图书管理功能
void MainWindow::onAddBook()
{
    QMessageBox::information(this, "提示", "添加图书功能待实现");
}

void MainWindow::onEditBook()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的图书！");
        return;
    }
    QMessageBox::information(this, "提示", "编辑图书功能待实现");
}

void MainWindow::onDeleteBook()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的图书！");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  "确定要删除选中的图书吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = currentIndex.row();
        if (m_bookModel->removeBook(row)) {
            QMessageBox::information(this, "成功", "图书删除成功！");
            updateStatusBar();
        } else {
            QMessageBox::warning(this, "错误", "删除失败！");
        }
    }
}

void MainWindow::onRefresh()
{
    m_bookModel->refreshData();
    ui->statusbar->showMessage("数据已刷新", 2000);
    updateStatusBar();
}

// 读者管理功能
void MainWindow::onAddReader()
{
    QMessageBox::information(this, "提示", "添加读者功能待实现");
}

void MainWindow::onManageReaders()
{
    QMessageBox::information(this, "提示", "管理读者功能待实现");
}

// 借阅管理功能
void MainWindow::onBorrowBook()
{
    QMessageBox::information(this, "提示", "借书功能待实现");
}

void MainWindow::onReturnBook()
{
    QMessageBox::information(this, "提示", "还书功能待实现");
}

// 文件操作
void MainWindow::onExportData()
{
    QMessageBox::information(this, "提示", "导出数据功能待实现");
}

// 帮助
void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于图书管理系统",
                       "图书与借阅管理系统\n\n"
                       "版本: 1.0\n"
                       "作者: 学生作业项目\n\n"
                       "功能说明:\n"
                       "• 图书信息管理\n"
                       "• 读者信息管理\n"
                       "• 图书借阅管理\n"
                       "• 数据查询与导出");
}

// 状态栏更新
void MainWindow::updateStatusBar()
{
    int totalBooks = DatabaseManager::instance().getBookCount();
    int totalReaders = DatabaseManager::instance().getReaderCount();
    int borrowedCount = DatabaseManager::instance().getBorrowedCount();

    QString status = QString("图书总数: %1 | 读者总数: %2 | 已借出: %3")
                         .arg(totalBooks).arg(totalReaders).arg(borrowedCount);

    ui->statusbar->showMessage(status);
}

