#include "StoryCreator.hpp"
#include "StoryManager.hpp"
#include "ui_StoryCreator.h"
#include <QGraphicsItem>

StoryCreator::StoryCreator(ICorePtr core, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::StoryCreator),
    m_core(core),
    m_storyManager(core->getStoryManager())
{
    ui->setupUi(this);
    initialize();
}

StoryCreator::~StoryCreator()
{
    delete ui;
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
    ui->storyView->setScene(m_storyManager->getStoryScene().data());
}

void StoryCreator::initSelectTemplateNodesView()
{
    ui->viewNodes->setModel(m_storyManager->getStoryNodeSelectModel().data());

    QSize sizeNode = ui->viewNodes->size();
    sizeNode.setWidth(sizeNode.width() - 10);
    sizeNode.setHeight(sizeNode.width());
    ui->viewNodes->setIconSize(sizeNode);
}

void StoryCreator::initToolBar()
{ 
    ui->mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    QList<QAction*> actions;
    m_actClear = new QAction(QIcon(":/tool_bar_icons/Resources/clear.png"), "Clear");
    actions << m_actClear;
    ui->mainToolBar->addActions(actions);


    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addSeparator();
    m_nodeCounterView = new QLabel(QString("Node count: %1").arg(0));
    ui->mainToolBar->addWidget(m_nodeCounterView);
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
