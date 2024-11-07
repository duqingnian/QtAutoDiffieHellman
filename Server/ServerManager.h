#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QDialog>
#include "Server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ServerManager;
}
QT_END_NAMESPACE

class ServerManager : public QDialog
{
    Q_OBJECT

public:
    ServerManager(QWidget *parent = nullptr);
    ~ServerManager();

private slots:
    void on_btn_start_clicked();

private:
    Ui::ServerManager *ui;
    Server *_server;


};
#endif // SERVERMANAGER_H
