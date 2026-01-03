#include "borrowdialog.h"
#include "ui_borrowdialog.h"
#include "borrowmodel.h"
#include "borrowbookdialog.h"  // 添加借书对话框头文件
#include <QMessageBox>
#include <QDebug>

BorrowDialog::BorrowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowDialog),
    m_borrowModel(new BorrowModel(this))
{
    ui->setupUi(this);
    setWindowTitle("借阅管理");

    // 设置模型和表格
    ui->tableViewBorrow->setModel(m_borrowModel);
    ui->tableViewBorrow->verticalHeader()->setVisible(false); // 隐藏行号
    ui->tableViewBorrow->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 设置列宽
    ui->tableViewBorrow->setColumnWidth(0, 60);   // 记录ID
    ui->tableViewBorrow->setColumnWidth(1, 150);  // 图书
    ui->tableViewBorrow->setColumnWidth(2, 100);  // 读者
    ui->tableViewBorrow->setColumnWidth(3, 90);   // 借书日期
    ui->tableViewBorrow->setColumnWidth(4, 90);   // 应还日期
    ui->tableViewBorrow->setColumnWidth(5, 90);   // 还书日期
    ui->tableViewBorrow->setColumnWidth(6, 60);   // 状态
    ui->tableViewBorrow->setColumnWidth(7, 70);   // 逾期天数

    setupConnections();
}

BorrowDialog::~BorrowDialog()
{
    delete ui;
}

void BorrowDialog::setupConnections()
{
    // 连接筛选框
    connect(ui->statusComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BorrowDialog::on_statusFilterChanged);

    // 连接搜索和重置按钮
    //connect(ui->searchButton, &QPushButton::clicked, this, &BorrowDialog::on_searchButton_clicked);
    //connect(ui->resetButton, &QPushButton::clicked, this, &BorrowDialog::on_resetButton_clicked);

    // 连接底部按钮
    //connect(ui->refreshButton, &QPushButton::clicked, this, &BorrowDialog::on_refreshButton_clicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);
    //connect(ui->borrowBookButton, &QPushButton::clicked, this, &BorrowDialog::on_borrowBookButton_clicked);
    //connect(ui->returnBookButton, &QPushButton::clicked, this, &BorrowDialog::on_returnBookButton_clicked);
}

void BorrowDialog::on_statusFilterChanged(int index)
{
    QString status = ui->statusComboBox->itemData(index).toString();
    qDebug() << "状态筛选变为:" << status;

    // 本次先简单实现全部显示，筛选逻辑下次提交完善
    // m_borrowModel->setFilterStatus(status);
    m_borrowModel->refreshData();
}

void BorrowDialog::on_searchButton_clicked()
{
    QString keyword = ui->searchLineEdit->text().trimmed();  // 修复这里：->text()
    qDebug() << "搜索关键词:" << keyword;

    // 本次先简单刷新，搜索逻辑下次提交完善
    // m_borrowModel->searchRecords(keyword);
    m_borrowModel->refreshData();
}

void BorrowDialog::on_resetButton_clicked()
{
    ui->searchLineEdit->clear();
    ui->statusComboBox->setCurrentIndex(0);
    m_borrowModel->refreshData();
}

void BorrowDialog::on_refreshButton_clicked()
{
    m_borrowModel->refreshData();
    QMessageBox::information(this, "提示", "借阅记录已刷新。");  // 修复这里：QMessageBox::information
}

void BorrowDialog::on_borrowBookButton_clicked()
{
    // 打开借书对话框
    BorrowBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 借书成功后刷新借阅记录
        m_borrowModel->refreshData();
        QMessageBox::information(this, "成功", "借书操作已完成！");
    }
}

void BorrowDialog::on_returnBookButton_clicked()
{
    // 先提示功能待实现
    QMessageBox::information(this, "功能预览", "还书功能将在下次提交实现。");
}
