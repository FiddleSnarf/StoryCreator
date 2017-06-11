#include "StoryManager.hpp"
#include "JsonStoryHelper/JsonStoryHelper.h"
#include "Common/StoryTypesNodeCollector.hpp"

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
    StoryTypesNodeCollector typesCollector;
    StoryCommon::SelectTNodeList templateNodeList;
    QStringList nodesNameList = typesCollector.getNodesNameList();
    foreach(const QString& nodeName, nodesNameList)
    {
        StoryCommon::NodeSelectTemplate templateNode;
        const StoryTypesNodeCollector::TypeInfo typeInfo = typesCollector.getNodeTypeInfo(nodeName);

        templateNode.nodeType = nodeName;
        templateNode.toolTip = typeInfo.descriptionType;
        templateNode.icon = typeInfo.iconType;

        templateNodeList << templateNode;
    }
    m_storyNodeSelectModel->addTemplateNodesList(templateNodeList);
}

void StoryManager::createNewStory()
{
    const StoryCommon::StoryInfo newStoryInfo(CURR_JSON_VERSION);
    m_storyScene->initStoryInfo(newStoryInfo);
    emit signalStoryOpened();
}

void StoryManager::loadStory()
{

}

void StoryManager::closeStory()
{

}
