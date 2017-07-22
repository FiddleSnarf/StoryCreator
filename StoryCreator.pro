#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T21:40:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StoryCreator
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
    JsonStoryHelper/NodeAction.cpp \
    JsonStoryHelper/StoryNode.cpp \
    JsonStoryHelper/JsonStoryHelper.cpp \
    Common/StoryTypesNodeCollector.cpp \
    Widgets/NodeInfoWidget.cpp \
    Widgets/StoryNavigationWidget.cpp \
    Widgets/SearchLineWidget.cpp \
    Widgets/StatusBarWidget.cpp

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
    JsonStoryHelper/JsonStoryHelper.h \
    Common/StoryJsonTags.hpp \
    Common/StoryTypesNodeCollector.hpp \
    Widgets/NodeInfoWidget.hpp \
    Widgets/StoryNavigationWidget.hpp \
    Widgets/SearchLineWidget.hpp \
    Widgets/StatusBarWidget.hpp

FORMS    += \
    StoryCreator.ui \
    Widgets/NodeInfoWidget.ui \
    Widgets/StoryNavigationWidget.ui \
    Widgets/SearchLineWidget.ui \
    Widgets/StatusBarWidget.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += Translations\story_creator_ru.ts

win32:RC_FILE = icon.rc
