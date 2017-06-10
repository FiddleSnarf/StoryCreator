#pragma once

#include <QMainWindow>
#include <QAction>
#include <QLabel>


namespace Ui
{
    class StoryCreator;
}

class StoryManager;

class StoryCreator : public QMainWindow
{
    Q_OBJECT

public:
    explicit StoryCreator(StoryManager& storyManager, QWidget* parent = 0);
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

    StoryManager& m_storyManager;

    // toolBar
    QAction* m_actClear;
    QLabel* m_nodeCounterView;
};

