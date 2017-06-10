#pragma once

#include <QMainWindow>
#include <QAction>
#include <QLabel>

#include "Core/ICore.hpp"


namespace Ui
{
    class StoryCreator;
}

class StoryManager;

class StoryCreator : public QMainWindow
{
    Q_OBJECT

public:
    explicit StoryCreator(ICorePtr core, QWidget* parent = 0);
    ~StoryCreator();

private:
    void initialize();
    //void initSelectNodes(const StoryCommon::SelectNodeList& nodeList);
    void initStoryView();
    void initToolBar();
    void initConnects();

private slots:
    void slotStorySceneChanged();


private:
    Ui::StoryCreator* ui;

    ICorePtr m_core;
    StoryManagerPtr m_storyManager;

    // toolBar
    QAction* m_actClear;
    QLabel* m_nodeCounterView;
};

