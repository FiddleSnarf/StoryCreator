#include "StoryCreator.hpp"
#include "StoryManager.hpp"
#include "ui_StoryCreator.h"
#include <QGraphicsItem>

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
    m_actCreateNewStory = new QAction(QIcon(":/tool_bar_icons/Resources/new_story.png"), tr("Create new story"));
    m_actLoadStory = new QAction(QIcon(":/tool_bar_icons/Resources/load_story.png"), tr("Open story"));
    m_actCloseStory = new QAction(QIcon(":/tool_bar_icons/Resources/close_story.png"), tr("Close story"));
    actions << m_actCreateNewStory << m_actLoadStory << m_actCloseStory;
    m_ui->mainToolBar->addActions(actions);

    m_actCloseStory->setEnabled(false);


    m_ui->mainToolBar->addSeparator();
    m_ui->mainToolBar->addSeparator();
    m_nodeCounterView = new QLabel(tr("The number of nodes: %1").arg(0));
    m_ui->mainToolBar->addWidget(m_nodeCounterView);
}

void StoryCreator::initConnects()
{
    //TODO сделать проверку коннектов
    connect(m_storyManager.data(), &StoryManager::signalStoryOpened, this, &StoryCreator::slotStoryOpened);
    connect(m_storyManager.data(), &StoryManager::signalStoryClosed, this, &StoryCreator::slotStoryClosed);

    // Для story scene/view
    connect(m_storyManager->getStoryScene().data(), &QGraphicsScene::changed, this, &StoryCreator::slotStorySceneChanged);

    // Для toolbar
    connect(m_actCreateNewStory, &QAction::triggered, m_storyManager.data(), &StoryManager::createNewStory);
    connect(m_actLoadStory, &QAction::triggered, m_storyManager.data(), &StoryManager::loadStory);
    connect(m_actCloseStory, &QAction::triggered, m_storyManager.data(), &StoryManager::closeStory);
}

//=======================================================================================

//============================================ private slots ============================

void StoryCreator::slotStorySceneChanged()
{
    m_nodeCounterView->setText(QString(tr("Node count: %1")).arg(m_storyManager->getCountStoryNodes()));
}

void StoryCreator::slotStoryOpened()
{
}

void StoryCreator::slotStoryClosed()
{
}

//=======================================================================================
