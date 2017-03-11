#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T21:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StroryCreater
TEMPLATE = app


SOURCES += main.cpp\
    Items/StoryNodeItemImpl.cpp \
    Scene/StoryScene.cpp \
    StoryCreator.cpp \
    StoryManager.cpp \
    Views/StoryView.cpp \
    Widgets/ItemsListWidget.cpp \
    Models/SelectNodeModel.cpp

HEADERS  += \
    Items/StoryNodeItemImpl.hpp \
    Scene/StoryScene.hpp \
    Views/StoryView.hpp \
    StoryCreator.hpp \
    StoryManager.hpp \
    Widgets/ItemsListWidget.hpp \
    Models/SelectNodeModel.hpp \
    Common/StoryCommon.hpp

FORMS    += \
    StoryCreator.ui

RESOURCES += \
    resources.qrc
