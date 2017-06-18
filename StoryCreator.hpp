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
class StoryNodeItem;

class StoryCreator : public QMainWindow
{
    Q_OBJECT

    static const QString NODE_COUNT_CAPT;

public:
    explicit StoryCreator(ICorePtr core, QWidget* parent = Q_NULLPTR);
    virtual ~StoryCreator();

private:
    void initialize();
    void initToolBar();
    void initConnects();
    void initStoryView();
    void initSelectTemplateNodesView();

    void storyClosed();

private slots:
    /** \brief Слот вызывается при добавлении/удалении нодов.
     */
    void slotCountStoryNodesChanged();

    /** \brief Слот вызывается при открытии/закрытии истории.
     */
    void slotStoryStateChanged(bool state);

    /** \brief Слот вызывается при изменении состояния выделения нодов.
     */
    void slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode);


private:
    QScopedPointer<Ui::StoryCreator> m_ui;
    ICorePtr m_core;
    StoryManagerPtr m_storyManager;

    // Tool bar
    QPointer<QMenu>     m_saveMenu;
    QPointer<QAction>   m_actCreateNewStory;
    QPointer<QAction>   m_actLoadStory;
    QPointer<QAction>   m_actCloseStory;
    QPointer<QAction>   m_actSaveStory;
    QPointer<QAction>   m_actSaveAsStory;

    // Status bar
    QPointer<QLabel> m_nodeCounterView; // TODO всю эту шляпу заменить на отдельный статус бар
};

