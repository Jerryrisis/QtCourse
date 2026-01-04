#include "borrowbookdialog.h"
#include "ui_borrowbookdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QDebug>

BorrowBookDialog::BorrowBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrowBookDialog)
{
    ui->setupUi(this);
    setWindowTitle("借阅新书");

    // 设置默认应还日期（30天后）
    ui->dueDateEdit->setDate(QDate::currentDate().addDays(30));
    ui->dueDateEdit->setMinimumDate(QDate::currentDate().addDays(1));

    // 加载数据
    loadReaders();
    loadAvailableBooks();

    // 连接信号槽
    connect(ui->readerComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BorrowBookDialog::onReaderSelectionChanged);
    connect(ui->bookComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BorrowBookDialog::onBookSelectionChanged);
    connect(ui->refreshReaderButton, &QPushButton::clicked,
            this, &BorrowBookDialog::onRefreshReadersClicked);
    connect(ui->okButton, &QPushButton::clicked,
            this, &BorrowBookDialog::onBorrowButtonClicked);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &QDialog::reject);

    // 初始验证
    validateForm();


    this->setStyleSheet(R"(
    /* ===== 对话框基础样式 ===== */
    QDialog {
        background-color: #f8fafc;
        font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
        font-size: 13px;
    }

    /* ===== 所有标签样式 ===== */
    QLabel {
        color: #2c3e50;
        font-weight: 600;
        font-size: 13px;
        padding: 6px 0px;
        min-width: 70px;
    }

    /* 特定标签对象 */
    QLabel#label_2,    /* 选择读者: */
    QLabel#label_7,    /* 选择图书: */
    QLabel#label_14,   /* 借阅期限: */
    QLabel#label_3,    /* 读者姓名: */
    QLabel#label_5,    /* 电话: */
    QLabel#label_8,    /* 书名: */
    QLabel#label_9,    /* ISBN: */
    QLabel#label_12 {  /* 可借数量: */
        color: #2c3e50;
        font-weight: 600;
        font-size: 13px;
        padding: 6px 0px;
        min-width: 70px;
    }

    /* 信息显示标签 (读者和图书信息) */
    QLabel#readerNameLabel,
    QLabel#readerPhoneLabel,
    QLabel#bookTitleLabel,
    QLabel#bookIsbnLabel,
    QLabel#bookAvailableLabel {
        color: #5f6368;
        font-weight: 500;
        font-size: 13px;
        padding: 6px 0px;
        border-bottom: 1px dotted #d1d9e0;
        min-height: 20px;
    }

    /* ===== 输入控件样式 ===== */
    /* 读者下拉框 */
    QComboBox#readerComboBox {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        min-width: 250px;
        padding-right: 30px;
    }

    QComboBox#readerComboBox:focus {
        border-color: #1a73e8;
        background-color: white;
    }

    /* 图书下拉框 */
    QComboBox#bookComboBox {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        min-width: 250px;
        padding-right: 30px;
    }

    QComboBox#bookComboBox:focus {
        border-color: #1a73e8;
        background-color: white;
    }

    /* 下拉框通用样式 */
    QComboBox::drop-down {
        border: none;
        width: 30px;
        background-color: #f8fafc;
        border-left: 1px solid #d1d9e0;
        border-radius: 0px 3px 3px 0px;
    }

    QComboBox::down-arrow {
        width: 12px;
        height: 12px;
    }

    QComboBox QAbstractItemView {
        border: 1px solid #e0e0e0;
        border-radius: 4px;
        background-color: white;
        selection-background-color: #e8f0fe;
        selection-color: #1a73e8;
        outline: none;
        padding: 4px 0px;
    }

    /* 日期选择器 */
    QDateEdit#dueDateEdit {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        min-width: 120px;
    }

    QDateEdit#dueDateEdit:focus {
        border-color: #1a73e8;
        background-color: white;
    }

    QDateEdit#dueDateEdit::drop-down {
        width: 30px;
        border-left: 1px solid #d1d9e0;
        background-color: #f8fafc;
        border-radius: 0px 3px 3px 0px;
    }

    /* ===== 按钮样式 ===== */
    /* 刷新按钮 - 灰色主题 */
    QPushButton#refreshReaderButton {
        background-color: #f1f3f4;
        color: #5f6368;
        border: 1px solid #dadce0;
        border-radius: 4px;
        padding: 8px 12px;
        font-weight: 600;
        font-size: 13px;
        min-width: 60px;
        min-height: 36px;
    }

    QPushButton#refreshReaderButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#refreshReaderButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* 取消按钮 - 灰色主题 */
    QPushButton#cancelButton {
        background-color: #f1f3f4;
        color: #5f6368;
        border: 1px solid #dadce0;
        border-radius: 4px;
        padding: 10px 24px;
        font-weight: 600;
        font-size: 13px;
        min-width: 85px;
        min-height: 36px;
    }

    QPushButton#cancelButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#cancelButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* 确认借书按钮 - 蓝色主题 (主要操作) */
    QPushButton#okButton {
        background-color: #1a73e8;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 10px 24px;
        font-weight: 600;
        font-size: 13px;
        min-width: 100px;
        min-height: 36px;
    }

    QPushButton#okButton:hover {
        background-color: #0d62d9;
    }

    QPushButton#okButton:pressed {
        background-color: #0b57d0;
    }

    /* 按钮禁用状态 */
    QPushButton:disabled {
        background-color: #f1f3f4;
        color: #9aa0a6;
        border: 1px solid #f1f3f4;
    }

    /* 确认按钮特殊状态 - 当表单不完整时 */
    QPushButton#okButton:disabled {
        background-color: #c2e7ff;
        color: white;
    }

    /* ===== 布局美化 ===== */
    /* 水平间隔器 */
    QSpacerItem#horizontalSpacer {
        /* 自动填充空间 */
    }

    /* 读者选择行 */
    QHBoxLayout#horizontalLayout_8 {
        spacing: 8px;
        margin-bottom: 8px;
    }

    /* 读者信息行 (姓名+电话) */
    QHBoxLayout#horizontalLayout_7 {
        spacing: 20px;
        margin-bottom: 16px;
    }

    /* 图书选择行 */
    QHBoxLayout#horizontalLayout_6 {
        spacing: 8px;
        margin-bottom: 8px;
    }

    /* 图书信息行 (书名+ISBN+可借数量) */
    QHBoxLayout#horizontalLayout_4,
    QHBoxLayout#horizontalLayout_5 {
        spacing: 20px;
        margin-bottom: 8px;
    }

    /* 借阅期限行 */
    QHBoxLayout#horizontalLayout_3 {
        spacing: 8px;
        margin-bottom: 20px;
    }

    /* 按钮行 */
    QHBoxLayout#horizontalLayout_2 {
        spacing: 12px;
        margin-top: 20px;
    }

    /* 主水平布局 */
    QHBoxLayout#horizontalLayout {
        spacing: 0px;
        margin: 20px;
    }

    /* ===== 分组视觉区分 ===== */
    /* 读者信息组 */
    QHBoxLayout#horizontalLayout_8 + QHBoxLayout#horizontalLayout_7 {
        background-color: #f8f9fa;
        border-radius: 4px;
        padding: 8px 12px;
        margin-bottom: 20px;
    }

    /* 图书信息组 */
    QHBoxLayout#horizontalLayout_6 + QHBoxLayout#horizontalLayout_4 + QHBoxLayout#horizontalLayout_5 {
        background-color: #f8f9fa;
        border-radius: 4px;
        padding: 8px 12px;
        margin-bottom: 20px;
    }
)");

}

