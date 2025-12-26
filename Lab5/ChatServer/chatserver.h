#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);

    bool isUsernameTaken(const QString &username) const;
    void addUsername(const QString &username);
    void removeUsername(const QString &username);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
    QVector<ServerWorker*>m_clients;
    QSet<QString> m_loggedUsers;

    void broadcast(const QJsonObject &message, ServerWorker *exclude);

signals:
    void logMessage(const QString &msg);

public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void userDiscconnected(ServerWorker *sender);
};

#endif // CHATSERVER_H
