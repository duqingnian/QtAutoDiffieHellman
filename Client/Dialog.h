#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSslSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btn_connect_clicked();
    void on_btn_send_clicked();
    void onConnected();
    void onEncrypted();
    void onSslErrors(const QList<QSslError> &errors);
    void onReadyRead();

private:
    Ui::Dialog *ui;
    QSslSocket *socket;
};

#endif // DIALOG_H
