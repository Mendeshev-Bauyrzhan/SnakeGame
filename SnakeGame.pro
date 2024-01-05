TEMPLATE = app
HEADERS += SnakeWidget.h
SOURCES += SnakeWidget.cpp main.cpp
QT += widgets
windows:TARGET = ../SnakeGame
win32:RC_ICONS += snake.ico
