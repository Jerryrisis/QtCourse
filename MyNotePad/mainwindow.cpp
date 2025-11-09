#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textChanged = false;
    on_actionNew_triggered();
    statusLabel.setMaximumWidth(180);
    statusLabel.setText("length: " + QString::number(0) + " lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(180);
    statusCursorLabel.setText("Ln: " + QString::number(0) + " Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("林俊韬"));
    ui->statusbar->addPermanentWidget(author);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    //如果此处将actionPaste默认状态设置为false，该文本编辑器将不可在最开始粘贴来自外部复制的文本，因此此处设置为true
    ui->actionPaste->setEnabled(true);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_WordWrap->setChecked(false);
    }
    else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_WordWrap->setChecked(true);
    }

    ui->actionStatusBar->setChecked(true);
    ui->actionToolBar->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}


void MainWindow::on_actionSearch_triggered()
{
    searchDialog dlg(this, ui->TextEdit);
    dlg.exec();
}


void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this, ui->TextEdit);
    dlg.exec();
}


void MainWindow::on_actionNew_triggered()
{
    if(!userEditConfirmed())return;
    FilePath = "";
    ui->TextEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
    textChanged = false;
}


void MainWindow::on_actionOpen_triggered()
{
    if(!userEditConfirmed())return;
    ui->TextEdit->clear();
    QString filename = QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text Files (*.txt) ;; All (*.*)"));
    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        //QMessageBox::warning(this, ".." , "打开文件失败");
        ui->statusbar->showMessage("打开文件失败！" , 2000);
        return;
    }

    FilePath = filename;

    QTextStream in(&file);
    QString text = in.readAll();
    ui->TextEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    textChanged = false;
}


void MainWindow::on_actionSave_triggered()
{
    if(FilePath == ""){
        QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text Files (*.txt)"));

        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text))
        {
            //QMessageBox::warning(this, "提示", "保存文件失败");
            ui->statusbar->showMessage("文件保存失败！" , 2000);
            return;
        }
        file.close();
        FilePath = filename;
    }

    QFile file(FilePath);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        //QMessageBox::warning(this, "提示", "保存文件失败");
        ui->statusbar->showMessage("文件保存失败！" , 2000);
        return;
    }
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();
    ui->statusbar->showMessage("文件保存成功！" , 2000);
    this->setWindowTitle(QFileInfo(FilePath).absoluteFilePath());
    textChanged = false;
}


bool MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text Files (*.txt)"));

    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        //QMessageBox::warning(this, ".." , "保存文件失败");
        ui->statusbar->showMessage("文件保存失败！" , 2000);
        return false;
    }
    FilePath = filename;
    QTextStream out(&file);
    QString text = ui->TextEdit->toPlainText();
    out<<text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(FilePath).absoluteFilePath());
    ui->statusbar->showMessage("文件保存成功！" , 2000);
    return true;
}


void MainWindow::on_TextEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*" + this->windowTitle());
        textChanged = true;
    }

    statusLabel.setText("length: " + QString::number(ui->TextEdit->toPlainText().length()) + " lines: " + QString::number(ui->TextEdit->document()->lineCount()));

}

bool MainWindow::userEditConfirmed()
{
    if(textChanged){
        //QString path = (FilePath != "") ? FilePath : "无标题.txt";
        QString path = FilePath;
        if(path!=""){
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Question);
            msg.setWindowTitle("...");
            msg.setWindowFlag(Qt::Drawer);
            msg.setText(QString("是否将更改保存到") + "\"" + path + "\" ?");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            int r = msg.exec();
            switch(r){
            case QMessageBox::Yes:
                on_actionSave_triggered();
                break;
            case QMessageBox::No:
                textChanged = false;
                break;
            case QMessageBox::Cancel:
                return false;
            }
        }
        else{
            QMessageBox msg(this);
            msg.setIcon(QMessageBox::Question);
            msg.setWindowTitle("...");
            msg.setWindowFlag(Qt::Drawer);
            msg.setText(QString("是否将更改另存为"));
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            int r = msg.exec();
            switch(r){
            case QMessageBox::Yes:
                //小优化（将on_actionSaveAs_triggered()槽函数返回类型改为bool，若另存成功返回true，否则在当前函数userEditConfirmed()返回false
                //避免打开另存为窗口未保存直接关闭窗口，仍然继续执行新建窗口等操作清空了原未保存的文本导致丢失
                if(!on_actionSaveAs_triggered())return false;
                break;
            case QMessageBox::No:
                textChanged = false;
                break;
            case QMessageBox::Cancel:
                return false;
            }
        }
    }
    return true;
}


void MainWindow::on_actionUndo_triggered()
{
    ui->TextEdit->undo();
}


void MainWindow::on_actionCut_triggered()
{
    ui->TextEdit->cut();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->TextEdit->redo();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->TextEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->TextEdit->paste();
}


void MainWindow::on_TextEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}


void MainWindow::on_TextEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

void MainWindow::on_TextEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}


void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit{color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionEditBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if(color.isValid()){
        ui->TextEdit->setStyleSheet(QString("QPlainTextEdit{backgroundcolor: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionFontBackgroundcolor_triggered()
{

}


void MainWindow::on_action_WordWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->TextEdit->lineWrapMode();

    if(mode == QTextEdit::NoWrap){
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->action_WordWrap->setChecked(true);
    }
    else{
        ui->TextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->action_WordWrap->setChecked(false);
    }
}


void MainWindow::on_actionFontStyle_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok,this);
    if(ok){
        ui->TextEdit->setFont(font);
    }
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}


void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}


void MainWindow::on_actionExit_triggered()
{
    if(userEditConfirmed())
        exit(0);
}


void MainWindow::on_actionAllSelect_triggered()
{
    ui->TextEdit->selectAll();
}


void MainWindow::on_TextEdit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = -1;
    int pos = ui->TextEdit->textCursor().position();
    QString text = ui->TextEdit->toPlainText();

    for(int i =0; i < pos; i++){
        if( text[i] == '\n'){
            ln ++;
            flg = i;
        }
    }

    flg++;
    col = pos - flg;
    statusCursorLabel.setText("Ln: " + QString::number(ln + 1) + " Col: " + QString::number(col + 1));
}


void MainWindow::on_actionLineNum_triggered()
{

}

