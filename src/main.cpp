#include <QApplication>
#include <QMessageBox>
#include "gui/MainWindow.h"

/* Application entry point */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Site Blocker");
    app.setApplicationVersion("1.0");
     app.setWindowIcon(QIcon(":/icons/app.ico"));

    MainWindow window;
    window.show();

    return app.exec();
}