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
        padding: 8px 0px;
        min-width: 60px;
    }

    /* 特定标签对象 */
    QLabel#label,      /* 读者编号 */
    QLabel#label_2,    /* 姓名 */
    QLabel#label_3,    /* 性别 */
    QLabel#label_5,    /* 电话 */
    QLabel#label_4,    /* 邮箱 */
    QLabel#label_6 {   /* 地址 */
        color: #2c3e50;
        font-weight: 600;
        font-size: 13px;
        padding: 8px 0px;
        min-width: 60px;
    }

    /* ===== 输入框样式 ===== */
    /* 所有单行文本框 */
    QLineEdit {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        selection-background-color: #1a73e8;
    }

    QLineEdit:focus {
        border-color: #1a73e8;
        background-color: white;
        outline: none;
    }

    /* 特定输入框 */
    QLineEdit#readerIdLineEdit,
    QLineEdit#nameLineEdit,
    QLineEdit#phoneLineEdit,
    QLineEdit#emailLineEdit,
    QLineEdit#addressLineEdit {
        min-width: 200px; /* 设置最小宽度 */
    }

    /* ===== 性别下拉框样式 ===== */
    QComboBox#genderComboBox {
        border: 1.5px solid #d1d9e0;
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        padding-right: 30px; /* 为下拉箭头留空间 */
    }

    QComboBox#genderComboBox:focus {
        border-color: #1a73e8;
        background-color: white;
    }

    QComboBox#genderComboBox::drop-down {
        border: none;
        width: 30px;
        background-color: #f8fafc;
        border-left: 1px solid #d1d9e0;
        border-radius: 0px 3px 3px 0px;
    }

    QComboBox#genderComboBox::down-arrow {
        width: 12px;
        height: 12px;
    }

    QComboBox#genderComboBox QAbstractItemView {
        border: 1px solid #e0e0e0;
        border-radius: 4px;
        background-color: white;
        selection-background-color: #e8f0fe;
        selection-color: #1a73e8;
        outline: none;
        padding: 4px 0px;
    }

    QComboBox#genderComboBox QAbstractItemView::item {
        padding: 8px 12px;
        border-radius: 2px;
    }

    QComboBox#genderComboBox QAbstractItemView::item:selected {
        background-color: #e8f0fe;
        color: #1a73e8;
    }

    /* ===== 按钮区域样式 ===== */
    /* 水平布局容器 */
    QHBoxLayout#horizontalLayout_9 {
        spacing: 12px; /* 按钮间距 */
        margin-top: 20px; /* 顶部间距 */
    }

    /* 水平间隔器 */
    QSpacerItem#horizontalSpacer,
    QSpacerItem#horizontalSpacer_2 {
        /* 自动填充空间 */
    }

    /* 垂直间隔器 */
    QSpacerItem#verticalSpacer,
    QSpacerItem#verticalSpacer_2 {
        /* 自动填充空间 */
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

    /* 确认按钮 - 蓝色主题 (与添加图书一致) */
    QPushButton#okButton {
        background-color: #1a73e8;
        color: white;
        border: none;
        border-radius: 4px;
        padding: 10px 24px;
        font-weight: 600;
        font-size: 13px;
        min-width: 85px;
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

    /* ===== 布局美化 ===== */
    /* 主垂直布局 */
    QVBoxLayout#verticalLayout {
        spacing: 16px; /* 行间距 */
        margin: 20px; /* 内边距 */
    }

    /* 每个水平布局行 */
    QHBoxLayout#horizontalLayout,    /* 读者编号行 */
    QHBoxLayout#horizontalLayout_2,  /* 姓名行 */
    QHBoxLayout#horizontalLayout_3,  /* 性别行 */
    QHBoxLayout#horizontalLayout_4,  /* 电话行 */
    QHBoxLayout#horizontalLayout_5,  /* 邮箱行 */
    QHBoxLayout#horizontalLayout_6 { /* 地址行 */
        spacing: 12px; /* 标签和输入框间距 */
        margin: 0px;
    }

    /* 表单容器布局 */
    QGridLayout#gridLayout {
        spacing: 8px; /* 单元格间距 */
    }

    /* ===== 必填字段指示器 (可选) ===== */
    /* 如果需要标记必填字段，可以取消注释下面的代码 */
    /*
    QLabel[required="true"] {
        color: #c5221f;
    }

    QLabel[required="true"]::after {
        content: " *";
        color: #c5221f;
    }
    */
)");

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
