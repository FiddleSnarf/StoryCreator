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

    /** \enum EnStoryCreatorTabs
     *  \brief Перечисление вкладок главного окна StoryCreater.
     */
    enum EnStoryCreatorTabs
    {
        enNodeEditorTabIdx = 0,
        enItemsEditorTabIdx
    };

public:
    explicit StoryCreator(ICorePtr core, QWidget* parent = Q_NULLPTR);
    virtual ~StoryCreator();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void initialize();
    void initToolBar();
    void initConnects();
    void initStoryView();
    void initSelectTemplateNodesView();

    void storyClosed();

private slots:

    /** \brief Слот вызывается при открытии/закрытии истории.
     */
    void slotStoryStateChanged(bool state);

    /** \brief Слот вызывается при изменении состояния выделения нодов.
     */
    void slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode);

    /** \brief Слот вызывается при любом изменении данных или геометрии нодов.
     */
    void slotStoryDataChanged();

    /** \brief Слот вызывается при успешном сохранении истории.
     */
    void slotStorySaved();

    /** \brief Слот вызывается при переключении вкладок редактора истории.
     */
    void slotTabStoryCreatorChanged(int index);


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
    QPointer<class StatusBarWidget> m_statusBar;
};

