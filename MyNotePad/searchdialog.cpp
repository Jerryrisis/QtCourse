#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QMessageBox>

searchDialog::searchDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::searchDialog)
{
    ui->setupUi(this);

    pTextEdit = textEdit;
    ui->rbDown->setCheckable(true);
}

searchDialog::~searchDialog()
{
    delete ui;
}

void searchDialog::on_btnFindNext_clicked()
{
    QString target=ui->searchtext->text();

    if(target == "" || pTextEdit ==nullptr){
        return;
    }
    QString text= pTextEdit->toPlainText();

    QTextCursor c = pTextEdit->textCursor();
    int index=-1;

    if(ui->rbDown->isChecked()){
        index=text.indexOf(target,c.position(),ui->cbClass->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >=0){
            c.setPosition(index);
            c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->rbUp->isChecked()){
        index = text.lastIndexOf(target, c.position() - text.length() - 1,ui->cbClass->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);
        if(index >=0){
            c.setPosition(index + target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }

    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("记事本");
        msg.setText(QString("找不到") + target);
        msg.setWindowFlag(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}


void searchDialog::on_btnCancel_clicked()
{
    accept();
}

