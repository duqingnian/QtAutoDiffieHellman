#include "Dialog.h"
#include "./ui_Dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
    ui->setupUi(this);

    socket = new QSslSocket(this);

    // 设置 SSL 协议
    socket->setProtocol(QSsl::TlsV1_2OrLater);

    connect(socket, &QSslSocket::connected, this, &Dialog::onConnected);
    connect(socket, &QSslSocket::encrypted, this, &Dialog::onEncrypted);
    connect(socket, &QSslSocket::sslErrors, this, &Dialog::onSslErrors);
    connect(socket, &QSslSocket::readyRead, this, &Dialog::onReadyRead);
}

Dialog::~Dialog() {}

void Dialog::onConnected() {
    qDebug() << "Connected to server, starting SSL handshake";
    socket->startClientEncryption();
}

void Dialog::onEncrypted() {
    qDebug() << "SSL encryption established with server";
}

void Dialog::onSslErrors(const QList<QSslError> &errors) {
    for (const QSslError &error : errors) {
        qWarning() << "SSL Error:" << error.errorString();
    }
    socket->ignoreSslErrors();  // 忽略 SSL 错误，仅用于测试环境
}

void Dialog::on_btn_connect_clicked() {
    QString host = ui->edit_ip->text();
    quint16 port = ui->edit_port->text().toUInt();

    socket->connectToHostEncrypted(host, port);
}

void Dialog::on_btn_send_clicked() {
    QString message = ui->edit_msg->text();
    if (socket->isEncrypted() && socket->isOpen()) {
        socket->write(message.toUtf8() + "\n");
        ui->edit_msg_history->appendPlainText("me: " + message);
    }
}

void Dialog::onReadyRead() {
    while (socket->canReadLine()) {
        QString message = QString::fromUtf8(socket->readLine()).trimmed();
        ui->edit_msg_history->appendPlainText("server: " + message);
        qDebug() << "Message from server:" << message;
    }
}
