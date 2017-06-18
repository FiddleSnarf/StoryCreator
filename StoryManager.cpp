#include "StoryManager.hpp"
#include "JsonStoryHelper/JsonStoryHelper.h"
#include "Common/StoryTypesNodeCollector.hpp"

const QString StoryManager::DEF_STORY_FILE_NAME = QObject::tr("Just story");

StoryManager::StoryManager(StoryTypesNodeCollector &collector, QObject* parent) :
    QObject(parent),
    m_typesCollector(collector),
    m_storyScene(new StoryScene(collector)),
    m_storyNodeSelectModel(new SelectNodeModel),
    m_isStoryOpen(false),
    m_isLoadedStory(false)
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

bool StoryManager::isStoryBeLoaded() const
{
    return m_isLoadedStory;
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
    m_currentStoryInfo.version = newStoryInfo.version;
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
        m_isLoadedStory = true;
        m_currentStoryInfo.version = storyInfo.version;
        m_currentStoryInfo.filePath = storyInfo.filePath;
        m_currentStoryInfo.storyName = storyInfo.storyName;

        emit signalStoryStateChanged(m_isStoryOpen);
    }
}

void StoryManager::slotCloseStory()
{
    m_storyScene.reset(new StoryScene(m_typesCollector));
    m_isStoryOpen = false;
    m_isLoadedStory = false;
    m_currentStoryInfo.clear();
    emit signalStoryStateChanged(m_isStoryOpen);
}

void StoryManager::slotSaveStory()
{
    if (m_currentStoryInfo.filePath.isEmpty())
        return;

    if (!saveStory())
        return; // TODO сделать какое-нить сообщение юзеру
}

void StoryManager::slotSaveAsStory()
{
    const QString defaultFileName = m_currentStoryInfo.storyName.isEmpty() ? DEF_STORY_FILE_NAME : m_currentStoryInfo.storyName;
    const QString filePath = JsonStoryHelper::selectSaveStoryFilePath(defaultFileName);
    if (filePath.isEmpty())
        return;

    m_isLoadedStory = true;
    m_currentStoryInfo.filePath = filePath;
    if (!saveStory())
        return; // TODO сделать какое-нить сообщение юзеру
}

bool StoryManager::saveStory()
{
    //m_currentStoryInfo.additionalViewParams; // TODO сделать запись доп параметров графического отображения story в отдельный файл

    // Перезаполним данные нодов в текущей story
    m_currentStoryInfo.nodeList.clear();
    const StoryNodeItemList listNodes = m_storyScene->getStoryNodeList();
    for (StoryNodeItemList::const_iterator it = listNodes.cbegin(); it != listNodes.cend(); ++it)
    {
        m_currentStoryInfo.nodeList << (*it)->getNodeInfo();
    }
    emit signalStoryStateChanged(m_isStoryOpen);
    return JsonStoryHelper::saveJsonStory(m_currentStoryInfo.filePath, m_currentStoryInfo);
}

void StoryManager::slotUpdateStoryName(const QString& storyName)
{
    // TODO Сделать редактирование названия истории
}
