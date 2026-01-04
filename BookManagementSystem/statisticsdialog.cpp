#include "statisticsdialog.h"
#include "ui_statisticsdialog.h"
#include "databasemanager.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>

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
    connect(ui->exportButton, &QPushButton::clicked, this, &StatisticsDialog::onExportClicked);

    // 初始加载数据
    onRefreshClicked();

    ui->bookRankingTable->verticalHeader()->setVisible(false);
    ui->readerRankingTable->verticalHeader()->setVisible(false);

    ui->bookRankingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerRankingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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


void StatisticsDialog::onExportClicked()
{
    // 1. 弹出文件保存对话框
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "导出数据",
                                                    "借阅统计.csv",
                                                    "CSV文件 (*.csv);;所有文件 (*)"
                                                    );

    if (fileName.isEmpty()) {
        return;
    }

    // 2. 创建QFile对象并尝试打开文件
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {  // 修正这里
        QMessageBox::warning(this, "错误", "无法创建文件！");  // 修正这里
        return;
    }

    // 3. 写入UTF-8 BOM并创建文本流
    file.write("\xEF\xBB\xBF");  // 添加UTF-8 BOM

    QTextStream out(&file);
    // 不再需要setCodec，因为已经添加了BOM

    // 4. 判断当前是哪个标签页
    QTableWidget *currentTable = nullptr;
    QString reportTitle;

    int currentTab = ui->tabWidget->currentIndex();
    if (currentTab == 0) { // 图书排行榜
        currentTable = ui->bookRankingTable;
        reportTitle = "图书借阅排行榜";
    } else { // 读者活跃榜
        currentTable = ui->readerRankingTable;
        reportTitle = "读者借阅活跃榜";
    }

    // 5. 写入文件内容
    out << reportTitle << "\n";
    out << "导出时间：" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n\n";

    // 写入表头
    for (int col = 0; col < currentTable->columnCount(); ++col) {
        out << currentTable->horizontalHeaderItem(col)->text();
        if (col < currentTable->columnCount() - 1) {
            out << ",";
        }
    }
    out << "\n";

    // 写入表格数据
    for (int row = 0; row < currentTable->rowCount(); ++row) {
        for (int col = 0; col < currentTable->columnCount(); ++col) {
            QTableWidgetItem *item = currentTable->item(row, col);
            QString text = item ? item->text() : "";

            // 处理特殊字符
            if (text.contains(',') || text.contains('"') || text.contains('\n')) {
                text.replace("\"", "\"\"");
                out << "\"" << text << "\"";
            } else {
                out << text;
            }

            if (col < currentTable->columnCount() - 1) {
                out << ",";
            }
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", QString("数据已成功导出到：\n%1").arg(fileName));
}
