QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogopcjeobrobkaobrazu.cpp \
    dialogopcjewyswietlacz.cpp \
    dialogpolaczmodbus.cpp \
    grafikaimage.cpp \
    logika.cpp \
    main.cpp \
    mainwindow.cpp \
    modbusmaster.cpp \
    qdialogwlasna.cpp \
    wyswietlaczgrafika.cpp \
    wyswietlaczpixele.cpp \
    wyswietlaczscene.cpp \
    wyswietlaczview.cpp \
    zarzadzaniewysylaniemobrazumodbus.cpp

HEADERS += \
    dialogopcjeobrobkaobrazu.h \
    dialogopcjewyswietlacz.h \
    dialogpolaczmodbus.h \
    grafikaimage.h \
    logika.h \
    mainwindow.h \
    modbusmaster.h \
    qdialogwlasna.h \
    wyswietlaczgrafika.h \
    wyswietlaczpixele.h \
    wyswietlaczscene.h \
    wyswietlaczview.h \
    zarzadzaniewysylaniemobrazumodbus.h

FORMS += \
    dialogopcjeobrobkaobrazu.ui \
    dialogopcjewyswietlacz.ui \
    dialogpolaczmodbus.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myresources.qrc \
    myresources.qrc
