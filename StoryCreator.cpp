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
    m_actClear = new QAction(QIcon(":/tool_bar_icons/Resources/clear.png"), "Clear");
    actions << m_actClear;
    m_ui->mainToolBar->addActions(actions);


    m_ui->mainToolBar->addSeparator();
    m_ui->mainToolBar->addSeparator();
    m_nodeCounterView = new QLabel(QString("Node count: %1").arg(0));
    m_ui->mainToolBar->addWidget(m_nodeCounterView);
}

void StoryCreator::initConnects()
{
    // для story scene/view
    connect(m_storyManager->getStoryScene().data(), &QGraphicsScene::changed, this, &StoryCreator::slotStorySceneChanged);

    // для toolbar
    connect(m_actClear, &QAction::triggered, m_storyManager->getStoryScene().data(), &StoryScene::clearScene);
}

//=======================================================================================

//============================================ private slots ============================

void StoryCreator::slotStorySceneChanged()
{
    m_nodeCounterView->setText(QString(tr("Node count: %1")).arg(m_storyManager->getCountStoryNodes()));
}

//=======================================================================================
