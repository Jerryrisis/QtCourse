#include "readerdialog.h"
#include "ui_readerdialog.h"
#include "readermodel.h"
#include "addreaderdialog.h"
#include <QMessageBox>
#include <QDebug>

ReaderDialog::ReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderDialog),
    m_readerModel(new ReaderModel(this))
{
    ui->setupUi(this);
    setWindowTitle("读者管理");

    setupTableView();
    setupConnections();
    updateStatus();
}

ReaderDialog::~ReaderDialog()
{
    delete ui;
}

void ReaderDialog::setupTableView()
{
    ui->tableViewReaders->setModel(m_readerModel);

    // 调整列宽
    ui->tableViewReaders->setColumnWidth(0, 60);   // ID
    ui->tableViewReaders->setColumnWidth(1, 100);  // 读者编号
    ui->tableViewReaders->setColumnWidth(2, 80);   // 姓名
    ui->tableViewReaders->setColumnWidth(3, 60);   // 性别
    ui->tableViewReaders->setColumnWidth(4, 120);  // 电话
    ui->tableViewReaders->setColumnWidth(5, 150);  // 邮箱
    ui->tableViewReaders->setColumnWidth(6, 150);  // 地址
    ui->tableViewReaders->setColumnWidth(7, 100);  // 注册日期

    // 隐藏行号
    ui->tableViewReaders->verticalHeader()->setVisible(false);

    // 设置选择行为
    ui->tableViewReaders->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewReaders->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ReaderDialog::setupConnections()
{
    //connect(ui->addButton, &QPushButton::clicked, this, &ReaderDialog::on_addButton_clicked);
    //connect(ui->editButton, &QPushButton::clicked, this, &ReaderDialog::on_editButton_clicked);
    //connect(ui->deleteButton, &QPushButton::clicked, this, &ReaderDialog::on_deleteButton_clicked);
    //connect(ui->refreshButton, &QPushButton::clicked, this, &ReaderDialog::on_refreshButton_clicked);
    connect(ui->closeButton, &QPushButton::clicked, this, &ReaderDialog::on_closeButton_clicked);

    //connect(ui->searchButton, &QPushButton::clicked, this, &ReaderDialog::on_searchButton_clicked);
    //connect(ui->resetButton, &QPushButton::clicked, this, &ReaderDialog::on_resetButton_clicked);
}

void ReaderDialog::on_addButton_clicked()
{
    AddReaderDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap readerData = dialog.getReaderData();

        if (m_readerModel->addReader(readerData)) {
            QMessageBox::information(this, "成功", "读者添加成功！");
            updateStatus();
        } else {
            QMessageBox::warning(this, "错误", "读者添加失败！");
        }
    }
}

void ReaderDialog::on_editButton_clicked()
{
    QModelIndex currentIndex = ui->tableViewReaders->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要编辑的读者！");
        return;
    }

    int row = currentIndex.row();
    QVariantMap readerData = m_readerModel->getReaderData(row);

    if (readerData.isEmpty()) {
        QMessageBox::warning(this, "错误", "无法获取读者数据！");
        return;
    }

    // 创建编辑对话框
    AddReaderDialog dialog(this);
    dialog.setWindowTitle("编辑读者信息");

    // 设置现有数据到对话框
    dialog.setReaderData(readerData);

    if (dialog.exec() == QDialog::Accepted) {
        QVariantMap updatedData = dialog.getReaderData();

        // 确保ID和读者编号不变
        updatedData["id"] = readerData["id"];
        updatedData["reader_id"] = readerData["reader_id"];  // 读者编号不可修改

        if (m_readerModel->updateReader(row, updatedData)) {
            QMessageBox::information(this, "成功", "读者信息更新成功！");
            updateStatus();
        } else {
            QMessageBox::warning(this, "错误", "更新读者信息失败！");
        }
    }
}

void ReaderDialog::on_deleteButton_clicked()
{
    QModelIndex currentIndex = ui->tableViewReaders->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "警告", "请先选择要删除的读者！");
        return;
    }

    int row = currentIndex.row();
    QVariantMap readerData = m_readerModel->getReaderData(row);
    QString readerName = readerData["name"].toString();
    QString readerId = readerData["reader_id"].toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  QString("确定要删除读者【%1】（%2）吗？")
                                      .arg(readerName).arg(readerId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_readerModel->removeReader(row)) {
            QMessageBox::information(this, "成功", "读者删除成功！");
            updateStatus();
        } else {
            QMessageBox::warning(this, "错误", "读者删除失败！");
        }
    }
}

void ReaderDialog::on_refreshButton_clicked()
{
    m_readerModel->refreshData();
    QMessageBox::information(this, "提示", "读者数据已刷新！", QMessageBox::Ok);
    updateStatus();
}

void ReaderDialog::on_closeButton_clicked()
{
    close();
}

void ReaderDialog::on_searchButton_clicked()
{
    QString keyword = ui->searchLineEdit->text().trimmed();
    if (!keyword.isEmpty()) {
        m_readerModel->searchReaders(keyword);
    }
    updateStatus();
}

void ReaderDialog::on_resetButton_clicked()
{
    ui->searchLineEdit->clear();
    m_readerModel->searchReaders("");
    updateStatus();
}

void ReaderDialog::updateStatus()
{
    int totalReaders = m_readerModel->getTotalReaders();
    setWindowTitle(QString("读者管理 - 共 %1 位读者").arg(totalReaders));
}
