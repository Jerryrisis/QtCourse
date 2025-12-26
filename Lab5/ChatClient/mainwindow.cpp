#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QJsonValue>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    m_chatClient = new ChatClient(this);

    connect(m_chatClient, &ChatClient::connected, this, &MainWindow::connectedToServer);
    // connect(m_chatClient, &ChatClient::messageReceived, this, &MainWindow::messageReceived);
    connect(m_chatClient, &ChatClient::jsonReceived, this, &MainWindow::jsonReceived);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWindowTitleWithUsername(const QString &username)
{
    setWindowTitle(QString("当前用户: %1").arg(username));
}

void MainWindow::on_loginButton_clicked()
{
    m_chatClient->connectToServer(QHostAddress(ui->serverEdit->text()), 1967);
}


void MainWindow::on_sayButton_clicked()
{
    if(!ui->sayLIneEdit->text().isEmpty())
    {
        m_chatClient->sendMessage(ui->sayLIneEdit->text());
    }
}


void MainWindow::on_logoutButton_clicked()
{
    m_chatClient->disconnectFromHost();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);


    for(auto aItem : ui->userlistWidget->findItems(ui->usernameEdit->text(), Qt::MatchExactly))
    {
        qDebug("remove");
        ui->userlistWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MainWindow::connectedToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    QString username = ui->usernameEdit->text(); // 假设输入用户名的控件是 usernameLineEdit
    setWindowTitleWithUsername(username);

    m_chatClient->sendMessage(ui->usernameEdit->text(), "login");


}

void MainWindow::messageReceived(const QString &sender, const QString &text)
{
    ui->roomTextEdit->append(QString("%1 : %2").arg(sender).arg(text));
}

void MainWindow::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    if(typeVal.isNull() || !typeVal.isString())
        return;
    if(typeVal.toString().compare("message",Qt::CaseInsensitive) == 0)
    {
        const QJsonValue textVal = docObj.value("text");
        const QJsonValue senderVal = docObj.value("sender");

        if(textVal.isNull() || !textVal.isString())
            return;

        if(senderVal.isNull() || !senderVal.isString())
            return;


        messageReceived(senderVal.toString(), textVal.toString());

    }
    else if(typeVal.toString().compare("newuser", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userNameVal = docObj.value("username");
        if(userNameVal.isNull() || !userNameVal.isString())
            return;

        userJoined(userNameVal.toString());

    }

    else if(typeVal.toString().compare("userdisconnected", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userNameVal = docObj.value("username");
        if(userNameVal.isNull() || !userNameVal.isString())
            return;

        userLetf(userNameVal.toString());

    }
    else if(typeVal.toString().compare("userlist", Qt::CaseInsensitive) == 0)
    {
        const QJsonValue userlistVal = docObj.value("userlist");
        if(userlistVal.isNull() || !userlistVal.isArray())
            return;

        userListReceived(userlistVal.toVariant().toStringList());

    }
}

void MainWindow::userJoined(const QString &user)
{
    ui->userlistWidget->addItem(user);
}

void MainWindow::userLetf(const QString &user)
{
    for(auto aItem : ui->userlistWidget->findItems(user, Qt::MatchExactly))
    {
        qDebug("remove");
        ui->userlistWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MainWindow::userListReceived(const QStringList &list)
{
    ui->userlistWidget->clear();
    ui->userlistWidget->addItems(list);
}

