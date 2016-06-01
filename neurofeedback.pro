QT          += sql widgets
CONFIG += -DQT_DEBUG
QMAKE_CXXFLAGS += -std=c++1y -Wall -Wextra -pedantic -fno-inline
INCLUDEPATH += /home/g/prog/openvibe/dependencies/include/
LIBS += -L/home/g/prog/openvibe/dependencies/lib  -lvrpn -lpthread -lasound

HEADERS     = glwidget.h \
              helper.h \
              widget.h \
              window.h \
    volumecontrol.h \
    gamecontroller.h
SOURCES     = glwidget.cpp \
              helper.cpp \
              main.cpp \
              widget.cpp \
              window.cpp \
    volumecontrol.cpp \
    gamecontroller.cpp

# install
# target.path = $$[QT_INSTALL_EXAMPLES]/opengl/2dpainting
INSTALLS += target
wince {
    DEPLOYMENT_PLUGIN += qmysql3
}
