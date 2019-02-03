QT += quick androidextras

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

SOURCES += \
    main.cpp \
    notificationclient.cpp

OTHER_FILES += \
    qml/main.qml \
    android-sources/src/de/ur/mi/jh/MainTest.java \
    android-sources/AndroidManifest.xml

RESOURCES += \
    main.qrc

HEADERS += \
    notificationclient.h

target.path = /Users/jurgenhahn/Projekte/Sketchable-Interaction/application_android
INSTALLS += target
