

#include <QtCore/QThread>
#include "MainWindow.hpp"
#include <sigrun/log/Log.hpp>
#include <regex>

MainWindow::MainWindow(): QMainWindow()
{SIOBJECT("SIREN")

    connect(&d_worker, &UpdateWorker::updated, this, &MainWindow::update_all);
    d_worker.start();
}

MainWindow::~MainWindow()
{
    INFO("Closing MainWindow...");
    INFO("MainWindow closed!");
}

void MainWindow::update_all(double delta, int fps)
{
    INFO("Updated with " + std::to_string(fps) + " fps (real delta time: " + std::to_string(delta) + " )");
}
