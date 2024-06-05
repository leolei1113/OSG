#include "HWCAD.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HWCAD w;
    w.show();
    return a.exec();
}
