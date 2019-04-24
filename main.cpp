#include "service.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Service service;
    service.show();

    return a.exec();
}
