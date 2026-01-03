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
