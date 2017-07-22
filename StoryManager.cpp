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
    connect(m_storyScene.data(), &StoryScene::signalStoryNodeAdded, this, &StoryManager::signalStoryNodeAdded);
    connect(m_storyScene.data(), &StoryScene::signalStoryNodeDeleted, this, &StoryManager::signalStoryNodeDeleted);
    connect(m_storyScene.data(), &StoryScene::signalItemSelectedChanged, this, &StoryManager::signalItemSelectedChanged);

    connect(m_storyScene.data(), &StoryScene::signalDataNodeChanged, this, &StoryManager::signalDataNodeChanged);
    connect(m_storyScene.data(), &StoryScene::signalGeometryNodeChanged, this, &StoryManager::signalGeometryNodeChanged);
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
    checkDisplayDataStory(m_currentStoryInfo);
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
        checkDisplayDataStory(storyInfo);
        m_storyScene->initStoryInfo(storyInfo);
        m_isStoryOpen = true;
        m_isLoadedStory = true;
        m_currentStoryInfo = storyInfo;
        m_storyName = storyInfo.storyName;

        emit signalStoryStateChanged(m_isStoryOpen);
    }

    // Испускаем сигнал о том что нет несохраненных данных
    emit signalStorySaved();
}

void StoryManager::slotCloseStory()
{
    closeStory();
}

void StoryManager::slotSaveStory()
{
    if (isStoryBeLoaded())
        saveStory();
    else
        slotSaveAsStory();
}

void StoryManager::slotSaveAsStory()
{
    const QString defaultFileName = m_storyName.isEmpty() ? DEF_STORY_FILE_NAME : m_storyName;
    const QString filePath = JsonStoryHelper::selectSaveStoryFilePath(defaultFileName);
    if (filePath.isEmpty())
        return;

    saveStoryAs(filePath);
}

bool StoryManager::saveStory()
{
    StoryCommon::StoryInfo updatedStoryInfo;
    fillUpdatedStoryInfo(updatedStoryInfo);
    emit signalStoryStateChanged(m_isStoryOpen);
    if (!JsonStoryHelper::saveJsonStory(updatedStoryInfo.filePath, updatedStoryInfo))
    {
        QMessageBox::warning(Q_NULLPTR, tr("Attention!"), tr("Error saving history!"));
        return false;
    }

    m_currentStoryInfo = updatedStoryInfo;
    checkDisplayDataStory(m_currentStoryInfo);
    emit signalStorySaved();
    return true;
}

bool StoryManager::saveStoryAs(const QString& fullFilePath)
{
    m_isLoadedStory = true;
    m_currentStoryInfo.filePath = fullFilePath;
    if (!saveStory())
    {
        m_isLoadedStory = false;
        m_currentStoryInfo.filePath.clear();
        return false;
    }
    return true;
}

bool StoryManager::closeStory()
{
    if (!m_isStoryOpen)
        return false;

    StoryCommon::StoryInfo updatedStory;
    fillUpdatedStoryInfo(updatedStory);
    if (m_currentStoryInfo != updatedStory)
    {
        if (QMessageBox::question(Q_NULLPTR, tr("Attention!"),
            tr("The current history has been changed, do you want to save the changes?")) == QMessageBox::Yes)
        {
            if (!isStoryBeLoaded())
            {
                const QString defaultFileName = m_storyName.isEmpty() ? DEF_STORY_FILE_NAME : m_storyName;
                const QString filePath = JsonStoryHelper::selectSaveStoryFilePath(defaultFileName);
                if (filePath.isEmpty())
                    return false;

                if (!saveStoryAs(filePath))
                    return false;
            }
            else
            {
                if (!saveStory())
                    return false;
            }
        }
    }

    m_storyScene.reset(new StoryScene(m_typesCollector));
    connectToScene();
    m_isStoryOpen = false;
    m_isLoadedStory = false;
    m_currentStoryInfo.clear();
    m_storyName.clear();
    emit signalStoryStateChanged(m_isStoryOpen);
    return true;
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
    updatedStory.additionalViewParams = StoryGUI::StoryAdditionalInfo(m_storyScene->getNodesCoordinates());
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
    StoryNodeItemList selectedItems = m_storyScene->getSelectedNodeItems();
    if (!selectedItems.isEmpty())
        return selectedItems.first();

    return StoryNodeItemPtr();
}

void StoryManager::slotUpdateStoryName(const QString& storyName)
{
    if (isStoryOpen() && m_storyName != storyName)
    {
        m_storyName = storyName;
        emit signalStoryNameChanged(storyName);
    }
}

const QString& StoryManager::getCurrentStoryName() const
{
    return m_storyName;
}

void StoryManager::slotDeleteSelectedNode()
{
    m_storyScene->slotDeleteSelectedNode();
}

StoryGUI::EnDisplayDataStates StoryManager::getDisplayDataState() const
{
    return m_currentStoryInfo.additionalViewParams.state;
}

void StoryManager::checkDisplayDataStory(StoryCommon::StoryInfo &storyInfo) const
{
    if (storyInfo.filePath.isEmpty())
    {
        storyInfo.additionalViewParams.state = StoryGUI::EnDisplayDataStates::enNoExist;
        return;
    }

    // Проверяем есть ли файл с графическими параметрами истории
    const QString gFilePath = storyInfo.filePath + StoryGUI::GRAPHIC_FILE_EX;
    if (!QFile::exists(gFilePath))
    {
        storyInfo.additionalViewParams.state = StoryGUI::EnDisplayDataStates::enNoExist;
        return;
    }

    if (storyInfo.additionalViewParams.nodesPosMap.size() != storyInfo.nodeList.size()) // TODO тут нужна бы проверка получше
    {
        storyInfo.additionalViewParams.state = StoryGUI::EnDisplayDataStates::enExistErr;
        return;
    }

    storyInfo.additionalViewParams.state = StoryGUI::EnDisplayDataStates::enExistOk;
}
