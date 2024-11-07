#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslDiffieHellmanParameters>
#include <QMap>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    ~Server();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onEncrypted();
    void onSslErrors(const QList<QSslError> &errors);
    void onClientDisconnected();
    void onReadyRead();

private:
    void setupSslConfiguration(QSslSocket *socket);

    QMap<QSslSocket *, QString> clients;  // 维护客户端连接和对应的状态
};

#endif // SERVER_H
