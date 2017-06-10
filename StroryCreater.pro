#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T21:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StroryCreater
TEMPLATE = app

QMAKE_LFLAGS += -static #вроде как для статик линковки Mingw


SOURCES += main.cpp\
    Items/StoryNodeItemImpl.cpp \
    Scene/StoryScene.cpp \
    StoryCreator.cpp \
    StoryManager.cpp \
    Views/StoryView.cpp \
    Models/SelectNodeModel.cpp \
    Core/CoreImpl.cpp \
    JsonStoryHelper/JsonStoryProvider.cpp \
    JsonStoryHelper/NodeAction.cpp \
    JsonStoryHelper/StoryNode.cpp

HEADERS  += \
    Items/StoryNodeItemImpl.hpp \
    Scene/StoryScene.hpp \
    Views/StoryView.hpp \
    StoryCreator.hpp \
    StoryManager.hpp \
    Models/SelectNodeModel.hpp \
    Common/StoryCommon.hpp \
    Core/CoreImpl.hpp \
    Core/ICore.hpp \
    JsonStoryHelper/NodeAction.h \
    JsonStoryHelper/StoryNode.h \
    JsonStoryHelper/JsonStoryHelper.h

FORMS    += \
    StoryCreator.ui

RESOURCES += \
    resources.qrc
