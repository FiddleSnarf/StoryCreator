#include "StoryManager.hpp"
#include "JsonStoryHelper/JsonStoryHelper.h"
#include "Common/StoryTypesNodeCollector.hpp"

StoryManager::StoryManager(StoryTypesNodeCollector &collector, QObject* parent) :
    QObject(parent),
    m_typesCollector(collector),
    m_storyScene(new StoryScene(collector)),
    m_storyNodeSelectModel(new SelectNodeModel),
    m_isStoryOpen(false)
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

bool StoryManager::isStoryOpen() const
{
    return m_isStoryOpen;
}

void StoryManager::initialization()
{
    StoryCommon::SelectTNodeList templateNodeList;
    QStringList nodesNameList = m_typesCollector.getNodesNameList();
    foreach(const QString& nodeName, nodesNameList)
    {
        StoryCommon::NodeSelectTemplate templateNode;
        const StoryTypesNodeCollector::TypeInfo typeInfo = m_typesCollector.getNodeTypeInfo(nodeName);

        templateNode.nodeType = nodeName;
        templateNode.toolTip = typeInfo.descriptionType;
        templateNode.icon = typeInfo.iconType;

        templateNodeList << templateNode;
    }
    m_storyNodeSelectModel->addTemplateNodesList(templateNodeList);
}

void StoryManager::slotCreateNewStory()
{
    const StoryCommon::StoryInfo newStoryInfo(StoryCommon::CURR_JSON_VERSION);
    m_storyScene->initStoryInfo(newStoryInfo);
    m_isStoryOpen = true;
    emit signalStoryStateChanged(m_isStoryOpen);
}

void StoryManager::slotLoadStory()
{
    const QString filePath = JsonStoryHelper::selectLoadStoryFilePath();
    if (!filePath.isEmpty())
    {
        StoryCommon::StoryInfo storyInfo;
        if (!JsonStoryHelper::loadJsonStory(filePath, storyInfo))
        {
            return; // TODO сделать какое-нить сообщение пользователю
        }

        // TODO А вот тут надо бы сделать проверку StoryInfo на всякие ошибки, типа дублирование нодов и.т.д
        m_storyScene->initStoryInfo(storyInfo);
        m_isStoryOpen = true;
        emit signalStoryStateChanged(m_isStoryOpen);
    }
}

void StoryManager::slotCloseStory()
{
    m_storyScene.reset(new StoryScene(m_typesCollector));
    m_isStoryOpen = false;
    emit signalStoryStateChanged(m_isStoryOpen);
}
