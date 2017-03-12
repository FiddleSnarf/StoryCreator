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

void StoryCreator::initialize()
{
    // TODO временно для отладки (тут должна быть загрузка типов нодов из json/xml)
    StoryCommon::NodeTemplate node;
    node.toolTip = "lulz";
    node.icon = QIcon(":/story_node_icons/Resources/story_node_icons/new_icon.png");
    node.nodeType = "story";
    StoryCommon::SelectNodeList nodeList;
    nodeList << node << node << node;
    // ==========================
    initStoryScene();
    initSelectNodes(nodeList);
}

void StoryCreator::initStoryScene()
{
    StoryScene& scene = m_storyManager.getStoryScene();
    ui->storyView->setScene(&scene);
}

void StoryCreator::initSelectNodes(const StoryCommon::SelectNodeList& nodeList)
{
    ui->viewNodes->setModel(&m_storyManager.getStoryNodeSelectModel());
    m_storyManager.setSelectListStoryNodes(nodeList);

    QSize sizeNode = ui->viewNodes->size();
    sizeNode.setWidth(sizeNode.width() - 10);
    sizeNode.setHeight(sizeNode.width());
    ui->viewNodes->setIconSize(sizeNode);
}
