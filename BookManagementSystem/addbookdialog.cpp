#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include <QDate>
#include <QDebug>

AddBookDialog::AddBookDialog(QWidget *parent, bool isEditMode)
    : QDialog(parent)
    , ui(new Ui::AddBookDialog)
    , m_isEditMode(isEditMode)
{
    ui->setupUi(this);

    // 根据模式设置窗口标题
    if (m_isEditMode) {
        setWindowTitle("编辑图书信息");
    } else {
        setWindowTitle("添加新图书");
    }

    // 设置日期控件为当前日期
    ui->dateEdit->setDate(QDate::currentDate());

    // 设置总数量默认值
    ui->countSpinBox->setValue(1);
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

void AddBookDialog::on_okButton_clicked()
{
    accept();
}

void AddBookDialog::on_cancelButton_clicked()
{
    reject();
}

// 获取对话框中的数据
QVariantMap AddBookDialog::getBookData() const
{
    QVariantMap bookData;

    bookData["isbn"] = ui->isbnLineEdit->text().trimmed();
    bookData["title"] = ui->titleLineEdit->text().trimmed();
    bookData["author"] = ui->authorLineEdit->text().trimmed();
    bookData["publisher"] = ui->publisherLineEdit->text().trimmed();
    bookData["publish_date"] = ui->dateEdit->date().toString("yyyy-MM-dd");
    bookData["category"] = ui->categoryComboBox->currentText();
    bookData["total_count"] = ui->countSpinBox->value();
    bookData["available_count"] = ui->countSpinBox->value();
    bookData["price"] = ui->priceDoubleSpinBox->value();
    //bookData["description"] = ui->descriptionTextEdit->toPlainText().trimmed();

    return bookData;
}

// 设置对话框数据（用于编辑时预填）
void AddBookDialog::setBookData(const QVariantMap &bookData)
{
    ui->isbnLineEdit->setText(bookData.value("isbn").toString());
    ui->titleLineEdit->setText(bookData.value("title").toString());
    ui->authorLineEdit->setText(bookData.value("author").toString());
    ui->publisherLineEdit->setText(bookData.value("publisher").toString());

    // 设置日期
    QDate date = QDate::fromString(bookData.value("publish_date").toString(), "yyyy-MM-dd");
    if (date.isValid()) {
        ui->dateEdit->setDate(date);
    }

    // 设置分类
    QString category = bookData.value("category").toString();
    int index = ui->categoryComboBox->findText(category);
    if (index != -1) {
        ui->categoryComboBox->setCurrentIndex(index);
    } else {
        ui->categoryComboBox->setCurrentText(category);
    }

    ui->countSpinBox->setValue(bookData.value("total_count").toInt());
    ui->priceDoubleSpinBox->setValue(bookData.value("price").toDouble());
    //ui->descriptionTextEdit->setPlainText(bookData.value("description").toString());
}

// 设置ISBN是否可编辑
void AddBookDialog::setISBNEditable(bool editable)
{
    ui->isbnLineEdit->setReadOnly(!editable);
}
