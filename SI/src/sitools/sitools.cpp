#include <QApplication>
#include <QFileSystemWatcher>
#include <QProcess>

#include "sitools.hpp"

void MyClass::handle(const QString &str)
{
    QProcess::startDetached(QString(d_command));
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QFileSystemWatcher watcher;
    watcher.addPath("plugins/env/values/");

    MyClass* mc = new MyClass(QString(argv[1]));

    QObject::connect(&watcher, SIGNAL(directoryChanged(QString)), mc, SLOT(handle(QString)));

    return app.exec();
}