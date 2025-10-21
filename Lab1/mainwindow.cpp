#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<math.h>
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    digitBTNs ={{Qt::Key_0,ui->btnNum0},
              {Qt::Key_1,ui->btnNum1},
              {Qt::Key_2,ui->btnNum2},
              {Qt::Key_3,ui->btnNum3},
              {Qt::Key_4,ui->btnNum4},
              {Qt::Key_5,ui->btnNum5},
              {Qt::Key_6,ui->btnNum6},
              {Qt::Key_7,ui->btnNum7},
              {Qt::Key_8,ui->btnNum8},
              {Qt::Key_9,ui->btnNum9},
               };
    foreach (auto btn, digitBTNs) {
        connect(btn, SIGNAL(clicked()),this ,SLOT(btnNumClicked()));
    }

    connect(ui->btnPlus, SIGNAL(clicked()),this ,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnMinus, SIGNAL(clicked()),this ,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnMultiple, SIGNAL(clicked()),this ,SLOT(btnbinaryOperatorClicked()));
    connect(ui->btnDivide, SIGNAL(clicked()),this ,SLOT(btnbinaryOperatorClicked()));

    connect(ui->btnPercentage, SIGNAL(clicked()),this ,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSqrt, SIGNAL(clicked()),this ,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse, SIGNAL(clicked()),this ,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare, SIGNAL(clicked()),this ,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSign, SIGNAL(clicked()),this ,SLOT(btnUnaryOperatorClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;

    if(operands.size()==2 && opcodes.size()>0){
        double operand1=operands.front().toDouble();
        operands.pop_front();

        double operand2=operands.front().toDouble();
        operands.pop_front();

        QString op=opcodes.front();
        opcodes.pop_front();

        if(op=="+"){
            result=operand1+operand2;
        }
        else if(op=="-"){
            result=operand1-operand2;
        }
        else if(op=="×"){
            result=operand1*operand2;
        }
        else if(op=="/"){
            result=operand1/operand2;
        }
        operands.push_back(QString::number(result));
    }

    return QString::number(result);
}

void MainWindow::btnNumClicked()
{
    QString digit =  qobject_cast<QPushButton*>(sender())->text();
    if(operand=="0" && digit!="."){
        operand.clear();
    }

    operand += digit;
    ui->Display->setText(operand);

}



void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains(".") && !operand.isEmpty())
        operand += qobject_cast<QPushButton*>(sender())->text();
    ui->Display->setText(operand);
}


void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length()-1);
    ui->Display->setText(operand);
}


void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    while(!operands.empty())
        operands.pop_back();
    ui->Display->setText(operand);
}





void MainWindow::btnbinaryOperatorClicked()
{
    QString opcode = qobject_cast<QPushButton*>(sender())->text();

    if(operand !="" && !operand.endsWith(".")){
        operands.push_back(operand);
        operand.clear();

        opcodes.push_back(opcode);


    QString result = calculation();

    ui->Display->setText(result);

    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operand !="" && !operand.endsWith(".")){
        double result=operand.toDouble();
        operand.clear();

        QString op=qobject_cast<QPushButton*>(sender())->text();

        if(op == "%"){
            result /=100.0;
        }
        else if(op == "1/χ"){
            result = 1/result;
        }
        else if(op =="χ^2"){
            result *=result;
        }
        else if(op == "√"){
            result = sqrt(result);
        }
        else if(op == "+/-"){
            result = 0-result;
        }

           ui->Display->setText(QString::number(result));
    }

}



void MainWindow::on_btnEqual_clicked()
{
    if (!operand.isEmpty()) {
        if (operand.endsWith('.')) {
            operand.chop(1);
        }

        if (operand.isEmpty()) {
            operand = "0";
        }
        operands.push_back(operand);
        operand.clear();
    }

    if (operands.size() < 1) {
        ui->Display->setText("没有操作数");
        return;
    }
    if(operands.size() == 1){
        ui->statusbar->showMessage(operands.front());
        ui->Display->setText(operands.front());
    }
    if (operands.size() != opcodes.size() + 1) {
        ui->Display->setText("表达式错误");
        return;
    }

    QString result = calculation();
    ui->Display->setText(result);
}

void MainWindow::on_btnClearAll_clicked()
{
    operand = operand.left(operand.length()-1);
    ui->Display->setText(operand);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    foreach (auto btnKey, digitBTNs.keys()) {
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
    }

    if(event->key() == Qt::Key_Plus)
        ui->btnPlus->animateClick();
    else if(event->key() == Qt::Key_Minus)
        ui->btnMinus->animateClick();
    else if(event->key() == Qt::Key_Backspace)
        ui->btnDel->animateClick();
    else if(event->key() == Qt::Key_Percent)
        ui->btnPercentage->animateClick();
    else if(event->key() == Qt::Key_Slash)
        ui->btnDivide->animateClick();
    else if(event->key() == Qt::Key_Equal)
        ui->btnEqual->animateClick();
}

