#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bookmodel.h"
#include <QDebug>
#include <QTableView>
#include <QMessageBox>
#include <QShortcut>
#include "databasemanager.h"
#include "addbookdialog.h"
#include "readerdialog.h"
#include "addreaderdialog.h"
#include "readermodel.h"
#include "borrowdialog.h"
#include "statisticsdialog.h"

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

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);

    this->setStyleSheet(R"(
/* ===== MainWindow 主窗口基础样式 ===== */
QMainWindow {
    background-color: #f8fafc; /* 柔和背景色 */
    font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
    font-size: 13px;
}

/* ===== 菜单栏美化 ===== */
QMenuBar {
    background-color: #2c3e50; /* 深蓝背景 */
    color: white;
    padding: 4px 0px;
    font-weight: 500;
    border-bottom: 2px solid #3498db;
}

QMenuBar::item {
    background-color: transparent;
    padding: 6px 12px;
    border-radius: 3px;
    margin: 0 1px;
}

QMenuBar::item:selected {
    background-color: #3498db; /* 选中项蓝色 */
}

QMenuBar::item:pressed {
    background-color: #2980b9;
}

/* 下拉菜单样式 */
QMenu {
    background-color: white;
    border: 1px solid #bdc3c7;
    border-radius: 6px;
    padding: 4px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
}

QMenu::item {
    padding: 8px 24px 8px 12px; /* 右侧留出空间 */
    border-radius: 4px;
    min-width: 120px;
}

QMenu::item:selected {
    background-color: #3498db;
    color: white;
}

QMenu::separator {
    height: 1px;
    background-color: #ecf0f1;
    margin: 4px 8px;
}

/* ===== 搜索区域美化 ===== */
/* 水平布局容器 */
QWidget#centralwidget > QVBoxLayout > QHBoxLayout {
    spacing: 8px; /* 控件间距 */
    margin: 12px 4px 8px 4px;
}

/* "搜索："标签 */
QLabel#label {
    color: #2c3e50;
    font-weight: bold;
    padding: 8px 0px;
    min-width: 40px;
}

/* 搜索输入框 */
QLineEdit#searchLineEdit {
    border: 2px solid #bdc3c7;
    border-radius: 6px;
    padding: 8px 12px;
    background-color: white;
    font-size: 13px;
    min-height: 16px;
    selection-background-color: #3498db;
}

QLineEdit#searchLineEdit:focus {
    border-color: #3498db;
    background-color: #f8fafc;
}

/* 搜索按钮 - 蓝色主题 */
QPushButton#searchButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #3498db, stop:1 #2980b9);
    border: 2px solid #2980b9;
    border-radius: 6px;
    color: white;
    padding: 8px 16px;
    font-weight: bold;
    min-width: 70px;
}

QPushButton#searchButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #3cb0fd, stop:1 #3498db);
    border-color: #3498db;
}

QPushButton#searchButton:pressed {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #2980b9, stop:1 #3498db);
    padding-top: 9px;
    padding-bottom: 7px;
}

/* 重置按钮 - 灰色主题 */
QPushButton#resetButton {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #95a5a6, stop:1 #7f8c8d);
    border: 2px solid #7f8c8d;
    border-radius: 6px;
    color: white;
    padding: 8px 16px;
    font-weight: bold;
    min-width: 70px;
}

QPushButton#resetButton:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #a0b0b2, stop:1 #95a5a6);
    border-color: #95a5a6;
}

QPushButton#resetButton:pressed {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                              stop:0 #7f8c8d, stop:1 #95a5a6);
}

/* ===== 表格视图美化 ===== */
QTableView#tableView {
    background-color: white;
    alternate-background-color: #f8f9fa; /* 交替行颜色 */
    selection-background-color: #3498db; /* 选中行背景 */
    selection-color: white; /* 选中行文字 */
    gridline-color: #e9ecef; /* 网格线颜色 */
    border: 1px solid #dee2e6;
    border-radius: 6px;
    outline: 0; /* 去除焦点虚线框 */
    font-size: 12.5px;
}

/* 表头样式 */
QHeaderView::section {
    background-color: #3498db; /* 蓝色表头 */
    color: white;
    padding: 10px 8px;
    border: none;
    border-right: 1px solid #2980b9; /* 列分割线 */
    font-weight: bold;
    font-size: 13px;
}

QHeaderView::section:last {
    border-right: none; /* 最后一列无右边框 */
}

/* 隐藏垂直表头（行号） */
QHeaderView#verticalHeader {
    background-color: transparent;
    width: 0px; /* 设置为0宽度，完全隐藏 */
    max-width: 0px;
    min-width: 0px;
}

/* 表格项样式 */
QTableView::item {
    padding: 6px 8px;
    border-bottom: 1px solid #f1f3f4;
}

QTableView::item:selected {
    background-color: #2980b9; /* 选中项加深蓝色 */
}

/* 奇偶行不同颜色 */
QTableView::item:alternate {
    background-color: #f8f9fa;
}

/* ===== 状态栏美化 ===== */
QStatusBar {
    background-color: #2c3e50; /* 与菜单栏统一 */
    color: #ecf0f1;
    padding: 4px 8px;
    font-size: 12px;
    border-top: 1px solid #34495e;
}

