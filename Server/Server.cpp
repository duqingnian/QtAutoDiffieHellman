#include "Server.h"
#include <QDebug>
#include <QSslKey>
#include <QFile>

Server::Server(QObject *parent) : QTcpServer(parent) {}

Server::~Server() {}

void Server::incomingConnection(qintptr socketDescriptor) {
    auto *socket = new QSslSocket(this);

    if (socket->setSocketDescriptor(socketDescriptor)) {
        // 加载私钥
        QFile keyFile("D:/project/qt/socket/Server/build/Desktop_Qt_6_8_0_MSVC2022_64bit-Release/privateKey.pem");
        if (keyFile.open(QIODevice::ReadOnly)) {
            QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "123456");  // 使用密码加载私钥
            socket->setPrivateKey(privateKey);
            keyFile.close();
        } else {
            qCritical() << "Failed to load private key";
            delete socket;
            return;
        }

        // 设置证书
        socket->setLocalCertificate("D:/project/qt/socket/Server/build/Desktop_Qt_6_8_0_MSVC2022_64bit-Release/certificate.pem");
        socket->setProtocol(QSsl::TlsV1_2OrLater);

        // 设置 SSL 配置和 Diffie-Hellman 参数
        setupSslConfiguration(socket);

        connect(socket, &QSslSocket::encrypted, this, &Server::onEncrypted);
        connect(socket, &QSslSocket::sslErrors, this, &Server::onSslErrors);
        connect(socket, &QSslSocket::disconnected, this, &Server::onClientDisconnected);
        connect(socket, &QSslSocket::readyRead, this, &Server::onReadyRead);

        socket->startServerEncryption();
    } else {
        delete socket;
    }
}

void Server::setupSslConfiguration(QSslSocket *socket) {
    QSslConfiguration sslConfig = socket->sslConfiguration();
    QSslDiffieHellmanParameters dhParams = QSslDiffieHellmanParameters::defaultParameters();
    if (dhParams.isValid()) {
        sslConfig.setDiffieHellmanParameters(dhParams);
    } else {
        qWarning() << "Invalid Diffie-Hellman parameters";
    }
    socket->setSslConfiguration(sslConfig);
}

void Server::onEncrypted() {
    auto *socket = qobject_cast<QSslSocket *>(sender());
    if (socket) {
        qDebug() << "SSL encryption established with client:" << socket->peerAddress().toString();
        clients.insert(socket, socket->peerAddress().toString());
    }
}

void Server::onSslErrors(const QList<QSslError> &errors) {
    for (const QSslError &error : errors) {
        qWarning() << "SSL Error:" << error.errorString();
    }
    auto *socket = qobject_cast<QSslSocket *>(sender());
    if (socket) {
        socket->ignoreSslErrors();  // 忽略 SSL 错误，仅用于测试环境
    }
}

void Server::onClientDisconnected() {
    auto *socket = qobject_cast<QSslSocket *>(sender());
    if (socket) {
        qDebug() << "Client disconnected:" << clients.value(socket);
        clients.remove(socket);
        socket->deleteLater();
    }
}

void Server::onReadyRead() {
    auto *socket = qobject_cast<QSslSocket *>(sender());
    if (socket && socket->isEncrypted()) {
        while (socket->canReadLine()) {
            QString message = QString::fromUtf8(socket->readLine()).trimmed();
            qDebug() << "Message from client:" << message;

            // 回显消息
            socket->write("Server: " + message.toUtf8() + "\n");
        }
    }
}
