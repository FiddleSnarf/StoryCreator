#include <QMessageBox>

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

void StoryManager::connectToScene()
{
    connect(m_storyScene.data(), &StoryScene::signalCountStoryNodesChanged, this, &StoryManager::signalCountStoryNodesChanged);
    connect(m_storyScene.data(), &StoryScene::signalItemSelectedChanged, this, &StoryManager::signalItemSelectedChanged);
}

StoryScenePtr StoryManager::getStoryScene() const
{
    return m_storyScene;
}

SelectNodeModelPtr StoryManager::getStoryNodeSelectModel() const
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
    connectToScene();
}

void StoryManager::slotCreateNewStory()
{
    slotCloseStory();

    const StoryCommon::StoryInfo newStoryInfo(StoryCommon::CURR_JSON_VERSION);
    m_storyScene->initStoryInfo(newStoryInfo);
    m_isStoryOpen = true;
    m_currentStoryInfo.version = newStoryInfo.version;
    emit signalStoryStateChanged(m_isStoryOpen);
}

void StoryManager::slotLoadStory()
{
    slotCloseStory();

    const QString filePath = JsonStoryHelper::selectLoadStoryFilePath();
    if (!filePath.isEmpty())
    {
        StoryCommon::StoryInfo storyInfo;
        if (!JsonStoryHelper::loadJsonStory(filePath, storyInfo))
        {
            QMessageBox::warning(Q_NULLPTR, tr("Attention!"), tr("Error loading history!"));
            return;
        }

        // TODO А вот тут надо бы сделать проверку StoryInfo на всякие ошибки, типа дублирование нодов и.т.д
        m_storyScene->initStoryInfo(storyInfo);
        m_isStoryOpen = true;
        m_isLoadedStory = true;
        m_currentStoryInfo = storyInfo;
        m_storyName = storyInfo.storyName;

        emit signalStoryStateChanged(m_isStoryOpen);
    }
}

void StoryManager::slotCloseStory()
{
    if (!m_isStoryOpen)
        return;

    StoryCommon::StoryInfo updatedStory;
    fillUpdatedStoryInfo(updatedStory);
    if (m_currentStoryInfo != updatedStory)
    {
        if (QMessageBox::question(Q_NULLPTR, tr("Attention!"),
            tr("The current history has been changed, do you want to save the changes?")) == QMessageBox::Yes)
        {
            if (!isStoryBeLoaded())
                slotSaveAsStory();
            else
                saveStory(&updatedStory);
        }
    }

    m_storyScene.reset(new StoryScene(m_typesCollector));
    connectToScene();
    m_isStoryOpen = false;
    m_isLoadedStory = false;
    m_currentStoryInfo.clear();
    m_storyName.clear();
    emit signalStoryStateChanged(m_isStoryOpen);
}

void StoryManager::slotSaveStory()
{
    if (!isStoryBeLoaded())
        return;

    saveStory();
}

void StoryManager::slotSaveAsStory()
{
    const QString defaultFileName = m_storyName.isEmpty() ? DEF_STORY_FILE_NAME : m_storyName;
    const QString filePath = JsonStoryHelper::selectSaveStoryFilePath(defaultFileName);
    if (filePath.isEmpty())
        return;

    m_isLoadedStory = true;
    m_currentStoryInfo.filePath = filePath;
    saveStory();
}

void StoryManager::saveStory(StoryCommon::StoryInfo* updatedStoryPtr)
{
    if (updatedStoryPtr)
    {
        m_currentStoryInfo = *updatedStoryPtr;
    }else
    {
        StoryCommon::StoryInfo updatedStoryInfo;
        fillUpdatedStoryInfo(updatedStoryInfo);
        m_currentStoryInfo = updatedStoryInfo;
    }
    emit signalStoryStateChanged(m_isStoryOpen);
    if (!JsonStoryHelper::saveJsonStory(m_currentStoryInfo.filePath, m_currentStoryInfo))
        QMessageBox::warning(Q_NULLPTR, tr("Attention!"), tr("Error saving history!"));
}

void StoryManager::slotUpdateStoryName(const QString& storyName)
{
    if (isStoryOpen())
        m_storyName = storyName;
}

const QString& StoryManager::getCurrentStoryName() const
{
    return m_storyName;
}

void StoryManager::fillUpdatedStoryInfo(StoryCommon::StoryInfo& updatedStory) const
{
    updatedStory.version = m_currentStoryInfo.version;
    updatedStory.filePath = m_currentStoryInfo.filePath;
    updatedStory.storyName = m_storyName;
    const StoryNodeItemList listNodes = m_storyScene->getStoryNodeList();
    for (StoryNodeItemList::const_iterator it = listNodes.cbegin(); it != listNodes.cend(); ++it)
    {
        updatedStory.nodeList << (*it)->getNodeInfo();
    }
    // Отсортируем ноды по ID
    qSort(updatedStory.nodeList);

    // Обновим параметры отображения
    //updatedStory.additionalViewParams = m_storyScene->getNodesViewParams(); // TODO
}

const StoryNodeItemList& StoryManager::getStoryNodeList() const
{
    if (m_storyScene)
        return m_storyScene->getStoryNodeList();

    static StoryNodeItemList emptyNodeList;
    return emptyNodeList;
}

StoryNodeItemPtr StoryManager::getNodeItemForID(int nodeId) const
{
    return m_storyScene->getNodeItemForID(nodeId);
}

void StoryManager::selectNodeForID(int nodeId, bool centerOn)
{
    m_storyScene->selectNodeForID(nodeId, centerOn);
}

StoryNodeItemPtr StoryManager::getSelectedNodeItem() const
{
    return m_storyScene->getSelectedNodeItem();
}
