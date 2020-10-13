

#ifndef SITEST_SITOOLS_HPP
#define SITEST_SITOOLS_HPP

#include <QWidget>
#include <QMessageBox>

class MyClass : public QWidget
{
Q_OBJECT

public:
    MyClass(QString command, QWidget* parent=0):
        QWidget(parent),
        d_command(command)
    {}

    ~MyClass()
    {}

private:
    Q_SLOT void handle(const QString& str);

    QString d_command;
};
#endif //SITEST_SITOOLS_HPP
