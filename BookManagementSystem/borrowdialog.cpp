#include "borrowdialog.h"
#include "ui_borrowdialog.h"
#include "borrowmodel.h"
#include "borrowbookdialog.h"
#include "databasemanager.h"
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

    ui->statusComboBox->addItem("全部", "");

    ui->statusComboBox->addItem("借出", "借出");

    ui->statusComboBox->addItem("已还", "已还");

    ui->statusComboBox->addItem("逾期", "逾期");

    setupConnections();

    ui->returnBookButton->setEnabled(false);
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

    connect(ui->tableViewBorrow->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &BorrowDialog::onSelectionChanged);
}

void BorrowDialog::on_statusFilterChanged(int index)
{
    QString status = ui->statusComboBox->itemData(index).toString();
    qDebug() << "状态筛选变为:" << status;

    // 调用模型的筛选函数
    m_borrowModel->setFilterStatus(status);
}

void BorrowDialog::on_searchButton_clicked()
{
    QString keyword = ui->searchLineEdit->text().trimmed();
    qDebug() << "搜索关键词:" << keyword;

    // 调用模型的搜索函数
    m_borrowModel->searchRecords(keyword);
}

void BorrowDialog::on_resetButton_clicked()
{
    ui->searchLineEdit->clear();
    ui->statusComboBox->setCurrentIndex(0);

    // 重置筛选和搜索条件
    m_borrowModel->setFilterStatus("");
    m_borrowModel->searchRecords("");
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
    // 获取当前选中的借阅记录
    QModelIndex currentIndex = ui->tableViewBorrow->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要归还的借阅记录！");
        return;
    }

    int row = currentIndex.row();
    QVariantMap recordData = m_borrowModel->getRecordData(row);

    // 检查记录是否可以归还
    QString status = recordData["status"].toString();
    if (status == "已还") {
        QMessageBox::warning(this, "提示", "该书已归还！");
        return;
    }

    QString bookTitle = recordData["book_title"].toString();
    QString readerName = recordData["reader_name"].toString();
    QString borrowDate = recordData["borrow_date"].toString();

    // 确认还书对话框
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认还书",
                                  QString("确认归还以下图书？\n\n"
                                          "图书：%1\n"
                                          "读者：%2\n"
                                          "借阅日期：%3")
                                      .arg(bookTitle).arg(readerName).arg(borrowDate),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int recordId = recordData["id"].toInt();

        // 调用还书功能（需要在DatabaseManager中实现returnBook函数）
        if (DatabaseManager::instance().returnBook(recordId)) {
            QMessageBox::information(this, "成功", "还书成功！");

            // 刷新借阅记录
            m_borrowModel->refreshData();

            // 如果主窗口有更新状态栏的功能，可以触发更新
            // emit bookReturned();
        } else {
            QMessageBox::warning(this, "错误", "还书失败！");
        }
    }
}


void BorrowDialog::onSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (current.isValid()) {
        int row = current.row();
        QVariantMap recordData = m_borrowModel->getRecordData(row);
        QString status = recordData["status"].toString();

        // 根据借阅状态更新还书按钮状态
        ui->returnBookButton->setEnabled(status == "借出");

        // 可以在这里显示更多借阅详情
        QString bookTitle = recordData["book_title"].toString();
        QString readerName = recordData["reader_name"].toString();
        QString dueDate = recordData["due_date"].toString();

        // 更新状态栏或其他UI元素
        QString info = QString("选中: %1 - %2 (应还: %3)")
                           .arg(bookTitle).arg(readerName).arg(dueDate);
        // ui->statusLabel->setText(info); // 如果你有状态标签
    } else {
        ui->returnBookButton->setEnabled(false);
    }
}
