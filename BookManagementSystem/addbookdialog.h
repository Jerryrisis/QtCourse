#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class AddBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数：mode参数决定是添加模式还是编辑模式
    explicit AddBookDialog(QWidget *parent = nullptr, bool isEditMode = false);
    ~AddBookDialog();

    // 获取用户输入的数据
    QVariantMap getBookData() const;

    // 设置对话框数据（用于编辑时预填）
    void setBookData(const QVariantMap &bookData);

    // 设置ISBN是否可编辑（编辑模式下ISBN通常不可修改）
    void setISBNEditable(bool editable);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddBookDialog *ui;
    bool m_isEditMode;  // 标记是编辑模式还是添加模式
};

#endif // ADDBOOKDIALOG_H
