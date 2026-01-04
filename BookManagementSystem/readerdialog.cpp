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

    this->setStyleSheet(R"(
    /* ===== 对话框基础样式 ===== */
    QDialog {
        background-color: #f8fafc;
        font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
        font-size: 13px;
    }

    /* ===== 状态标签样式 ===== */
    QLabel#searchStatusLabel {
        color: #5f6368;
        font-size: 12px;
        padding: 6px 0px;
        min-height: 20px;
    }

    /* ===== 搜索区域样式 ===== */
    /* 搜索输入框 */
    QLineEdit#searchLineEdit {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        selection-background-color: #1a73e8;
    }

    QLineEdit#searchLineEdit:focus {
        border-color: #1a73e8;
        background-color: white;
        outline: none;
    }

    /* 搜索按钮 - 蓝色主题 (主要操作) */
    QPushButton#searchButton {
        background-color: #1a73e8;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 8px 16px;
        font-weight: 600;
        font-size: 13px;
        min-width: 70px;
        min-height: 36px;
    }

    QPushButton#searchButton:hover {
        background-color: #0d62d9;
    }

    QPushButton#searchButton:pressed {
        background-color: #0b57d0;
    }

    /* 重置按钮 - 灰色主题 (次要操作) */
    QPushButton#resetButton {
        background-color: #f1f3f4;
        color: #5f6368;
        border: 1px solid #dadce0;
        border-radius: 4px;
        padding: 8px 16px;
        font-weight: 600;
        font-size: 13px;
        min-width: 70px;
        min-height: 36px;
    }

    QPushButton#resetButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#resetButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* ===== 读者表格样式 ===== */
    QTableView#tableViewReaders {
        background-color: white;
        border: 1px solid #e0e0e0;
        border-radius: 4px;
        alternate-background-color: #f8f9fa;
        selection-background-color: #e8f0fe;
        selection-color: #1a73e8;
        gridline-color: #f1f3f4;
        font-size: 13px;
    }

    /* 表格表头 */
    QHeaderView::section {
        background-color: #f8f9fa;
        color: #5f6368;
        padding: 10px 8px;
        border: none;
        border-bottom: 2px solid #e0e0e0;
        font-weight: 600;
        font-size: 13px;
    }

    QHeaderView::section:checked {
        background-color: #e8f0fe;
    }

    /* 隐藏垂直行号 */
    QHeaderView#verticalHeader {
        width: 0px;
        max-width: 0px;
        min-width: 0px;
    }

    /* 表格项 */
    QTableView::item {
        padding: 8px 8px;
        border-bottom: 1px solid #f8f9fa;
    }

    QTableView::item:selected {
        background-color: #e8f0fe;
        color: #1a73e8;
    }

    /* ===== 操作按钮区域样式 ===== */
    /* 添加按钮 - 蓝色主题 (主要操作) */
    QPushButton#addButton {
        background-color: #1a73e8;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 8px 20px;
        font-weight: 600;
        font-size: 13px;
        min-width: 80px;
        min-height: 36px;
    }

    QPushButton#addButton:hover {
        background-color: #0d62d9;
    }

    QPushButton#addButton:pressed {
        background-color: #0b57d0;
    }

    /* 编辑按钮 - 灰色主题 (中性操作) */
    QPushButton#editButton {
        background-color: #f1f3f4;
        color: #5f6368;
        border: 1px solid #dadce0;
        border-radius: 4px;
        padding: 8px 20px;
        font-weight: 600;
        font-size: 13px;
        min-width: 80px;
        min-height: 36px;
    }

    QPushButton#editButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#editButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* 删除按钮 - 红色主题 (危险操作) */
    QPushButton#deleteButton {
        background-color: #fce8e6;
        color: #c5221f;
        border: 1px solid #f6aea9;
        border-radius: 4px;
        padding: 8px 20px;
        font-weight: 600;
        font-size: 13px;
        min-width: 80px;
        min-height: 36px;
    }

    QPushButton#deleteButton:hover {
        background-color: #f6aea9;
        border-color: #e37470;
    }

    QPushButton#deleteButton:pressed {
        background-color: #e37470;
        color: white;
    }

    /* 刷新按钮 - 灰色主题 (中性操作) */
    QPushButton#refreshButton {
        background-color: #f1f3f4;
        color: #5f6368;
        border: 1px solid #dadce0;
        border-radius: 4px;
        padding: 8px 20px;
        font-weight: 600;
        font-size: 13px;
        min-width: 80px;
        min-height: 36px;
    }

    QPushButton#refreshButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#refreshButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* 关闭按钮 - 灰色主题 (退出操作) */
    QPushButton#closeButton {
        background-color: #5f6368;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 8px 20px;
        font-weight: 600;
        font-size: 13px;
        min-width: 80px;
        min-height: 36px;
    }

    QPushButton#closeButton:hover {
        background-color: #3c4043;
    }

    QPushButton#closeButton:pressed {
        background-color: #202124;
    }

    /* 按钮禁用状态 */
    QPushButton:disabled {
        background-color: #f1f3f4;
        color: #9aa0a6;
        border: 1px solid #f1f3f4;
    }
)");
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
