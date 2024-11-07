#include "ServerManager.h"
#include "./ui_ServerManager.h"

ServerManager::ServerManager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ServerManager)
{
    ui->setupUi(this);
}

ServerManager::~ServerManager()
{
    delete ui;
    _server->close();
    _server->deleteLater();
}

void ServerManager::on_btn_start_clicked()
{
    _server = new Server();
    if (!_server->listen(QHostAddress::Any, 12345))
    {
        ui->plainTextEdit->appendPlainText("Server failed to start");
    }
    else
    {
        ui->plainTextEdit->appendPlainText("Server started on port 12345.");
        ui->btn_start->setEnabled(false);
    }
}

