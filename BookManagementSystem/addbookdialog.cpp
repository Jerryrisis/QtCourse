// addbookdialog.cpp
#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include <QDebug>

AddBookDialog::AddBookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBookDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加新图书");
    // 可以在这里初始化一些控件，例如设置出版日期为今天
    ui->dateEdit->setDate(QDate::currentDate());
}

AddBookDialog::~AddBookDialog()
{
    delete ui;
}

QVariantMap AddBookDialog::getBookData() const
{
    QVariantMap bookData;
    bookData["isbn"] = ui->isbnLineEdit->text().trimmed();
    bookData["title"] = ui->titleLineEdit->text().trimmed();
    bookData["author"] = ui->authorLineEdit->text().trimmed();
    bookData["publisher"] = ui->publisherLineEdit->text().trimmed();
    bookData["publish_date"] = ui->dateEdit->date().toString("yyyy-MM-dd"); // 格式化为字符串
    bookData["category"] = ui->categoryComboBox->currentText();
    bookData["total_count"] = ui->countSpinBox->value();
    bookData["available_count"] = ui->countSpinBox->value(); // 新书可借数量等于总数
    bookData["price"] = ui->priceDoubleSpinBox->value();
    return bookData;
}

void AddBookDialog::on_okButton_clicked()
{
    accept();
}

