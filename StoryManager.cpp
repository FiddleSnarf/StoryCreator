#include "StoryManager.hpp"

//===================================== public ==========================================

StoryManager::StoryManager(QObject* parent) :
    QObject(parent),
    m_storyScene(new StoryScene),
    m_storyNodeSelectModel(new SelectNodeModel)
{
    initialization();
}

StoryManager::~StoryManager()
{

}

StoryScenePtr StoryManager::getStoryScene()
{
    return m_storyScene;
}

SelectNodeModelPtr StoryManager::getStoryNodeSelectModel()
{
    return m_storyNodeSelectModel;
}

int StoryManager::getCountStoryNodes() const
{
    return m_storyScene->nodeCount();
}

//=======================================================================================

void StoryManager::initialization()
{
    // TODO это для отладки, тут должны быть реальные типы нодов
    StoryCommon::NodeSelectTemplate templateNode;
    templateNode.toolTip = "lulz";
    templateNode.nodeType = "story";
    templateNode.icon = QIcon(":/story_node_icons/Resources/new_icon.png");

    StoryCommon::SelectTNodeList templateNodeList;
    templateNodeList << templateNode;

    templateNode.icon = QIcon(":/story_node_icons/Resources/airplane_icon.png");
    templateNodeList << templateNode;

    templateNode.icon = QIcon(":/story_node_icons/Resources/bear_icon.png");
    templateNodeList << templateNode;
    //===========================================================

    m_storyNodeSelectModel->addTemplateNodesList(templateNodeList);
}
