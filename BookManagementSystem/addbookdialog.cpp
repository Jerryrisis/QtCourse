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


    this->setStyleSheet(R"(
    /* ===== 对话框基础样式 ===== */
    QDialog {
        background-color: #f8fafc;
        font-family: "Segoe UI", "Microsoft YaHei", sans-serif;
        font-size: 13px;
    }

    /* ===== 标签样式 ===== */
    QLabel {
        color: #2c3e50;         /* 与主窗口菜单栏颜色一致 */
        font-weight: 600;       /* 中等加粗，更现代 */
        font-size: 13px;
        padding: 4px 0px;
    }

    /* ===== 输入框样式 ===== */
    QLineEdit, QDateEdit, QComboBox, QSpinBox, QDoubleSpinBox {
        border: 1.5px solid #d1d9e0;     /* 更细的边框，更现代 */
        border-radius: 4px;
        padding: 8px 12px;
        background-color: white;
        font-size: 13px;
        min-height: 20px;
        selection-background-color: #3498db; /* 选中文本背景色 */
    }

    QLineEdit:focus, QDateEdit:focus, QComboBox:focus,
    QSpinBox:focus, QDoubleSpinBox:focus {
        border-color: #3498db;           /* 焦点时蓝色边框 */
        background-color: white;
        outline: none;                  /* 移除默认焦点框 */
    }

    /* ===== 下拉框特定样式 ===== */
    QComboBox {
        padding-right: 30px;            /* 为下拉箭头留空间 */
    }

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
        image: url(:/icons/arrow-down.png); /* 可自定义箭头图标 */
    }

    /* ===== 数值框特定样式 ===== */
    QSpinBox::up-button, QSpinBox::down-button,
    QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
        width: 20px;
        border: 1px solid #d1d9e0;
        background-color: #f8fafc;
        border-radius: 2px;
    }

    QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
        image: url(:/icons/arrow-up.png);
        width: 10px;
        height: 10px;
    }

    QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
        image: url(:/icons/arrow-down.png);
        width: 10px;
        height: 10px;
    }

    /* ===== 日期选择器特定样式 ===== */
    QDateEdit::drop-down {
        width: 30px;
    }

    /* ===== 按钮样式 ===== */
    QPushButton {
        border-radius: 4px;
        padding: 10px 24px;
        font-weight: 600;
        font-size: 13px;
        min-width: 85px;
        min-height: 36px;
        border: none;
        transition: all 0.2s;           /* 平滑过渡效果 */
    }

    /* 取消按钮 - 灰色主题 */
    QPushButton#cancelButton {
        background-color: #f1f3f4;       /* 浅灰色背景 */
        color: #5f6368;                  /* 深灰色文字 */
        border: 1px solid #dadce0;
    }

    QPushButton#cancelButton:hover {
        background-color: #e8eaed;
        border-color: #d2e3fc;
    }

    QPushButton#cancelButton:pressed {
        background-color: #d2e3fc;
        color: #1a73e8;
    }

    /* 确认按钮 - 蓝色主题（与主窗口搜索按钮一致） */
    QPushButton#okButton {
        background-color: #1a73e8;       /* 现代蓝色 */
        color: white;
    }

    QPushButton#okButton:hover {
        background-color: #0d62d9;
        box-shadow: 0 2px 4px rgba(26, 115, 232, 0.3);
    }

    QPushButton#okButton:pressed {
        background-color: #0b57d0;
        box-shadow: none;
    }

    /* 按钮禁用状态 */
    QPushButton:disabled {
        background-color: #f1f3f4;
        color: #9aa0a6;
        border: 1px solid #f1f3f4;
    }

    /* ===== 容器布局样式 ===== */
    QVBoxLayout, QHBoxLayout, QGridLayout {
        /* 确保布局不添加额外样式 */
    }

    /* ===== 标题样式 ===== */
    QLabel[title="true"] {
        font-size: 18px;
        font-weight: 700;
        color: #202124;
        padding-bottom: 12px;
    }

    /* ===== 必填字段指示器 ===== */
    QLabel[required="true"]::after {
        content: "*";
        color: #d93025;
        margin-left: 2px;
    }
)");
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
