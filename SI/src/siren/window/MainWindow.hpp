

#ifndef SITEST_MAINWINDOW_HPP
#define SITEST_MAINWINDOW_HPP

#include <memory>

#include <sigrun/SIObject.hpp>

#include <QMainWindow>
#include <QObject>
#include <QMetaMethod>

#include "../background/UpdateWorker.hpp"
#include "../region/RegionRepresentation.hpp"

class MainWindow: public SIObject, public QMainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();

    void set_is_running(bool running);
private:
    bool d_is_exit_requested = false;

    void draw_background(QPaintEvent* event);
    void draw_region_representations(QPaintEvent* event);

    Q_SLOT void loop(double delta, int fps);

    QMetaMethod fetchIndexOfMethod(QObject* obj, const char* name)
    {
        const QMetaObject* meta_object = obj->metaObject();
        QByteArray normalized_name = QMetaObject::normalizedSignature(name);
        int index = meta_object->indexOfMethod(normalized_name.constData());
        return meta_object->method(index);
    }

// A QObject::connect wrapper
    QMetaObject::Connection dynamicConnection(QObject* source,
                                              const char* signal_name,
                                              QObject* dest,
                                              const char* slot_name)
    {
        return QObject::connect(source, fetchIndexOfMethod(source, signal_name),
                                dest, fetchIndexOfMethod(dest, slot_name));
    }

    std::map<std::string, std::unique_ptr<RegionRepresentation>> d_region_representations;

    std::unique_ptr<UpdateWorker> up_update_worker;
    std::unique_ptr<QPainter> up_qp;

    int d_width, d_height;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};


#endif //SITEST_MAINWINDOW_HPP
