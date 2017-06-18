#include "StoryCreator.hpp"
#include "StoryManager.hpp"
#include "ui_StoryCreator.h"
#include <QGraphicsItem>


const QString StoryCreator::NODE_COUNT_CAPT = QObject::tr("Node count: %1");


StoryCreator::StoryCreator(ICorePtr core, QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::StoryCreator),
    m_core(core),
    m_storyManager(core->getStoryManager())
{
    m_ui->setupUi(this);
    initialize();
}

StoryCreator::~StoryCreator()
{

}

//===================================== private =========================================

void StoryCreator::initialize()
{
    showMaximized();

    initToolBar();
    initConnects();
    initStoryView();
    initSelectTemplateNodesView();

    m_ui->nodeRedactorTab->setEnabled(false);
    m_ui->itemsRedactorTab->setEnabled(false);
    m_actCloseStory->setEnabled(false);
    m_ui->nodeInfoWidget->setVisible(false);
}

void StoryCreator::initStoryView()
{
    m_ui->storyView->setScene(m_storyManager->getStoryScene().data());
}

void StoryCreator::initSelectTemplateNodesView()
{
    m_ui->viewNodes->setModel(m_storyManager->getStoryNodeSelectModel().data());

    // Устанавливаем размер иконок
    QSize sizeNode = m_ui->viewNodes->size();
    sizeNode.setWidth(sizeNode.width() - 10);
    sizeNode.setHeight(sizeNode.width());
    m_ui->viewNodes->setIconSize(sizeNode);
}

void StoryCreator::initToolBar()
{ 
    m_ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    QList<QAction*> actions;
    m_saveMenu = new QMenu(this);
    m_actSaveStory = new QAction(QIcon(":/tool_bar_icons/Resources/save_story.png"), tr("Save"));
    m_actSaveAsStory = new QAction(QIcon(":/tool_bar_icons/Resources/save_story.png"), tr("Save as..."));
    m_saveMenu->addAction(m_actSaveStory);
    m_saveMenu->addAction(m_actSaveAsStory);
    m_saveMenu->setIcon(QIcon(":/tool_bar_icons/Resources/save_story.png"));

    m_actCreateNewStory = new QAction(QIcon(":/tool_bar_icons/Resources/new_story.png"), tr("Create new story"));
    m_actLoadStory = new QAction(QIcon(":/tool_bar_icons/Resources/load_story.png"), tr("Open story"));
    m_actCloseStory = new QAction(QIcon(":/tool_bar_icons/Resources/close_story.png"), tr("Close story"));
    actions << m_actCreateNewStory << m_actLoadStory << m_saveMenu->menuAction() << m_actCloseStory;
    m_ui->mainToolBar->addActions(actions);

    m_actCloseStory->setEnabled(false);
    m_saveMenu->setEnabled(false);

    m_ui->mainToolBar->addSeparator();
    m_ui->mainToolBar->addSeparator();
    m_nodeCounterView = new QLabel(NODE_COUNT_CAPT.arg("---"));
    m_ui->mainToolBar->addWidget(m_nodeCounterView);
}

void StoryCreator::initConnects()
{
    connect(m_storyManager.data(), &StoryManager::signalStoryStateChanged, this, &StoryCreator::slotStoryStateChanged);

    // Для story scene/view
    const StoryScenePtr scenePtr = m_storyManager->getStoryScene();
    connect(scenePtr.data(), &StoryScene::signalCountStoryNodesChanged, this, &StoryCreator::slotCountStoryNodesChanged);
    connect(scenePtr.data(), &StoryScene::signalItemSelectedChanged, this, &StoryCreator::slotItemSelectedChanged);

    // Для toolbar
    connect(m_actCreateNewStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotCreateNewStory);
    connect(m_actLoadStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotLoadStory);
    connect(m_actCloseStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotCloseStory);

    connect(m_actSaveStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotSaveStory);
    connect(m_actSaveAsStory, &QAction::triggered, m_storyManager.data(), &StoryManager::slotSaveAsStory);
}

void StoryCreator::storyClosed()
{
    const StoryScenePtr scenePtr = m_storyManager->getStoryScene();
    m_ui->storyView->setScene(scenePtr.data());
    connect(scenePtr.data(), &StoryScene::signalCountStoryNodesChanged, this, &StoryCreator::slotCountStoryNodesChanged);
    connect(scenePtr.data(), &StoryScene::signalItemSelectedChanged, this, &StoryCreator::slotItemSelectedChanged);
    m_ui->nodeInfoWidget->setVisible(false);
}

//=======================================================================================

//============================================ private slots ============================

void StoryCreator::slotCountStoryNodesChanged()
{
    if (m_storyManager->isStoryOpen())
        m_nodeCounterView->setText(NODE_COUNT_CAPT.arg(m_storyManager->getCountStoryNodes()));
    else
        m_nodeCounterView->setText(NODE_COUNT_CAPT.arg("---"));
}

void StoryCreator::slotStoryStateChanged(bool state)
{
    m_ui->nodeRedactorTab->setEnabled(state);
    m_ui->itemsRedactorTab->setEnabled(state);
    m_actCloseStory->setEnabled(state);
    m_saveMenu->setEnabled(state);
    m_actSaveStory->setEnabled(m_storyManager->isStoryBeLoaded());
    slotCountStoryNodesChanged();

    // Надо переконнектиться т.к. пересоздали сцену
    if (!state)
        storyClosed();
}

void StoryCreator::slotItemSelectedChanged(bool state, StoryNodeItem* selectedNode)
{
    if (state)
    {
        if (!selectedNode)
            return;

        m_ui->nodeInfoWidget->setCurrentNodeItem(selectedNode);
    }
    m_ui->nodeInfoWidget->setVisible(state);
}

//=======================================================================================
