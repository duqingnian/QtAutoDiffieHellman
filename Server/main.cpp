#include "ServerManager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerManager w;
    w.show();
    return a.exec();
}
