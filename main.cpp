#include "tonestackcalculator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToneStackCalculator w;
    w.show();
    return a.exec();
}