QStatusBar::item {
    border: none;
    background-color: transparent;
}
)");
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

    connect(ui->tableView, &QTableView::doubleClicked, this, &MainWindow::onEditBook);

    QShortcut *deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    connect(deleteShortcut, &QShortcut::activated, this, &MainWindow::onDeleteBook);
    connect(ui->actionStatistics, &QAction::triggered, this, &MainWindow::onStatistics);
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
    // 创建并显示对话框
    AddBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 用户点击了“确定”
        QVariantMap newBookData = dialog.getBookData();

        // 通过 BookModel 添加到数据库并刷新视图
        if (m_bookModel->addBook(newBookData)) {
            ui->statusbar->showMessage("添加图书成功！", 3000);
            updateStatusBar(); // 更新状态栏统计信息
        } else {
            QMessageBox::warning(this, "错误", "添加图书失败，可能是ISBN重复或数据库错误。");
        }
    }
    // 如果用户点击“取消”，则什么都不做
}

void MainWindow::onEditBook()
{
    // 获取当前选中的行
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的图书！");
        return;
    }

    int row = currentIndex.row();
    QVariantMap bookData = m_bookModel->getBookData(row);

    if (bookData.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取图书数据！");
        return;
    }

    // 创建编辑对话框
    AddBookDialog dialog(this, true);  // true表示编辑模式
    dialog.setBookData(bookData);
    dialog.setISBNEditable(false);  // 编辑时ISBN不可修改

    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap updatedData = dialog.getBookData();

        // 确保ID保持不变
        updatedData["id"] = bookData["id"];

        if (m_bookModel->updateBook(row, updatedData)) {
            QMessageBox::information(this, "成功", "图书信息更新成功！");
            updateStatusBar();
        } else {
            QMessageBox::warning(this, "错误", "更新图书信息失败！");
        }
    }
}

void MainWindow::onDeleteBook()
{
    // 获取当前选中的行
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的图书！");
        return;
    }

    int row = currentIndex.row();
    QVariantMap bookData = m_bookModel->getBookData(row);

    if (bookData.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取图书数据！");
        return;
    }

    QString bookTitle = bookData["title"].toString();
    QString bookISBN = bookData["isbn"].toString();
    int availableCount = bookData["available_count"].toInt();
    int totalCount = bookData["total_count"].toInt();

    // 检查图书是否可删除
    if (availableCount < totalCount) {
        QMessageBox::warning(this, "无法删除",
                             QString("图书《%1》目前有 %2 本被借出，无法删除！\n请先确保所有图书都已归还。")
                                 .arg(bookTitle).arg(totalCount - availableCount));
        return;
    }

    // 确认删除对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除图书《%1》吗？\nISBN: %2\n\n删除后无法恢复！")
                                      .arg(bookTitle).arg(bookISBN),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 执行删除操作
        if (m_bookModel->removeBook(row)) {
            QMessageBox::information(this, "成功", QString("图书《%1》已成功删除！").arg(bookTitle));
            updateStatusBar();

            // 如果当前有搜索关键词，清空搜索
            if (!ui->searchLineEdit->text().isEmpty()) {
                ui->searchLineEdit->clear();
                onResetSearch();
            }
        } else {
            QMessageBox::warning(this, "错误", "删除图书失败！可能是该图书有借阅记录。");
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
// 添加读者
void MainWindow::onAddReader()
{
    AddReaderDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap readerData = dialog.getReaderData();

        // 创建临时ReaderModel来添加读者
        ReaderModel readerModel(this);
        if (readerModel.addReader(readerData)) {
            QMessageBox::information(this, "成功", "读者添加成功！");
            updateStatusBar();  // 更新主窗口状态栏
        } else {
            QMessageBox::warning(this, "错误", "读者添加失败！");
        }
    }
}

// 管理读者
void MainWindow::onManageReaders()
{
    // 打开读者管理对话框
    ReaderDialog dialog(this);
    dialog.exec();
}

// 借阅管理功能
void MainWindow::onBorrowBook()
{
    BorrowDialog dialog(this);
    dialog.exec(); // 打开借阅管理总界面
    // 注意：这里未来可以优化为直接弹出“借书”子对话框
}

void MainWindow::onReturnBook()
{
    // 暂时也打开总界面，后续可优化
    BorrowDialog dialog(this);
    dialog.exec();
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
                       "作者: 林俊韬\n"
                       "学号：2023414300210\n\n"
                       "功能说明:\n"
                       "• 图书信息管理\n"
                       "• 读者信息管理\n"
                       "• 图书借阅管理\n"
                       "• 图书借阅排行与读者排行（可导出）");
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


void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    if (index.isValid()) {
        QMenu contextMenu(this);

        // 添加菜单项
        contextMenu.addAction(ui->actionEditBook);
        contextMenu.addAction(ui->actionDeleteBook);
        contextMenu.addSeparator();
        contextMenu.addAction(ui->actionRefresh);

        // 显示菜单
        contextMenu.exec(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::onStatistics()
{
    StatisticsDialog dialog(this);
    dialog.exec();
}
