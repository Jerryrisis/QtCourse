#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"
#include "databasemanager.h"
#include <QDebug>

StatisticsDialog::StatisticsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsDialog)
{
    ui->setupUi(this);
    setWindowTitle("借阅数据统计");

    // 设置表格属性
    ui->bookRankingTable->setColumnCount(6);
    ui->bookRankingTable->setHorizontalHeaderLabels(
        QStringList() << "排名" << "书名" << "作者" << "ISBN" << "借阅次数" << "可借数量");

    ui->readerRankingTable->setColumnCount(6);
    ui->readerRankingTable->setHorizontalHeaderLabels(
        QStringList() << "排名" << "姓名" << "读者编号" << "性别" << "总借阅次数" << "在借数量");

    // 设置表格样式
    ui->bookRankingTable->horizontalHeader()->setStretchLastSection(true);
    ui->readerRankingTable->horizontalHeader()->setStretchLastSection(true);

    ui->bookRankingTable->setAlternatingRowColors(true);
    ui->readerRankingTable->setAlternatingRowColors(true);

    // 连接信号槽
    connect(ui->refreshButton, &QPushButton::clicked, this, &StatisticsDialog::onRefreshClicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // 初始加载数据
    onRefreshClicked();

    ui->bookRankingTable->verticalHeader()->setVisible(false);
    ui->readerRankingTable->verticalHeader()->setVisible(false);

}

StatisticsDialog::~StatisticsDialog()
{
    delete ui;
}

void StatisticsDialog::onRefreshClicked()
{
    loadBookRanking();
    loadReaderRanking();
    updateSummary();
}

void StatisticsDialog::loadBookRanking()
{
    ui->bookRankingTable->setRowCount(0);

    QVector<QVariantMap> bookRanking = DatabaseManager::instance().getBookBorrowRanking(20);

    for (int i = 0; i < bookRanking.size(); ++i) {
        const QVariantMap &book = bookRanking.at(i);

        ui->bookRankingTable->insertRow(i);

        // 排名
        ui->bookRankingTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

        // 书名
        ui->bookRankingTable->setItem(i, 1, new QTableWidgetItem(book["title"].toString()));

        // 作者
        ui->bookRankingTable->setItem(i, 2, new QTableWidgetItem(book["author"].toString()));

        // ISBN
        ui->bookRankingTable->setItem(i, 3, new QTableWidgetItem(book["isbn"].toString()));

        // 借阅次数
        int borrowCount = book["borrow_count"].toInt();
        QTableWidgetItem *borrowItem = new QTableWidgetItem(QString::number(borrowCount));
        if (borrowCount > 0) {
            borrowItem->setForeground(Qt::blue);
        }
        ui->bookRankingTable->setItem(i, 4, borrowItem);

        // 可借数量
        int availableCount = book["available_count"].toInt();
        QTableWidgetItem *availableItem = new QTableWidgetItem(QString::number(availableCount));
        if (availableCount == 0) {
            availableItem->setForeground(Qt::red);
        } else if (availableCount < 3) {
            availableItem->setForeground(Qt::darkYellow);
        }
        ui->bookRankingTable->setItem(i, 5, availableItem);
    }

    // 调整列宽
    ui->bookRankingTable->resizeColumnsToContents();
}

void StatisticsDialog::loadReaderRanking()
{
    ui->readerRankingTable->setRowCount(0);

    QVector<QVariantMap> readerRanking = DatabaseManager::instance().getReaderBorrowRanking(20);

    for (int i = 0; i < readerRanking.size(); ++i) {
        const QVariantMap &reader = readerRanking.at(i);

        ui->readerRankingTable->insertRow(i);

        // 排名
        ui->readerRankingTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

        // 姓名
        ui->readerRankingTable->setItem(i, 1, new QTableWidgetItem(reader["name"].toString()));

        // 读者编号
        ui->readerRankingTable->setItem(i, 2, new QTableWidgetItem(reader["reader_id"].toString()));

        // 性别
        ui->readerRankingTable->setItem(i, 3, new QTableWidgetItem(reader["gender"].toString()));

        // 总借阅次数
        int totalBorrow = reader["borrow_count"].toInt();
        QTableWidgetItem *totalItem = new QTableWidgetItem(QString::number(totalBorrow));
        if (totalBorrow >= 10) {
            totalItem->setForeground(Qt::darkGreen);
        } else if (totalBorrow >= 5) {
            totalItem->setForeground(Qt::blue);
        }
        ui->readerRankingTable->setItem(i, 4, totalItem);

        // 在借数量
        int activeCount = reader["active_count"].toInt();
        QTableWidgetItem *activeItem = new QTableWidgetItem(QString::number(activeCount));
        if (activeCount > 0) {
            activeItem->setForeground(Qt::darkRed);
        }
        ui->readerRankingTable->setItem(i, 5, activeItem);
    }

    // 调整列宽
    ui->readerRankingTable->resizeColumnsToContents();
}

void StatisticsDialog::updateSummary()
{
    // 获取一些汇总统计
    int totalBooks = DatabaseManager::instance().getBookCount();
    int totalReaders = DatabaseManager::instance().getReaderCount();
    int activeBorrows = DatabaseManager::instance().getBorrowedCount();

    // 可以在这里添加更多统计信息展示
    qDebug() << "统计信息更新: 图书" << totalBooks << "本, 读者" << totalReaders
             << "人, 在借" << activeBorrows << "本";
}
