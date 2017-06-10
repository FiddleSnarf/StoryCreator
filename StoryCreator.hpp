#pragma once

#include <QMainWindow>
#include <QAction>
#include <QLabel>
#include <QPointer>

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
    explicit StoryCreator(ICorePtr core, QWidget* parent = nullptr);
    ~StoryCreator();

private:
    void initialize();
    void initToolBar();
    void initConnects();
    void initStoryView();
    void initSelectTemplateNodesView();

private slots:
    void slotStorySceneChanged();


private:
    QScopedPointer<Ui::StoryCreator> m_ui;
    ICorePtr m_core;
    StoryManagerPtr m_storyManager;

    // toolBar
    QAction* m_actClear; // TODO всю эту шляпу заменить на отдельный тул бар
    QPointer<QLabel> m_nodeCounterView; // TODO всю эту шляпу заменить на отдельный статус бар
};

