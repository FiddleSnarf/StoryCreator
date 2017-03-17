#include "StoryCreator.hpp"
#include "ui_StoryCreator.h"
#include <QGraphicsItem>

StoryCreator::StoryCreator(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::StoryCreator)
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
    // TODO временно для отладки (тут должна быть загрузка типов нодов из json/xml)
    StoryCommon::NodeTemplate node;
    node.toolTip = "lulz";
    node.nodeType = "story";
    node.icon = QIcon(":/story_node_icons/Resources/new_icon.png");

    StoryCommon::SelectNodeList nodeList;
    nodeList << node;

    node.icon = QIcon(":/story_node_icons/Resources/airplane_icon.png");
    nodeList << node;

    node.icon = QIcon(":/story_node_icons/Resources/bear_icon.png");
    nodeList << node;
    // ==========================

    showMaximized();
    initToolBar();
    initConnects();
    initStoryView();
    initSelectNodes(nodeList);
}

void StoryCreator::initStoryView()
{
    ui->storyView->setScene(m_storyManager.getStoryScene());
}

void StoryCreator::initSelectNodes(const StoryCommon::SelectNodeList& nodeList)
{
    ui->viewNodes->setModel(m_storyManager.getStoryNodeSelectModel());
    m_storyManager.setSelectListStoryNodes(nodeList);

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
    connect(m_storyManager.getStoryScene(), &QGraphicsScene::changed, this, &StoryCreator::slotStorySceneChanged);

    // для toolbar
    connect(m_actClear, &QAction::triggered, m_storyManager.getStoryScene(), &StoryScene::clearScene);
}

//=======================================================================================

//============================================ private slots ============================

void StoryCreator::slotStorySceneChanged()
{
    m_nodeCounterView->setText(QString(tr("Node count: %1")).arg(m_storyManager.getCountStoryNodes()));
}

//=======================================================================================
