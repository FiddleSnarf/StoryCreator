#include "StoryManager.hpp"
#include "JsonStoryHelper/JsonStoryHelper.h"

const QString StoryManager::CURR_JSON_VERSION = "1.0";

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

void StoryManager::initialization()
{
    StoryCommon::NodeSelectTemplate templateNode;
    templateNode.toolTip = tr("The most typical node");
    templateNode.nodeType = "common";
    templateNode.icon = QIcon(":/story_node_icons/Resources/new_icon.png");
    //TODO сюда добавить остальные типы нодов

    StoryCommon::SelectTNodeList templateNodeList;
    templateNodeList << templateNode;

    m_storyNodeSelectModel->addTemplateNodesList(templateNodeList);
}

void StoryManager::createNewStory()
{
    m_currentStory = StoryCommon::StoryInfo(CURR_JSON_VERSION);
    emit signalStoryOpened();
}

void StoryManager::loadStoryFile()
{

}
