

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <iostream>
#include <siren/background/UpdateWorker.hpp>

#include <QMainWindow>
#include <sigrun/SIObject.hpp>

class MainWindow: public QMainWindow, SIObject
{
public:
    MainWindow();
    ~MainWindow();

private:
    void update_all(double delta, int fps);

    UpdateWorker d_worker;
};


#endif //SITEST_MAINWINDOW_HPP