BorrowBookDialog::~BorrowBookDialog()
{
    delete ui;
}

void BorrowBookDialog::loadReaders()
{
    ui->readerComboBox->clear();
    m_readers.clear();

    QVector<QVariantMap> readers = DatabaseManager::instance().getAllReaders();
    for (const auto &reader : readers) {
        int id = reader["id"].toInt();
        QString displayText = QString("%1 (%2)").arg(reader["name"].toString())
                                  .arg(reader["reader_id"].toString());

        ui->readerComboBox->addItem(displayText, id);
        m_readers[id] = reader;
    }

    if (ui->readerComboBox->count() > 0) {
        ui->readerComboBox->setCurrentIndex(0);
    }
}

void BorrowBookDialog::loadAvailableBooks()
{
    ui->bookComboBox->clear();
    m_books.clear();

    QVector<QVariantMap> books = DatabaseManager::instance().getAvailableBooks();
    for (const auto &book : books) {
        int id = book["id"].toInt();
        QString displayText = QString("%1 - %2").arg(book["title"].toString())
                                  .arg(book["author"].toString());

        ui->bookComboBox->addItem(displayText, id);
        m_books[id] = book;
    }

    if (ui->bookComboBox->count() > 0) {
        ui->bookComboBox->setCurrentIndex(0);
    }
}

