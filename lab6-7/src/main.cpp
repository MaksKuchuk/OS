#include "view/window.hpp"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow test;
    test.show();

    app.exec();
}