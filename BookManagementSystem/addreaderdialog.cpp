#include "addreaderdialog.h"
#include "ui_addreaderdialog.h"
#include <QDate>
#include <QDebug>
#include <QMessageBox>

AddReaderDialog::AddReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddReaderDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加读者");


    // 连接确定按钮
    connect(ui->okButton, &QPushButton::clicked, this, [this]() {
        // 验证必填字段
        if (ui->readerIdLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "警告", "读者编号不能为空！");
            return;
        }
        if (ui->nameLineEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "警告", "读者姓名不能为空！");
            return;
        }
        accept();
    });

    // 连接取消按钮
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddReaderDialog::~AddReaderDialog()
{
    delete ui;
}


void AddReaderDialog::on_okButton_clicked()
{
    // 添加简单验证
    if (ui->readerIdLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "读者编号不能为空！");
        return;
    }
    if (ui->nameLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "读者姓名不能为空！");
        return;
    }

    accept();  // 关闭对话框并返回 Accepted
}

void AddReaderDialog::on_cancelButton_clicked()
{
    reject();  // 关闭对话框并返回 Rejected
}

QVariantMap AddReaderDialog::getReaderData() const
{
    QVariantMap readerData;

    readerData["reader_id"] = ui->readerIdLineEdit->text().trimmed();
    readerData["name"] = ui->nameLineEdit->text().trimmed();
    readerData["gender"] = ui->genderComboBox->currentText();
    readerData["phone"] = ui->phoneLineEdit->text().trimmed();
    readerData["email"] = ui->emailLineEdit->text().trimmed();
    readerData["address"] = ui->addressLineEdit->text().trimmed();
    readerData["register_date"] = QDate::currentDate().toString("yyyy-MM-dd");

    return readerData;
}


void AddReaderDialog::setReaderData(const QVariantMap &readerData)
{
    ui->readerIdLineEdit->setText(readerData.value("reader_id").toString());
    ui->nameLineEdit->setText(readerData.value("name").toString());

    // 设置性别
    QString gender = readerData.value("gender").toString();
    int index = ui->genderComboBox->findText(gender);
    if (index != -1) {
        ui->genderComboBox->setCurrentIndex(index);
    }

    ui->phoneLineEdit->setText(readerData.value("phone").toString());
    ui->emailLineEdit->setText(readerData.value("email").toString());
    ui->addressLineEdit->setText(readerData.value("address").toString());

    // 编辑模式下，读者编号不可修改
    ui->readerIdLineEdit->setReadOnly(true);
}