void BorrowBookDialog::onReaderSelectionChanged(int index)
{
    if (index >= 0) {
        int readerId = ui->readerComboBox->itemData(index).toInt();
        updateReaderInfo(readerId);
    }
    validateForm();
}

void BorrowBookDialog::onBookSelectionChanged(int index)
{
    if (index >= 0) {
        int bookId = ui->bookComboBox->itemData(index).toInt();
        updateBookInfo(bookId);
    }
    validateForm();
}

void BorrowBookDialog::updateReaderInfo(int readerId)
{
    if (m_readers.contains(readerId)) {
        const QVariantMap &reader = m_readers[readerId];
        ui->readerNameLabel->setText(reader["name"].toString());
        ui->readerPhoneLabel->setText(reader["phone"].toString());
    }
}

void BorrowBookDialog::updateBookInfo(int bookId)
{
    if (m_books.contains(bookId)) {
        const QVariantMap &book = m_books[bookId];
        ui->bookTitleLabel->setText(book["title"].toString());
        ui->bookIsbnLabel->setText(book["isbn"].toString());
        ui->bookAvailableLabel->setText(QString("可借数量: %1").arg(book["available_count"].toString()));
    }
}

void BorrowBookDialog::onRefreshReadersClicked()
{
    loadReaders();
}

void BorrowBookDialog::onBorrowButtonClicked()
{
    // 获取选中的读者和图书
    int readerId = getSelectedReaderId();
    int bookId = getSelectedBookId();
    QDate dueDate = getDueDate();

    if (readerId == -1 || bookId == -1) {
        QMessageBox::warning(this, "错误", "请选择读者和图书！");
        return;
    }

    // 执行借阅操作
    if (DatabaseManager::instance().borrowBook(bookId, readerId, dueDate)) {
        QMessageBox::information(this, "成功", "借书成功！");
        accept();  // 关闭对话框
    } else {
        QMessageBox::warning(this, "错误", "借书失败，请检查图书是否可借！");
    }
}

void BorrowBookDialog::validateForm()
{
    bool isValid = (ui->readerComboBox->currentIndex() >= 0) &&
                   (ui->bookComboBox->currentIndex() >= 0);
    ui->okButton->setEnabled(isValid);
}

// 获取选择的数据
int BorrowBookDialog::getSelectedReaderId() const
{
    if (ui->readerComboBox->currentIndex() >= 0) {
        return ui->readerComboBox->currentData().toInt();
    }
    return -1;
}

int BorrowBookDialog::getSelectedBookId() const
{
    if (ui->bookComboBox->currentIndex() >= 0) {
        return ui->bookComboBox->currentData().toInt();
    }
    return -1;
}

QDate BorrowBookDialog::getDueDate() const
{
    return ui->dueDateEdit->date();
}
