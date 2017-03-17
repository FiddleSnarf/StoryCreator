#pragma once

#include <QMainWindow>
#include <QAction>
#include <QLabel>

#include "StoryManager.hpp"

namespace Ui
{
    class StoryCreator;
}

class StoryCreator : public QMainWindow
{
    Q_OBJECT

public:
    explicit StoryCreator(QWidget* parent = 0);
    ~StoryCreator();

private:
    void initialize();
    void initSelectNodes(const StoryCommon::SelectNodeList& nodeList);
    void initStoryView();
    void initToolBar();
    void initConnects();

private slots:
    void slotStorySceneChanged();


private:
    Ui::StoryCreator* ui;

    StoryManager m_storyManager;

    // toolBar
    QAction* m_actClear;
    QLabel* m_nodeCounterView;
};

